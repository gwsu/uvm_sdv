/****************************************************************************
 * uvm_sdv_endpoint_mgr.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_ENDPOINT_MGR_H
#define INCLUDED_UVM_SDV_ENDPOINT_MGR_H
#include "uvm_sdv_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	UVM_SDV_ENDPOINT_SW2UVM,
	UVM_SDV_ENDPOINT_UVM2SW,
	UVM_SDV_ENDPOINT_BIDIR
} uvm_sdv_endpoint_dir_e;

typedef void (*uvm_sdv_endpoint_recv_func)(void *ud, uvm_sdv_transport_msg_t *msg);

typedef struct uvm_sdv_endpoint_s {
	uint32_t					endpoint_id;
	uvm_sdv_endpoint_dir_e		endpoint_dir;
	const char					*inst_name;
	const char					*type_name;
	uvm_sdv_transport_msg_t		*recv_msg_queue;
	uvm_sdv_endpoint_recv_func   recv_func;
	void						*recv_func_ud;
} uvm_sdv_endpoint_t;

void uvm_sdv_endpoint_mgr_init(uvm_sdv_transport_t *tp);

uvm_sdv_transport_t *uvm_sdv_endpoint_mgr_get_transport(void);

// TODO: Need a run-loop method

void uvm_sdv_endpoint_init(
		uvm_sdv_endpoint_t				*endpoint,
		const char						*inst_name,
		const char						*type_name,
		uvm_sdv_endpoint_dir_e			dir,
		uvm_sdv_endpoint_recv_func		recv_func,
		void							*recv_func_ud);

uint32_t uvm_sdv_endpoint_mgr_poll(void);

void uvm_sdv_endpoint_mgr_free_msg(uvm_sdv_transport_msg_t *msg);

void uvm_sdv_endpoint_queue_msg(uvm_sdv_endpoint_t *ep, uvm_sdv_transport_msg_t *msg);

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_mgr_recv_msg(
		uvm_sdv_transport_t 		*tp,
		uvm_sdv_endpoint_t 			*endpoint);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_UVM_SDV_ENDPOINT_MGR_H */
