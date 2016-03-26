
class simple_dpi_seq_driver_test extends simple_dpi_test_base;
	
	`uvm_component_utils(simple_dpi_seq_driver_test)
	
	/****************************************************************
	 * Data Fields
	 ****************************************************************/
	
	/****************************************************************
	 * new()
	 ****************************************************************/
	function new(string name, uvm_component parent=null);
		super.new(name, parent);
	endfunction

	/****************************************************************
	 * build_phase()
	 ****************************************************************/
	function void build_phase(uvm_phase phase);
		super.build_phase(phase);
	endfunction

	/****************************************************************
	 * connect_phase()
	 ****************************************************************/
	function void connect_phase(uvm_phase phase);
		super.connect_phase(phase);
	endfunction

	/****************************************************************
	 * run_phase()
	 ****************************************************************/
	task run_phase(uvm_phase phase);
		phase.raise_objection(this, "Run Phase");
		test_main("*.m_sdv_connector");
		phase.drop_objection(this, "Run Phase");
	endtask	
	
endclass



