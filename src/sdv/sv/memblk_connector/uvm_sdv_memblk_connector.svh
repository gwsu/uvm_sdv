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

//
// Class: uvm_sdv_memblk_connector
//
// Implements the Software-Driven Verification connector that 
// communicates with the embedded software.
//
class uvm_sdv_memblk_connector extends uvm_sdv_connector;
	`uvm_component_utils(uvm_sdv_memblk_connector)
	static string			MEMBLK_ADDR_CFG = "MEMBLK_ADDR";
	static string			MEMBLK_SIZE_CFG = "MEMBLK_SIZE";
	static int				MAX_MSG_SZ = 1024*1024;
	
	bit[63:0]				m_memblk;
	
	typedef struct {
		bit[63:0]			write_ptr;
		bit[63:0]			read_ptr;
		bit[31:0]			sz;
		bit[63:0]			data_ptr;
	} fifo_t;
	
	fifo_t					sw2hvl_msg_queue;
	fifo_t					hvl2sw_msg_queue;

	function new(string name, uvm_component parent=null);
		super.new(name, parent);
	endfunction
	
	/**
	 * build_phase()
	 *
	 * Override from class uvm_component
	 */
	virtual function void build_phase(input uvm_phase phase);
		int blk_sz = 1024; // size in words
		
		if (!uvm_config_db #(int)::get(this, "", MEMBLK_SIZE_CFG, blk_sz)) begin
			blk_sz = 1024;
		end
	
		assert (uvm_config_db #(bit[63:0])::get(this, "", MEMBLK_ADDR_CFG, m_memblk)) else
			`uvm_fatal(get_name(), "No mem-block address configured");
		
		sw2hvl_msg_queue.write_ptr = (m_memblk+4);
		sw2hvl_msg_queue.read_ptr  = (m_memblk+8);
		hvl2sw_msg_queue.write_ptr = (m_memblk+12);
		hvl2sw_msg_queue.read_ptr  = (m_memblk+16);
	
		sw2hvl_msg_queue.sz = (blk_sz-5)/2; // size in words
		hvl2sw_msg_queue.sz = (blk_sz-5)/2; // size in words
		
		sw2hvl_msg_queue.data_ptr = (m_memblk+20);
		hvl2sw_msg_queue.data_ptr = (m_memblk+20+4*sw2hvl_msg_queue.sz);
	endfunction

	/**
	 * pre_main_phase()
	 *
	 * Override from class uvm_component
	 */
	virtual task pre_main_phase(input uvm_phase phase);
		bit[31:0] data;
		
		read32(m_memblk, data);
		
		// Initialize the memory block if the magic number is wrong
		if (data != 'h55aaeeff) begin
			write32(sw2hvl_msg_queue.read_ptr,  0);
			write32(sw2hvl_msg_queue.write_ptr, 0);
			write32(hvl2sw_msg_queue.read_ptr,  0);
			write32(hvl2sw_msg_queue.write_ptr, 0);
			write32(m_memblk, 'h55aaeeff);
		end
	endtask

	/**
	 * FUNCTION: recv_msg
	 *
	 * Override from class uvm_sdv_connector
	 */
	virtual task recv_msg(output uvm_sdv_connector_msg msg);
		bit[31:0] rp, wp, sz, tmp;
		bit[31:0] msg_idx = 0;
		msg = uvm_sdv_connector_msg::type_id::create();
		
		// First, wait for the key word
		read32(sw2hvl_msg_queue.read_ptr, rp);
		forever begin
			read32(sw2hvl_msg_queue.write_ptr, wp);
			sz = (wp >= rp)?(wp-rp):(sw2hvl_msg_queue.sz-(rp-wp));
			
			if (sz < 2) begin
				backoff();
			end else begin
				// Have at least two words
				break;
			end
		end
	
		read32(sw2hvl_msg_queue.data_ptr+4*rp, tmp);
		rp = ((rp + 1) % sw2hvl_msg_queue.sz);
		msg.endpoint_id = tmp[31:16];
		msg.msg_type    = tmp[15:0];

		read32(sw2hvl_msg_queue.data_ptr+4*rp, tmp);
		
		rp = ((rp + 1) % sw2hvl_msg_queue.sz);
		msg.msg_len = tmp;
	
		assert(msg.msg_len <= MAX_MSG_SZ) else
			`uvm_fatal(get_name(), 
				$psprintf("Message size %0d exceeds maximimum size %0d", msg.msg_len, MAX_MSG_SZ));
		msg.msg_data = new[msg.msg_len];

		// Write-back the read pointer so the software
		// can see it
		write32(sw2hvl_msg_queue.read_ptr, rp);

		while (msg_idx < msg.msg_len) begin
			sz = (wp >= rp)?(wp-rp):(sw2hvl_msg_queue.sz-(rp-wp));
			
			if (sz > 0) begin
				read32(sw2hvl_msg_queue.data_ptr+4*rp, tmp);
				msg.msg_data[msg_idx++] = tmp;
				rp = ((rp + 1) % sw2hvl_msg_queue.sz);
				sz--;
				// If we're about to stall, then write-back the
				// read pointer
				if (sz == 0 && msg_idx < msg.msg_len) begin
					write32(sw2hvl_msg_queue.read_ptr, rp);
				end
			end else begin
				backoff();
				read32(sw2hvl_msg_queue.write_ptr, wp);
			end
		end
		
		// Commit the final read pointer
		write32(sw2hvl_msg_queue.read_ptr, rp);
	endtask

	/**
	 * FUNCTION: send_msg
	 *
	 * Override from class uvm_sdv_driver
	 */
	virtual task send_msg(input uvm_sdv_connector_msg msg);
		bit[31:0] rp, wp, sz, tmp;
		bit[31:0] msg_idx = 0;
		
		// Wait until we can write at least two words
		read32(hvl2sw_msg_queue.write_ptr, wp);
		forever begin
			read32(hvl2sw_msg_queue.read_ptr, rp);
			sz = (wp >= rp)?(hvl2sw_msg_queue.sz-(wp-rp)):(rp-wp);
			
			if (sz < 2) begin
				backoff();
			end else begin
				// Have at least two words
				break;
			end
		end

		tmp[31:16] = msg.endpoint_id;
		tmp[15:0]  = msg.msg_type;
		write32(hvl2sw_msg_queue.data_ptr+4*wp, tmp);
		wp = ((wp + 1) % hvl2sw_msg_queue.sz);
	
		write32(hvl2sw_msg_queue.data_ptr+4*wp, msg.msg_len);
		wp = ((wp + 1) % hvl2sw_msg_queue.sz);
		
		// Write-back the write pointer so the software
		// can see it
		write32(hvl2sw_msg_queue.write_ptr, wp);

		while (msg_idx < msg.msg_len) begin
			sz = (wp >= rp)?(hvl2sw_msg_queue.sz-(wp-rp)):(rp-wp);
			
			if (sz > 0) begin
				tmp = msg.msg_data[msg_idx++];
				write32(hvl2sw_msg_queue.data_ptr+4*wp, tmp);
				wp = ((wp + 1) % hvl2sw_msg_queue.sz);
				sz--;
				// If we're about to stall, then write-back the
				// write pointer
				if (sz == 0 && msg_idx < msg.msg_len) begin
					write32(hvl2sw_msg_queue.write_ptr, wp);
				end
			end else begin
				backoff();
				read32(hvl2sw_msg_queue.read_ptr, rp);
			end
		end
		
		// Commit the final write pointer
		write32(hvl2sw_msg_queue.write_ptr, wp);
	endtask
	
	virtual task backoff();
		// Default implementation is timed wait
		#100ns;
	endtask

	virtual task read32(
		input  bit[63:0]	addr, 
		output bit[31:0]	data);
		`uvm_fatal(get_name(), "read32 task unimplemented");
	endtask
	
	virtual task write32(
		input bit[63:0]	addr, 
		input bit[31:0]	data);
		`uvm_fatal(get_name(), "write32 task unimplemented");
	endtask

endclass

