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
 * Class: uvm_sdv_dpi_connector
 * 
 * Implements the uvm_sdv_connector API using DPI
 */
class uvm_sdv_dpi_connector extends uvm_sdv_connector;
	
	`uvm_component_utils(uvm_sdv_dpi_connector)

	function new(string name, uvm_component parent);
		super.new(name, parent);

	endfunction

	/**
	 * Function: build_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void build_phase(input uvm_phase phase);
		super.build_phase(phase);
		// Create the connection
		void'(uvm_sdv_dpi_connection::inst(get_full_name()));
	endfunction


	/**
	 * Task: recv_msg
	 *
	 * Override from class uvm_sdv_connector
	 */
	virtual task recv_msg(output uvm_sdv_connector_msg msg);
		uvm_sdv_dpi_connection conn = uvm_sdv_dpi_connection::inst(get_full_name());
		conn.recv_msg(msg);
	endtask

	/**
	 * Task: send_msg
	 *
	 * Override from class uvm_sdv_connector
	 */
	virtual task send_msg(input uvm_sdv_connector_msg msg);
		uvm_sdv_dpi_connection conn = uvm_sdv_dpi_connection::inst(get_full_name());
		conn.send_msg(msg);
	endtask

endclass

