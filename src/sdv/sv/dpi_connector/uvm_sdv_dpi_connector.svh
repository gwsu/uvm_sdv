/****************************************************************************
 * uvm_sdv_dpi_connector.svh
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_dpi_connector_svh
`define INCLUDED_uvm_sdv_dpi_connector_svh

/**
 * Class: uvm_sdv_dpi_connector
 * 
 * Implements the uvm_sdv_connector API using DPI
 */
class uvm_sdv_dpi_connector extends uvm_sdv_connector;
	
	`uvm_component_utils(uvm_sdv_dpi_connector)

	function new(string name, uvm_component parent);
		super.new(name, parent);

	endfunction

	/**
	 * Function: build_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void build_phase(input uvm_phase phase);
		super.build_phase(phase);
		// Create the connection
		void'(uvm_sdv_dpi_connection::inst(get_full_name()));
	endfunction


	/**
	 * Task: recv_msg
	 *
	 * Override from class uvm_sdv_connector
	 */
	virtual task recv_msg(output uvm_sdv_connector_msg msg);
		uvm_sdv_dpi_connection conn = uvm_sdv_dpi_connection::inst(get_full_name());
		conn.recv_msg(msg);
	endtask

	/**
	 * Task: send_msg
	 *
	 * Override from class uvm_sdv_connector
	 */
	virtual task send_msg(input uvm_sdv_connector_msg msg);
		uvm_sdv_dpi_connection conn = uvm_sdv_dpi_connection::inst(get_full_name());
		conn.send_msg(msg);
	endtask

endclass

`endif /* INCLUDED_uvm_sdv_dpi_connector_svh */
