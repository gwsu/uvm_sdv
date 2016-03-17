
#include "uvm_sdv_analysis_port.h"
#include "uvm_sdv_endpoint_mgr_int.h"

void uvm_sdv_analysis_port_init(
		uvm_sdv_analysis_port_t			*ap,
		const char						*name,
		uvm_sdv_pack_func				pack_f)
{
	ap->pack_func = pack_f;
	uvm_sdv_endpoint_init(&ap->endpoint, name, "AP",
			UVM_SDV_ENDPOINT_SW2UVM, 0, 0);
}

/**
 * Writes a transaction to an analysis port within the
 * UVM environment
 */
void uvm_sdv_analysis_port_write(
		uvm_sdv_analysis_port_t			*ap,
		void							*obj)
{
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, msg);

	msg->endpoint_id = ap->endpoint.endpoint_id;
	// TODO: message type (?)
	msg->msg_type = 0;
	ap->pack_func(&packer, obj);

	tp->send_msg(tp, msg);
}
