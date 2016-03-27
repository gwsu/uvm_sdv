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

class uvm_sdv_connector_msg extends uvm_object;
	`uvm_object_utils(uvm_sdv_connector_msg);

	bit[15:0]					endpoint_id;
	bit[15:0]					msg_type;
	bit[31:0]					msg_len;
	bit[31:0]					msg_data[];

	function void put(bit[31:0] data);
		if (msg_len >= msg_data.size()) begin
			bit[31:0] tmp[] = new[msg_data.size()+64];
			// Resize the message
			foreach (msg_data[i]) begin
				tmp[i] = msg_data[i];
			end
			msg_data = tmp;
		end
		msg_data[msg_len++] = data;
	endfunction

endclass

