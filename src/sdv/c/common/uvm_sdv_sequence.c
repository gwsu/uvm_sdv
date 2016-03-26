/****************************************************************************
 * uvm_sdv_sequence.c
 ****************************************************************************/
#include "uvm_sdv_config.h"
#include "uvm_sdv_sequence.h"
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_endpoint_mgr_int.h"
#include "uvm_sdv_packer.h"

/********************************************************************
 * uvm_sdv_sequence_start()
 *
 * Launches the sequence with type-name 'seq' on sequencer with
 * instance path 'seqr'.
 *
 * Returns:
 * Sequence instance id (>= 0) on success
 * -1 on failure
 ********************************************************************/
uint32_t uvm_sdv_sequence_start(
		const char				*seq,
		const char				*seqr)
{
	uvm_sdv_endpoint_mgr_t *ep_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_endpoint_zero_t *ep_zero = &ep_mgr->endpoint_zero;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;
	uint32_t msg_id, status;
	uint32_t seq_id = 0;

	tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_START_SEQ;

	msg_id = uvm_sdv_endpoint_zero_alloc_msgid(ep_zero);

	// TODO: mutex
	ep_mgr->sequence_id++;
	seq_id = ep_mgr->sequence_id;

	uvm_sdv_pack_int(&packer, msg_id, 32);
	uvm_sdv_pack_int(&packer, seq_id, 32);
	uvm_sdv_pack_string(&packer, seq);
	uvm_sdv_pack_string(&packer, seqr);

	tp->send_msg(tp, tp_msg);

	// Wait for an acknowledge
	tp_msg = uvm_sdv_endpoint_mgr_recv_msgid(tp, UVM_SDV_START_SEQ_RSP, msg_id);

	// Check the response
	uvm_sdv_packer_init(&packer, tp_msg);
	// Discard the msg_id
	uvm_sdv_unpack_int(&packer, 32);
	status = uvm_sdv_unpack_int(&packer, 32);

	if (status) {
		// TODO: mutex
		// Allocate a sequence id
		if (ep_mgr->active_sequence_list_sz >= ep_mgr->active_sequence_list_max) {
			// Need to resize the array
			uint32_t *ex_list = ep_mgr->active_sequence_list;

			ep_mgr->active_sequence_list_max += 16;
			ep_mgr->active_sequence_list = (uint32_t *)malloc(
					sizeof(uint32_t)*(ep_mgr->active_sequence_list_max));

			if (ex_list) {
				memcpy(ep_mgr->active_sequence_list, ex_list,
						sizeof(uint32_t)*ep_mgr->active_sequence_list_sz);
				free(ex_list);
			}
		}

		ep_mgr->active_sequence_list[ep_mgr->active_sequence_list_sz] = seq_id;
		ep_mgr->active_sequence_list_sz++;
	} else {
		seq_id = 0;
	}

	tp->free_msg(tp, tp_msg);

	// Poll to see if there are messages waiting to be processed
	uvm_sdv_endpoint_mgr_poll();

	return seq_id;
}

/********************************************************************
 * uvm_sdv_sequence_is_running()
 ********************************************************************/
uint32_t uvm_sdv_sequence_is_running(uint32_t seq_id)
{
	uvm_sdv_endpoint_mgr_t *ep_mgr = uvm_sdv_endpoint_mgr_get();
	uint32_t i, ret=0;

	// Poll to see if there are messages waiting to be processed
	uvm_sdv_endpoint_mgr_poll();

	// Search the sequence list
	for (i=0; i<ep_mgr->active_sequence_list_sz; i++) {
		if (ep_mgr->active_sequence_list[i] == seq_id) {
			ret = 1;
			break;
		}
	}

	return ret;
}

/********************************************************************
 * uvm_sdv_sequence_end_sequence()
 ********************************************************************/
void uvm_sdv_sequence_ended(uint32_t seq_id)
{
	uvm_sdv_endpoint_mgr_t *ep_mgr = uvm_sdv_endpoint_mgr_get();
	int32_t idx = -1;
	uint32_t i;

	// First, find the index
	for (i=0; i<ep_mgr->active_sequence_list_sz; i++) {
		if (ep_mgr->active_sequence_list[i] == seq_id) {
			idx = i;
			break;
		}
	}

	if (idx != -1) {
		// Update the list
		for (i=idx; i<(ep_mgr->active_sequence_list_sz-1); i++) {
			ep_mgr->active_sequence_list[i] =
					ep_mgr->active_sequence_list[i+1];
		}
		ep_mgr->active_sequence_list_sz--;
	}
}
