/****************************************************************************
 * uvm_sdv_endpoint_zero.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_ENDPOINT_ZERO_H
#define INCLUDED_UVM_SDV_ENDPOINT_ZERO_H
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_transport.h"

typedef struct uvm_sdv_endpoint_zero_s {
	uvm_sdv_endpoint_t					endpoint;
	uint32_t							msg_id;
} uvm_sdv_endpoint_zero_t;

void uvm_sdv_endpoint_zero_init(uvm_sdv_endpoint_zero_t *endpoint);

uint32_t uvm_sdv_endpoint_zero_alloc_msgid(uvm_sdv_endpoint_zero_t *endpoint);

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_zero_recv_msg(
		uvm_sdv_endpoint_zero_t				*endpoint,
		uint32_t							msg_id);

#endif /* INCLUDED_UVM_SDV_ENDPOINT_ZERO_H */

