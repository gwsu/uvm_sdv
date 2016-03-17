
#ifndef INCLUDED_UVM_SDV_TRANSPORT_MSG_H
#define INCLUDED_UVM_SDV_TRANSPORT_MSG_H

typedef struct uvm_sdv_transport_msg_s {
	uint16_t			endpoint_id;
	uint16_t			msg_type;
	uint32_t			msg_len;
	uint32_t			*msg_data;

	// Management fields used by the transport
	uint32_t							msg_data_max;
	struct uvm_sdv_transport_msg_s		*next;
} uvm_sdv_transport_msg_t;

#endif /* INCLUDED_UVM_SDV_TRANSPORT_MSG_H */
