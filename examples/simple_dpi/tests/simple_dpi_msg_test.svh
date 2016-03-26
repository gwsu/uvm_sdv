/****************************************************************************
 * simple_dpi_msg_test.svh
 ****************************************************************************/
`ifndef INCLUDED_simple_dpi_msg_test_svh
`define INCLUDED_simple_dpi_msg_test_svh

/**
 * Class: simple_dpi_msg_test
 * 
 * TODO: Add class documentation
 */
class simple_dpi_msg_test extends simple_dpi_test_base;
	`uvm_component_utils(simple_dpi_msg_test)
	
	function new(string name, uvm_component parent);
		super.new(name, parent);
	endfunction

	/**
	 * Function: build_phase
	 *
	 * Override from class amber_uvm_sdv_test_base
	 */
	virtual function void build_phase(input uvm_phase phase);
		super.build_phase(phase);
	endfunction

	/**
	 * Task: run_phase
	 *
	 * Override from class amber_uvm_sdv_test_base
	 */
	virtual task run_phase(input uvm_phase phase);
		phase.raise_objection(this, "Main");
		test_main("*.m_sdv_connector");
		phase.drop_objection(this, "Main");
	endtask

endclass

`endif /* INCLUDED_simple_dpi_msg_test_svh */
