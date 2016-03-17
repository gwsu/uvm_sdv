

#include "uvm_sdv_phasing.h"
#include "uvm_sdv_packer.h"
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_endpoint_mgr_int.h"


void uvm_sdv_raise_objection(const char *name, int count)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_msg_t *tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_RAISE_OBJECTION;
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, count, 32);

	tp->send_msg(tp, tp_msg);
}

void uvm_sdv_drop_objection(const char *name, int count)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_msg_t *tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_DROP_OBJECTION;
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, count, 32);

	tp->send_msg(tp, tp_msg);
}
