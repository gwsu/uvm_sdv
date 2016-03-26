/****************************************************************************
 * uvm_sdv_dpi_connection.svh
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_dpi_connection_svh
`define INCLUDED_uvm_sdv_dpi_connection_svh

/**
 * Class: uvm_sdv_dpi_connection_info
 * 
 * Connection-specific data on the connection between native-compiled
 * software and UVM SDV components
 */
class uvm_sdv_dpi_connection;

	mailbox #(bit[31:0])				m_uvm2sw;
	mailbox #(bit[31:0])				m_sw2uvm;
	semaphore							m_sw2uvm_sem;
	semaphore							m_uvm2sw_sem;
	
	static 	uvm_sdv_dpi_connection		m_inst_map[string];

	function new();
		m_sw2uvm_sem = new(1);
		m_uvm2sw_sem = new(1);
		
		m_uvm2sw = new(1);
		m_sw2uvm = new(1);
	endfunction
	
	
	static function uvm_sdv_dpi_connection inst(string connector_path);
		uvm_sdv_dpi_connection connection = null;
		
		if (!m_inst_map.exists(connector_path)) begin
			connection = new;
			m_inst_map[connector_path] = connection;
		end else begin
			connection = m_inst_map[connector_path];
		end
		
		return connection;
	endfunction
	
	task send_msg(uvm_sdv_connector_msg msg);
		bit[31:0] tmp;
	
		m_uvm2sw_sem.get(1);
		tmp[31:16] = msg.endpoint_id;
		tmp[15:0] = msg.msg_type;
	
		m_uvm2sw.put(tmp);
		m_uvm2sw.put(msg.msg_len);
	
		for (int i=0; i<msg.msg_len; i++) begin
			m_uvm2sw.put(msg.msg_data[i]);
		end
		
		m_uvm2sw_sem.put(1);
	endtask
	
	task recv_msg(output uvm_sdv_connector_msg msg);
		bit[31:0] tmp;
		
		m_sw2uvm_sem.get(1);
		msg = new();
		m_sw2uvm.get(tmp);
		msg.endpoint_id = tmp[31:16];
		msg.msg_type = tmp[15:0];
		
		m_sw2uvm.get(msg.msg_len);
	
		msg.msg_data = new[msg.msg_len];
		
		for (int i=0; i<msg.msg_len; i++) begin
			m_sw2uvm.get(msg.msg_data[i]);
		end
		
		m_sw2uvm_sem.put(1);
	endtask
	
	task write_sw2uvm(bit[31:0] data);
		m_sw2uvm.put(data);
	endtask
	
	task read_uvm2sw(output bit[31:0] data, output bit ok, input bit block);
		if (block) begin
			m_uvm2sw.get(data);
			ok = 1;
		end else begin
			ok = m_uvm2sw.try_get(data);
		end
	endtask

endclass

`endif /* INCLUDED_uvm_sdv_dpi_connection_info_svh */
