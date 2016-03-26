#include "uvm_sdv_dpi_transport.h"
#include "uvm_sdv_config.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#else
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#endif

typedef void *svScope;

#if defined(__CYGWIN__) || defined(_WIN32) || defined(__MINGW32__)
#define EXTERN_I __declspec(dllimport)
#define EXTERN_E __declspec(dllexport)
#else
#define EXTERN_I
#define EXTERN_E
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif


EXTERN_I svScope svGetScopeFromName(const char *name);
EXTERN_I svScope svGetScope(void);
EXTERN_I void svSetScope(const svScope);
EXTERN_I const char *svGetNameFromScope(const svScope);

static char prv_scope[1024];

typedef struct dpi_transport_s {
	uvm_sdv_transport_msg_t		*msg_alloc;
	void (*write_func)(const char *, uint32_t data);
	void (*read_func)(const char *, uint32_t *data, uint32_t *ok, uint32_t block);

	uint32_t					hdr_tmp[2];
	uint32_t					hdr_tmp_idx;

	const char					*connection;
	svScope						scope;
} dpi_transport_t;

void (*uvm_sdv_dpi_write_sw2uvm_p)(const char *, uint32_t) = 0;
void (*uvm_sdv_dpi_read_uvm2sw_p)(const char *, uint32_t *, uint32_t *, uint32_t) = 0;
const char *(*uvm_sdv_dpi_connect_p)(const char *) = 0;

/*
void _uvm_sdv_dpi_write_sw2uvm(const char *, uint32_t);
void _uvm_sdv_dpi_read_uvm2sw(const char *, uint32_t*, uint32_t*, uint32_t);
const char *_uvm_sdv_dpi_connect(const char *path_name);
 */

static uvm_sdv_transport_msg_t *dpi_transport_recv_msg(
		uvm_sdv_transport_t				*tp,
		uint32_t						block)
{
	dpi_transport_t *tp_prv = (dpi_transport_t *)tp->prv_data;
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);
	uint32_t data, ok, tmp, msg_idx;
	svScope scope = svGetScopeFromName(prv_scope);

	svSetScope(scope);

	// TODO: mutex

	// Wait until we can read at least two words
	while (tp_prv->hdr_tmp_idx < 2) {
		tp_prv->read_func(tp_prv->connection, &data, &ok, block);

		if (ok) {
			tp_prv->hdr_tmp[tp_prv->hdr_tmp_idx++] = data;
		} else {
			break;
		}
	}

	// Non-blocking, no data
	if (tp_prv->hdr_tmp_idx < 2) {
		return 0;
	}

	tmp = tp_prv->hdr_tmp[0];
	msg->endpoint_id = (tmp >> 16);
	msg->msg_type = (tmp & 0xFFFF);

	tmp = tp_prv->hdr_tmp[1];
	msg->msg_len = tmp;

	tp_prv->hdr_tmp_idx = 0;

	if (msg->msg_data_max < msg->msg_len) {
		// Re-alloc the buffer
		if (msg->msg_data) {
			free(msg->msg_data);
		}
		msg->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg->msg_len);
	}

	msg_idx = 0;
	while (msg_idx < msg->msg_len) {

		tp_prv->read_func(tp_prv->connection, &data, &ok, 1);

		msg->msg_data[msg_idx++] = data;
	}

	// TODO: mutex

	return msg;
}

static int dpi_transport_send_msg(
		uvm_sdv_transport_t				*tp,
		uvm_sdv_transport_msg_t			*msg)
{
	dpi_transport_t *tp_prv = (dpi_transport_t *)tp->prv_data;
	uint32_t tmp;
	uint32_t msg_idx = 0;
	svScope scope = svGetScopeFromName(prv_scope);

	svSetScope(scope);

	// TODO: mutex

	tmp = (msg->endpoint_id << 16 | msg->msg_type);
	tp_prv->write_func(tp_prv->connection, tmp);
	tp_prv->write_func(tp_prv->connection, msg->msg_len);

	while (msg_idx < msg->msg_len) {
		tp_prv->write_func(tp_prv->connection, msg->msg_data[msg_idx++]);
	}

	// TODO: mutex

	return 0;
}

static uvm_sdv_transport_msg_t *dpi_transport_alloc_msg(
		uvm_sdv_transport_t				*tp,
		uint32_t						msg_len)
{
	dpi_transport_t *tp_prv = (dpi_transport_t *)tp->prv_data;
	uvm_sdv_transport_msg_t *ret;

	// TODO: mutex
	if (tp_prv->msg_alloc) {
		ret = tp_prv->msg_alloc;
		tp_prv->msg_alloc = ret->next;

		if (ret->msg_data_max < msg_len) {
			free(ret->msg_data);
			ret->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg_len);
			ret->msg_data_max = msg_len;
		}
	} else {
		// Allocate a new message
		ret = (uvm_sdv_transport_msg_t *)malloc(sizeof(uvm_sdv_transport_msg_t));
		if (msg_len > 0) {
			ret->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg_len);
			ret->msg_data_max = msg_len;
		} else {
			ret->msg_data = 0;
			ret->msg_data_max = 0;
		}
	}

	// Initialize fields
	ret->msg_len = 0;
	ret->next = 0;

	// TODO: mutex

	return ret;
}

static void dpi_transport_free_msg(
		uvm_sdv_transport_t				*tp,
		uvm_sdv_transport_msg_t			*msg)
{
	dpi_transport_t *tp_prv = (dpi_transport_t *)tp->prv_data;
	// TODO: mutex
	msg->next = tp_prv->msg_alloc;
	tp_prv->msg_alloc = msg;
	// TODO: mutex
}

DLL_EXPORT int _uvm_sdv_dpi_init(void)
{
	svScope scope = svGetScope();
	strcpy(prv_scope, svGetNameFromScope(scope));
	fprintf(stdout, "uvm_sdv_dpi_init\n");
	fflush(stdout);
	if (!uvm_sdv_dpi_connect_p) {
#if defined(_WIN32) || defined(__CYGWIN__)
		// This is Windows- and Questa-specific code
		HANDLE hndl = GetModuleHandle("export_tramp.dll");
		if (!hndl) {
			hndl = GetModuleHandle(0); // try default
		}
		uvm_sdv_dpi_connect_p = (const char *(*)(const char *))GetProcAddress(hndl, "_uvm_sdv_dpi_connect");
		uvm_sdv_dpi_read_uvm2sw_p = (void (*)(const char *, uint32_t *, uint32_t *, uint32_t))GetProcAddress(hndl, "_uvm_sdv_dpi_read_uvm2sw");
		uvm_sdv_dpi_write_sw2uvm_p = (void (*)(const char *, uint32_t))GetProcAddress(hndl, "_uvm_sdv_dpi_write_sw2uvm");
#else
		void *hndl = 0;
		uvm_sdv_dpi_connect_p = (const char *(*)(const char *))dlsym(hndl, "_uvm_sdv_dpi_connect");
		uvm_sdv_dpi_read_uvm2sw_p = (void (*)(const char *, uint32_t *, uint32_t *, uint32_t))dlsym(hndl, "_uvm_sdv_dpi_read_uvm2sw");
		uvm_sdv_dpi_write_sw2uvm_p = (void (*)(const char *, uint32_t))dlsym(hndl, "_uvm_sdv_dpi_write_sw2uvm");
#endif
	}
	return 1;
}

DLL_EXPORT uvm_sdv_transport_t *uvm_sdv_dpi_transport_create(const char *agent_path)
{
	uvm_sdv_transport_t *tp = (uvm_sdv_transport_t *)malloc(sizeof(uvm_sdv_transport_t));
	dpi_transport_t *tp_prv = (dpi_transport_t *)malloc(sizeof(dpi_transport_t));
	svScope scope = svGetScopeFromName(prv_scope);
	const char *full_agent_path;

	svSetScope(scope);

	memset(tp_prv, 0, sizeof(dpi_transport_t));

	full_agent_path = uvm_sdv_dpi_connect_p(agent_path);
	tp_prv->connection = strdup(full_agent_path);
	tp_prv->write_func = uvm_sdv_dpi_write_sw2uvm_p;
	tp_prv->read_func = uvm_sdv_dpi_read_uvm2sw_p;

	tp->prv_data = tp_prv;
	tp_prv->msg_alloc = 0;

	tp->recv_msg = &dpi_transport_recv_msg;
	tp->send_msg = &dpi_transport_send_msg;
	tp->alloc_msg = &dpi_transport_alloc_msg;
	tp->free_msg = &dpi_transport_free_msg;

	return tp;
}
