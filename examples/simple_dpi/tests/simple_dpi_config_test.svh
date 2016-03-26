
class simple_dpi_config_test extends simple_dpi_test_base;
	`uvm_component_utils(simple_dpi_config_test)
	
	function new(string name, uvm_component parent=null);
		super.new(name, parent);
	endfunction
	
	function void build_phase(uvm_phase phase);
		sw_txn txn = sw_txn::type_id::create();

		uvm_config_db #(uvm_sdv_config_type_int)::set(this, "*.m_sdv_connector*",
			"TEST_CONFIG", 'h55aabbcc);
		
		txn.A = 5;
		txn.B = 27;
		uvm_config_db #(uvm_sdv_config_type_object)::set(this, "*.m_sdv_connector*",
			"TEST_CONFIG_TXN", txn);
	
		super.build_phase(phase);
	endfunction

	task main_phase(uvm_phase phase);
		phase.raise_objection(this, "Main");
		
		test_main("*.m_sdv_connector");
		
		phase.drop_objection(this, "Main");
	endtask
	
	/**
	 * Function: report_phase
	 *
	 * Override from class 
	 */
//	virtual function void report_phase(input uvm_phase phase);
//		uvm_report_server svr = get_report_server();
//		if (svr.get_severity_count(UVM_FATAL) == 0 &&
//				svr.get_severity_count(UVM_ERROR) == 0) begin
//			$display("PASS: uvm_sdv_config_test");
//		end else begin
//			$display("FAIL: uvm_sdv_config_test");
//		end
//	endfunction
	
endclass


	