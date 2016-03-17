/****************************************************************************
 * uvm_sdv_transport.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_TRANSPORT_H
#define INCLUDED_UVM_SDV_TRANSPORT_H
#include "uvm_sdv_config.h"
#include "uvm_sdv_transport_msg.h"



typedef struct uvm_sdv_transport_s {
	void									*prv_data;

	uvm_sdv_transport_msg_t *(*recv_msg)(
			struct uvm_sdv_transport_s		*tp,
			uint32_t						block);

	/**
	 * Sends or queues the message. The message
	 * data structure is transfers to the transport, and
	 * may not be subsequently changed by the caller.
	 */
	int (*send_msg)(
			struct uvm_sdv_transport_s		*tp,
			uvm_sdv_transport_msg_t			*msg);

	uvm_sdv_transport_msg_t *(*alloc_msg)(
			struct uvm_sdv_transport_s		*tp,
			uint32_t						msg_len);

	void (*free_msg)(
			struct uvm_sdv_transport_s		*tp,
			uvm_sdv_transport_msg_t			*msg);

} uvm_sdv_transport_t;


#endif /* INCLUDED_UVM_SDV_TRANSPORT_H */
