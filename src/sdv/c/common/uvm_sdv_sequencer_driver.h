/****************************************************************************
 * uvm_sdv_sequencer_driver.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H
#define INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H
#include "uvm_sdv_packer.h"
#include "uvm_sdv_endpoint_mgr.h"

typedef struct uvm_sdv_sequencer_driver_s {
	uvm_sdv_endpoint_t			endpoint;
	uvm_sdv_pack_func			req_pack_f;
	uvm_sdv_pack_func			rsp_pack_f;
} uvm_sdv_sequencer_driver_t;

void uvm_sdv_sequencer_driver_init(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		const char						*name,
		uvm_sdv_pack_func				req_pack_f,
		uvm_sdv_pack_func				rsp_pack_f);

void uvm_sdv_sequencer_driver_get_next_item(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item);

void uvm_sdv_sequencer_driver_item_done(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item);

#endif /* INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H */
