#include "uvm_sdv_dpi_transport.h"
#include "uvm_sdv_config.h"
#include <dlfcn.h>

typedef void *svScope;

svScope svGetScopeFromName(const char *name);
svScope svGetScope(void);
void svSetScope(const svScope);
const char *svGetNameFromScope(const svScope);

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

extern void _uvm_sdv_dpi_write_sw2uvm(const char *, uint32_t);
extern void _uvm_sdv_dpi_read_uvm2sw(const char *, uint32_t*, uint32_t*, uint32_t);
extern const char *_uvm_sdv_dpi_connect(const char *path_name);

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

int _uvm_sdv_dpi_init(void)
{
	svScope scope = svGetScope();
	strcpy(prv_scope, svGetNameFromScope(scope));
	return 1;
}

uvm_sdv_transport_t *uvm_sdv_dpi_transport_create(const char *agent_path)
{
	uvm_sdv_transport_t *tp = (uvm_sdv_transport_t *)malloc(sizeof(uvm_sdv_transport_t));
	dpi_transport_t *tp_prv = (dpi_transport_t *)malloc(sizeof(dpi_transport_t));
	svScope scope = svGetScopeFromName(prv_scope);
	const char *full_agent_path;
//	void *f;

	svSetScope(scope);

	memset(tp_prv, 0, sizeof(dpi_transport_t));

	/*
	if ((f = dlsym(0, "_uvm_sdv_dpi_write_sw2uvm"))) {
		tp_prv->write_func = (void (*)(uint32_t))f;
	} else {
		fprintf(stderr, "Failed to locate symbol _uvm_sdv_dpi_write_sw2uvm: %s\n",
				dlerror());
		exit(1);
	}

	if ((f = dlsym(0, "_uvm_sdv_dpi_read_uvm2sw"))) {
		tp_prv->read_func = (void (*)(uint32_t*, uint32_t*, uint32_t))f;
	} else {
		fprintf(stderr, "Failed to locate symbol _uvm_sdv_dpi_read_uvm2sw: %s\n",
				dlerror());
		exit(1);
	}
	 */

	full_agent_path = _uvm_sdv_dpi_connect(agent_path);
	tp_prv->connection = strdup(full_agent_path);
	tp_prv->write_func = &_uvm_sdv_dpi_write_sw2uvm;
	tp_prv->read_func = &_uvm_sdv_dpi_read_uvm2sw;

	tp->prv_data = tp_prv;
	tp_prv->msg_alloc = 0;

	tp->recv_msg = &dpi_transport_recv_msg;
	tp->send_msg = &dpi_transport_send_msg;
	tp->alloc_msg = &dpi_transport_alloc_msg;
	tp->free_msg = &dpi_transport_free_msg;

	return tp;
}
