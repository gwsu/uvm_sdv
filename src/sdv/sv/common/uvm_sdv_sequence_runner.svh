/****************************************************************************
 * uvm_sdv_sequence_runner.svh
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_sequence_runner_svh
`define INCLUDED_uvm_sdv_sequence_runner_svh

/**
 * Class: uvm_sdv_sequence_runner
 * 
 * TODO: Add class documentation
 */
typedef class uvm_sdv_connector;

class uvm_sdv_sequence_runner;
	uvm_sdv_connector				m_sdv_connector;
	bit[31:0]						m_seq_id;
	uvm_sequencer_base				m_seqr;
	uvm_sequence_base				m_seq;

	function new(
		uvm_sdv_connector			sdv_connector,
		bit[31:0] 					seq_id,
		uvm_sequencer_base			seqr,
		uvm_sequence_base			seq);
		m_sdv_connector = sdv_connector;
		m_seq_id = seq_id;
		m_seqr = seqr;
		m_seq = seq;
	endfunction

	task run();
		if (m_seq.do_not_randomize) begin
			if (!m_seq.randomize()) begin
				`uvm_error("uvm_sdv_sequence_runner", "Failed to randomize sequence");
			end
		end
		m_seq.start(m_seqr);
		m_sdv_connector.m_sequence_done(m_seq_id);
	endtask

endclass

`endif /* INCLUDED_uvm_sdv_sequence_runner_svh */
