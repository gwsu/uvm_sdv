/****************************************************************************
 * uvm_sdv_config_db.c
 ****************************************************************************/
#include "uvm_sdv_config_db.h"
#include "uvm_sdv_endpoint_mgr_int.h"

typedef enum {
	UVM_SDV_CFG_TYPE_INT,
	UVM_SDV_CFG_TYPE_STR,
	UVM_SDV_CFG_TYPE_OBJ
} uvm_sdv_cfg_type_e;

uint32_t uvm_sdv_config_db_get_int(
		const char 		*name,
		uint64_t 			*val)
{
	uint32_t cfg_ex = 0;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_endpoint_mgr_t *ep_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_t packer;
	uint32_t msg_id;

	uvm_sdv_packer_init(&packer, msg);

	msg->endpoint_id = 0;
	msg->msg_type = UVM_SDV_GET_CONFIG;

	msg_id = uvm_sdv_endpoint_zero_alloc_msgid(&ep_mgr->endpoint_zero);

	uvm_sdv_pack_int(&packer, msg_id, 32);
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, UVM_SDV_CFG_TYPE_INT, 32);

	tp->send_msg(tp, msg);

	// Now, wait for the response
	msg = uvm_sdv_endpoint_mgr_recv_msgid(tp, UVM_SDV_GET_CONFIG_RSP, msg_id);

	// Disect the response
	uvm_sdv_packer_init(&packer, msg);

	// Throw away the message id
	uvm_sdv_unpack_int(&packer, 32);

	// Check to see if the config requested exists
	cfg_ex = uvm_sdv_unpack_int(&packer, 32);

	if (cfg_ex) {
		*val = uvm_sdv_unpack_int(&packer, 64);
	}

	// Return the message to the transport
	tp->free_msg(tp, msg);

	return cfg_ex;
}

int32_t uvm_sdv_config_db_get_string(
		const char 		*name,
		char			*val,
		uint32_t		 len)
{
	uint32_t cfg_ex = 0;
	uvm_sdv_transport_t     *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_endpoint_mgr_t  *ep_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_t packer;
	uint32_t msg_id;
	int32_t val_len;

	uvm_sdv_packer_init(&packer, msg);

	msg->endpoint_id = 0;
	msg->msg_type = UVM_SDV_GET_CONFIG;

	msg_id = uvm_sdv_endpoint_zero_alloc_msgid(&ep_mgr->endpoint_zero);

	uvm_sdv_pack_int(&packer, msg_id, 32);
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, UVM_SDV_CFG_TYPE_STR, 32);

	tp->send_msg(tp, msg);

	// Now, wait for the response
	msg = uvm_sdv_endpoint_mgr_recv_msgid(tp, UVM_SDV_GET_CONFIG_RSP, msg_id);

	// Disect the response
	uvm_sdv_packer_init(&packer, msg);

	// Throw away the message id
	uvm_sdv_unpack_int(&packer, 32);

	// Check to see if the config requested exists
	cfg_ex = uvm_sdv_unpack_int(&packer, 32);

	if (cfg_ex) {
		val_len = uvm_sdv_unpack_string(&packer, 0, 0);
		uvm_sdv_unpack_string(&packer, val, len);
	} else {
		*val = 0;
	}

	// Return the message to the transport
	tp->free_msg(tp, msg);

	return (cfg_ex)?val_len:-1;
}

uint32_t uvm_sdv_config_db_get_object(
		const char 		*name,
		uvm_sdv_pack_func 	pack_func,
		void				*obj)
{
	uint32_t cfg_ex = 0;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_endpoint_mgr_t *ep_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_t packer;
	uint32_t msg_id;

	uvm_sdv_packer_init(&packer, msg);

	msg->endpoint_id = 0;
	msg->msg_type = UVM_SDV_GET_CONFIG;

	msg_id = uvm_sdv_endpoint_zero_alloc_msgid(&ep_mgr->endpoint_zero);

	uvm_sdv_pack_int(&packer, msg_id, 32);
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, UVM_SDV_CFG_TYPE_OBJ, 32);

	tp->send_msg(tp, msg);

	// Now, wait for the response
	msg = uvm_sdv_endpoint_mgr_recv_msgid(tp, UVM_SDV_GET_CONFIG_RSP, msg_id);

	// Disect the response
	uvm_sdv_packer_init(&packer, msg);

	// Throw away the message id
	uvm_sdv_unpack_int(&packer, 32);

	// Check to see if the config requested exists
	cfg_ex = uvm_sdv_unpack_int(&packer, 32);

	if (cfg_ex) {
		pack_func(&packer, obj);
	}

	// Return the message to the transport
	tp->free_msg(tp, msg);

	return cfg_ex;
}

