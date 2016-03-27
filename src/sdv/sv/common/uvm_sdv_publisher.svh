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
 * Class: uvm_sdv_publisher
 * 
 */
class uvm_sdv_publisher #(type T=uvm_object) extends uvm_component;
	uvm_analysis_port #(T)								analysis_port;
	
	typedef uvm_sdv_publisher #(T) this_type;
	uvm_sdv_nonblocking_endpoint_imp #(this_type)		nonblocking_endpoint_imp;
	uvm_sdv_connector									m_sdv_connector;
	
	`uvm_component_param_utils(this_type);
	
	function new(string name, uvm_component parent);
		super.new(name, parent);
		nonblocking_endpoint_imp = new(get_full_name(), this);
		analysis_port = new("analysis_port", this);
	endfunction

	/**
	 * Function: connect_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void connect_phase(input uvm_phase phase);
		/*
		m_sdv_agent = nonblocking_endpoint_imp.m_agent;
		assert(m_sdv_agent != null) else
			`uvm_fatal(get_name(), "m_sdv_agent is null");
		
		// TODO: connect
		 *  
		 */

	endfunction
	
	

	
	/**
	 * Function: message_received
	 *
	 * Override from class uvm_sdv_nonblocking_endpoint
	 */
	virtual function void message_received(uvm_sdv_connector_msg msg);
		T item = T::type_id::create();
		
		uvm_sdv_packer packer = new();
		packer.init(msg);
	
		item.do_unpack(packer);
		
		analysis_port.write(item);
	endfunction
	
endclass
