
`include "uvm_macros.svh"
package uvm_sdv_pkg;
	import uvm_pkg::*;
	
	typedef uvm_object				uvm_sdv_config_type_object;
	typedef bit[63:0]				uvm_sdv_config_type_int;
	typedef string 					uvm_sdv_config_type_string;
	
	`include "common/uvm_sdv_connector_msg.svh"
	
	`include "common/uvm_sdv_endpoint_base.svh"
	`include "common/uvm_sdv_blocking_endpoint.svh"
	`include "common/uvm_sdv_nonblocking_endpoint.svh"
	
	`include "common/uvm_sdv_packer.svh"

	`include "common/uvm_sdv_nonblocking_endpoint_imp.svh"
	`include "common/uvm_sdv_publisher.svh"

	`include "common/uvm_sdv_sequence_runner.svh"
	`include "common/uvm_sdv_connector.svh"
	
	`include "common/uvm_sdv_driver.svh"
	
endpackage
