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

class uvm_sdv_packer extends uvm_packer;
	
	uvm_sdv_connector_msg			m_msg;
	int								m_idx;
	
	function void init(uvm_sdv_connector_msg msg);
		m_msg = msg;
		m_idx = 0;
	endfunction
	
	/**
	 * Function: get_bits
	 *
	 * Override from class uvm_packer
	 */
	virtual function void get_bits(ref bit unsigned bits[]);
		`uvm_fatal("uvm_sdv_packer", "get_bits is unimplemented");
	endfunction

	/**
	 * Function: get_bytes
	 *
	 * Override from class uvm_packer
	 */
	virtual function void get_bytes(ref byte unsigned bytes[]);
		`uvm_fatal("uvm_sdv_packer", "get_bytes is unimplemented");
	endfunction

	/**
	 * Function: get_ints
	 *
	 * Override from class uvm_packer
	 */
	virtual function void get_ints(ref int unsigned ints[]);
		ints = m_msg.msg_data;
	endfunction

	/**
	 * is_null()
	 *
	 * Override from class uvm_packer
	 */
	virtual function bit is_null();
		return (m_msg.msg_data[m_idx] == 0);
	endfunction

	/**
	 * pack_field()
	 *
	 * Override from class uvm_packer
	 */
	virtual function void pack_field(input uvm_bitstream_t value, input int size);
		if (size <= 64) begin
			pack_field_int(value, size);
		end else begin
			`uvm_fatal("uvm_sdv_packer", $psprintf("pack_field size of %0d exceeds 64-bit max", size));
		end
	endfunction

	/**
	 * pack_field_int()
	 *
	 * Override from class uvm_packer
	 */
	virtual function void pack_field_int(input logic[63:0] value, input int size);
		if (size <= 32) begin
			m_msg.put(value[31:0]);
		end else begin
			m_msg.put(value[31:0]);
			m_msg.put(value[63:32]);
		end
	endfunction

	/**
	 * pack_string()
	 *
	 * Override from class uvm_packer
	 */
	virtual function void pack_string(input string value);
		bit[31:0] tmp = 0;
		bit[31:0] i = 0;
		bit[31:0] len = (value.len()+1+4)/4;
		
		m_msg.put(len);
		
		for (int i=0; i<len; i++) begin
			tmp = 0;
			for (int j=0; j<4; j++) begin
				tmp |= (value.getc(4*i+j) << 8*j);
			end
			m_msg.put(tmp);
		end
	endfunction

	/**
	 * unpack_field_int()
	 *
	 * Override from class uvm_packer
	 */
	virtual function logic[63:0] unpack_field_int(input int size);
		bit[63:0] ret = 0;
		
		if (size <= 32) begin
			ret = m_msg.msg_data[m_idx++];
		end else begin
			ret[31:0] = m_msg.msg_data[m_idx++];
			ret[63:32] = m_msg.msg_data[m_idx++];
		end
		
		return ret;
	endfunction

	/**
	 * unpack_string()
	 *
	 * Override from class uvm_packer
	 */
	virtual function string unpack_string(input int num_chars=-1);
		bit[31:0] tmp;
		string ret;
		bit valid = 1;
		
		bit[31:0] len = m_msg.msg_data[m_idx++];

		for (int i=0; i<len; i++) begin
			tmp = m_msg.msg_data[m_idx++];
			
			ret = {ret, tmp[7:0]};
			ret = {ret, tmp[15:8]};
			ret = {ret, tmp[23:16]};
			ret = {ret, tmp[31:24]};
		end
		
		return ret;
	endfunction

endclass

