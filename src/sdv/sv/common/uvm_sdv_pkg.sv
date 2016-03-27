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

`include "uvm_macros.svh"
package uvm_sdv_pkg;
	import uvm_pkg::*;
	
	typedef uvm_object				uvm_sdv_config_type_object;
	typedef bit[63:0]				uvm_sdv_config_type_int;
	typedef string 					uvm_sdv_config_type_string;
	
	`include "common/uvm_sdv_connector_msg.svh"
	
	`include "common/uvm_sdv_endpoint_base.svh"
	`include "common/uvm_sdv_blocking_endpoint.svh"
	`include "common/uvm_sdv_nonblocking_endpoint.svh"
	
	`include "common/uvm_sdv_packer.svh"

	`include "common/uvm_sdv_nonblocking_endpoint_imp.svh"
	`include "common/uvm_sdv_publisher.svh"

	`include "common/uvm_sdv_sequence_runner.svh"
	`include "common/uvm_sdv_connector.svh"
	
	`include "common/uvm_sdv_driver.svh"
	
endpackage
