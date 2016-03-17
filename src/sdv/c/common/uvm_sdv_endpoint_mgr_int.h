/****************************************************************************
 * uvm_sdv_endpoint_mgr_int.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_ENDPOINT_MGR_INT_H
#define INCLUDED_UVM_SDV_ENDPOINT_MGR_INT_H
#include "uvm_sdv_transport.h"
#include "uvm_sdv_endpoint_zero.h"

typedef enum {
	UVM_SDV_MSG_PRINT = 0,
	UVM_SDV_MSG_INFO,
	UVM_SDV_MSG_WARN,
	UVM_SDV_MSG_ERROR,
	UVM_SDV_MSG_FATAL,
	UVM_SDV_RAISE_OBJECTION,
	UVM_SDV_DROP_OBJECTION,
		// endpoint_id
		// inst_name
		// type_name
		// dir
	UVM_SDV_REGISTER_ENDPOINT, // 7

		// msg_id
		// seq_id
		// sequence type-name
		// sequencer path
	UVM_SDV_START_SEQ,

		// msg_id
		// status
	UVM_SDV_START_SEQ_RSP,

		// seq_id
	UVM_SDV_SEQ_DONE,

		// msg_id
		// config name
		// config type
	UVM_SDV_GET_CONFIG,

		// msg_id
		// config_exists
		// config_data
	UVM_SDV_GET_CONFIG_RSP

} uvm_sdv_msg_e;


typedef struct uvm_sdv_endpoint_mgr_s {
	uvm_sdv_transport_t					*transport;
	uvm_sdv_endpoint_t					**endpoint_list;
	uint32_t							endpoint_list_len;
	uint32_t							endpoint_list_max;

	uvm_sdv_endpoint_zero_t				endpoint_zero;

	// Sequence management data
	uint32_t							sequence_id;
	uint32_t							*active_sequence_list;
	uint32_t							active_sequence_list_sz;
	uint32_t							active_sequence_list_max;
} uvm_sdv_endpoint_mgr_t;

uvm_sdv_transport_t *uvm_sdv_endpoint_mgr_get_transport(void);

uvm_sdv_endpoint_mgr_t *uvm_sdv_endpoint_mgr_get(void);

void uvm_sdv_packstr(uint32_t *data, uint32_t *idx, const char *str);

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_mgr_recv_msg(
		uvm_sdv_transport_t 	*tp,
		uvm_sdv_endpoint_t 		*endpoint);

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_mgr_recv_msgid(
		uvm_sdv_transport_t 		*tp,
		uvm_sdv_msg_e				msg_type,
		int32_t						msg_id);

#endif /* INCLUDED_UVM_SDV_ENDPOINT_MGR_INT_H */
