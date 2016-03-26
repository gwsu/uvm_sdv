/****************************************************************************
 * simple_dpi_sdv_connector.svh
 ****************************************************************************/

/**
 * Class: simple_dpi_sdv_connector
 * 
 * TODO: Add class documentation
 */
class simple_dpi_sdv_connector extends uvm_sdv_dpi_connector;
	`uvm_component_utils(simple_dpi_sdv_connector)

	function new(string name, uvm_component parent);
		super.new(name, parent);
	endfunction

	/**
	 * Task: recv_msg
	 *
	 * Override from class 
	 */
	virtual task recv_msg(output uvm_sdv_connector_msg msg);
		#1;
		super.recv_msg(msg);
	endtask

	/**
	 * Task: send_msg
	 *
	 * Override from class 
	 */
	virtual task send_msg(input uvm_sdv_connector_msg msg);
		super.send_msg(msg);
	endtask

	

endclass


