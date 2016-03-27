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
 * Class: uvm_sdv_driver
 * 
 * Implements a driver for a UVM SDV sequencer
 */
class uvm_sdv_driver #(type REQ=uvm_object, type RSP=REQ) extends uvm_driver #(REQ, RSP);
	
	typedef uvm_sdv_driver #(REQ,RSP) this_type;
	`uvm_component_param_utils(this_type);
	
	uvm_sdv_blocking_endpoint			m_sdv_endpoint;
	semaphore							m_sem;
	uvm_sequencer_base					m_seqr;

	function new(string name="uvm_sdv_driver", uvm_component parent=null);
		super.new(name, parent);
		
		m_sem = new(1);
	endfunction
	
	/**
	 * Function: connect_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void connect_phase(input uvm_phase phase);
			
		// Create an endpoint
		m_sdv_endpoint = new(get_full_name());
		m_sdv_endpoint.set_endpoint_type("SEQR");
	endfunction

	/**
	 * Task: run_phase
	 *
	 * Override from class uvm_component
	 */
	virtual task run_phase(input uvm_phase phase);
		REQ						req;
		RSP						rsp;
		uvm_sdv_connector_msg	msg;
		uvm_sdv_packer			packer;
		
		packer = new();
		
		// Determine whether or not we're connected to a sequencer
		begin
			uvm_port_list		pl;
			
			seq_item_port.get_connected_to(pl);
			
			if (pl.size() == 0) begin
				m_seqr = null;
			end else if (pl.size() == 1) begin
				foreach (pl[name]) begin
					uvm_component p = pl[name].get_parent();
					
					if (!$cast(m_seqr, p)) begin
						`uvm_fatal(get_name(), "Connected object is not a sequencer");
					end
					break;
				end
			end else begin
				`uvm_fatal(get_name(), "More than one sequencer connected");
			end
		end
		
		forever begin
			int unsigned tmp[];
			
			seq_item_port.get_next_item(req);
			
			
			// Pack this sequence item and send it to the SW
			msg = new();
			packer.init(msg);

			// Pack the object to the message
			void'(req.pack_ints(tmp, packer));
			
			m_sdv_endpoint.send_msg(msg);
		
			// Receive response or acknowledgement 
			// that the sequence item is complete
			m_sdv_endpoint.recv_msg(msg);
			
			packer.init(msg);
			
			if (packer.is_null()) begin
				// A null response means no response
				seq_item_port.item_done();
			end else begin
				// Unpack the response
				rsp = RSP::type_id::create();
				rsp.do_unpack(packer);
				seq_item_port.item_done(rsp);
			end
		end

	endtask

endclass

