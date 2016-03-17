#ifndef INCLUDED_UVM_SDV_ANALYSIS_PORT_H
#define INCLUDED_UVM_SDV_ANALYSIS_PORT_H
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_packer.h"

typedef struct uvm_sdv_analysis_port_s {
	uvm_sdv_endpoint_t				endpoint;
	uvm_sdv_pack_func				pack_func;
} uvm_sdv_analysis_port_t;


void uvm_sdv_analysis_port_init(
		uvm_sdv_analysis_port_t			*ap,
		const char						*name,
		uvm_sdv_pack_func				pack_f);

void uvm_sdv_analysis_port_write(
		uvm_sdv_analysis_port_t			*ap,
		void							*obj);


#endif /* INCLUDED_UVM_SDV_ANALYSIS_PORT_H */
