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
 * Package: uvm_sdv_dpi_pkg
 * 
 * Provides infrastructure to connect native-compiled software
 * with UVM SDV components via DPI. This is primarily useful for testing purposes
 */
`include "uvm_macros.svh" 
package uvm_sdv_dpi_pkg;
	import uvm_pkg::*;
	import uvm_sdv_pkg::*;
	
	import "DPI-C" context function bit _uvm_sdv_dpi_init();

	`include "dpi_connector/uvm_sdv_dpi_connection.svh"
	`include "dpi_connector/uvm_sdv_dpi_connector.svh"
	
	function string _uvm_sdv_dpi_connect(string connector_path);
		automatic uvm_root root = uvm_root::get();
		automatic uvm_sdv_dpi_connection inst;
		automatic uvm_component comps[$], comp;
		
		root.find_all(connector_path, comps);
		
		if (comps.size() == 0) begin
			`uvm_fatal("uvm_sdv_dpi_pkg", $psprintf("Failed to find SDV connector with path %0s", connector_path));
		end else if (comps.size() > 1) begin
			`uvm_fatal("uvm_sdv_dpi_pkg", $psprintf("Found %0d connector with path %0s", 
					comps.size(), connector_path));
		end else begin
			connector_path = comps[0].get_full_name();
		end
		
		inst = uvm_sdv_dpi_connection::inst(connector_path);
	
		return connector_path;
	endfunction
	
	export "DPI-C" function _uvm_sdv_dpi_connect;

	task _uvm_sdv_dpi_write_sw2uvm(
		string			connector_path,
		int unsigned 	data);
		automatic uvm_sdv_dpi_connection inst = uvm_sdv_dpi_connection::inst(connector_path);
		inst.write_sw2uvm(data);
	endtask
	
	export "DPI-C" task _uvm_sdv_dpi_write_sw2uvm;
	
	task _uvm_sdv_dpi_read_uvm2sw(
		string			connector_path,
		output 			int unsigned data,
		output 			int	unsigned ok,
		input 			int unsigned  block);
		automatic uvm_sdv_dpi_connection inst = uvm_sdv_dpi_connection::inst(connector_path);
		inst.read_uvm2sw(data, ok, block);
	endtask
	
	export "DPI-C" task _uvm_sdv_dpi_read_uvm2sw;
	

 	static bit m_initialized = _uvm_sdv_dpi_init();
	
endpackage

