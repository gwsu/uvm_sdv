/****************************************************************************
 * simple_dpi_ap_test.svh
 ****************************************************************************/
`ifndef INCLUDED_simple_dpi_ap_test_svh
`define INCLUDED_simple_dpi_ap_test_svh

/**
 * Class: simple_dpi_ap_test
 * 
 * TODO: Add class documentation
 */
 
class simple_dpi_ap_test extends simple_dpi_test_base;
	`uvm_component_utils(simple_dpi_ap_test)
	
	uvm_tlm_analysis_fifo #(sw_txn)			m_analysis_fifo;
	int										m_transaction_count;

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

		m_analysis_fifo = new("m_analysis_fifo", this);
	endfunction

	/**
	 * Function: connect_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void connect_phase(input uvm_phase phase);
		m_env.m_sw_txn_publisher.analysis_port.connect(m_analysis_fifo.analysis_export);
	endfunction

	/**
	 * Task: run_phase
	 *
	 * Override from class amber_uvm_sdv_test_base
	 */
	virtual task run_phase(input uvm_phase phase);
		sw_txn txn;
		int stream_id;
		int trans_h = -1;
		
		fork 
			begin
				test_main("*.m_sdv_connector");
			end
		join_none
		
		forever begin
			m_analysis_fifo.get(txn);
			
			`uvm_info(get_name(), $psprintf("txn: A=%0d B=%0d", txn.A, txn.B), UVM_MEDIUM);
			if (txn.A+5 != txn.B) begin
				`uvm_error(get_name(), $psprintf("Expect B == A+5; A=%0d B=%0d", txn.A, txn.B));
			end
			m_transaction_count++;
		end
	endtask

	/**
	 * Function: report_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void report_phase(input uvm_phase phase);
		if (m_transaction_count != 5) begin
			`uvm_error(get_name(), $psprintf("Expecting 5 transactions; received %0d", m_transaction_count));
		end
	endfunction

endclass

`endif /* INCLUDED_simple_dpi_ap_test_svh */
