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

class uvm_sdv_blocking_endpoint extends uvm_sdv_endpoint_base;
	`uvm_object_utils(uvm_sdv_blocking_endpoint)
	
	mailbox #(uvm_sdv_connector_msg)	m_mb;
	
	function new(string name="uvm_sdv_blocking_endpoint");
		super.new(name);
		m_mb = new(16);
	endfunction
	
	task recv_msg(output uvm_sdv_connector_msg msg);
		m_mb.get(msg);
	endtask

	/**
	 * Task: m_msg_received
	 *
	 * Override from class uvm_sdv_endpoint_base
	 */
	virtual task m_msg_received(input uvm_sdv_connector_msg msg);
		m_mb.put(msg);
	endtask
	
endclass
	