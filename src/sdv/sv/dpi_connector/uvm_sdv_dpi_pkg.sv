/****************************************************************************
 * uvm_sdv_dpi_pkg.sv
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_dpi_pkg_sv
`define INCLUDED_uvm_sdv_dpi_pkg_sv

/**
 * Package: uvm_sdv_dpi_pkg
 * 
 * Provides infrastructure to connect native-compiled software
 * with UVM SDV components via DPI. This is primarily useful for testing purposes
 */
`include "uvm_macros.svh" 
package uvm_sdv_dpi_pkg;
	import uvm_pkg::*;
	import uvm_sdv_pkg::*;
	
	import "DPI-C" context function bit _uvm_sdv_dpi_init();

	`include "dpi_connector/uvm_sdv_dpi_connection.svh"
	`include "dpi_connector/uvm_sdv_dpi_connector.svh"
	
	function string _uvm_sdv_dpi_connect(string connector_path);
		automatic uvm_root root = uvm_root::get();
		automatic uvm_sdv_dpi_connection inst;
		automatic uvm_component comps[$], comp;
		
		root.find_all(connector_path, comps);
		
		if (comps.size() == 0) begin
			`uvm_fatal("uvm_sdv_dpi_pkg", $psprintf("Failed to find SDV connector with path %0s", connector_path));
		end else if (comps.size() > 1) begin
			`uvm_fatal("uvm_sdv_dpi_pkg", $psprintf("Found %0d connector with path %0s", 
					comps.size(), connector_path));
		end else begin
			connector_path = comps[0].get_full_name();
		end
		
		inst = uvm_sdv_dpi_connection::inst(connector_path);
	
		return connector_path;
	endfunction
	
	export "DPI-C" function _uvm_sdv_dpi_connect;

	task _uvm_sdv_dpi_write_sw2uvm(
		string			connector_path,
		int unsigned 	data);
		automatic uvm_sdv_dpi_connection inst = uvm_sdv_dpi_connection::inst(connector_path);
		inst.write_sw2uvm(data);
	endtask
	
	export "DPI-C" task _uvm_sdv_dpi_write_sw2uvm;
	
	task _uvm_sdv_dpi_read_uvm2sw(
		string			connector_path,
		output 			int unsigned data,
		output 			int	unsigned ok,
		input 			int unsigned  block);
		automatic uvm_sdv_dpi_connection inst = uvm_sdv_dpi_connection::inst(connector_path);
		inst.read_uvm2sw(data, ok, block);
	endtask
	
	export "DPI-C" task _uvm_sdv_dpi_read_uvm2sw;
	

 	static bit m_initialized = _uvm_sdv_dpi_init();
	
endpackage

`endif /* INCLUDED_uvm_sdv_dpi_pkg_sv */
