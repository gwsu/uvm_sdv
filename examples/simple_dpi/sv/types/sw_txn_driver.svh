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
 
`ifndef INCLUDED_sw_txn_driver_svh
`define INCLUDED_sw_txn_driver_svh

/**
 * Class: sw_txn_driver
 * 
 * TODO: Add class documentation
 */
class sw_txn_driver extends uvm_driver #(sw_txn);
	`uvm_component_utils(sw_txn_driver)
	
	uvm_analysis_port #(sw_txn)				ap;

	function new(string name, uvm_component parent);
		super.new(name, parent);
		
		ap = new("ap", this);
	endfunction

	/**
	 * Task: run_phase
	 *
	 * Override from class uvm_component
	 */
	virtual task run_phase(input uvm_phase phase);
		sw_txn txn;
		
		forever begin
			seq_item_port.get_next_item(txn);
			
			uvm_report_info(get_name(), $psprintf("Receive TXN: A=%0d B=%0d", txn.A, txn.B));
	
			ap.write(txn);
			
			seq_item_port.item_done();
		end
	endtask

endclass

`endif /* INCLUDED_sw_txn_driver_svh */
