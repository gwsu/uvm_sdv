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
 * Class: uvm_sdv_nonblocking_endpoint_imp
 * 
 * TODO: Add class documentation
 */
class uvm_sdv_nonblocking_endpoint_imp #(type T=int) extends uvm_sdv_nonblocking_endpoint;
	
	typedef uvm_sdv_nonblocking_endpoint_imp #(T) this_type;
	
	T							m_parent;

	function new(string inst_name, T parent=null);
		super.new(inst_name);
		m_parent = parent;
		assert($cast(m_parent, parent)) else
			`uvm_fatal(get_name(), $psprintf("Parent %p is not an instance of target class", parent));
	endfunction

	/**
	 * Function: message_received
	 *
	 * Override from class uvm_sdv_nonblocking_endpoint
	 */
	virtual function void message_received(input uvm_sdv_connector_msg msg);
		m_parent.message_received(msg);
	endfunction
	
endclass

