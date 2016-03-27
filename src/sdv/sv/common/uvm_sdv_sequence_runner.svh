/****************************************************************************
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License. 
 ****************************************************************************/

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

