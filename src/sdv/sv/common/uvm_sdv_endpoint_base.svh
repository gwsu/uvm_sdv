
typedef class uvm_sdv_connector;

class uvm_sdv_endpoint_base extends uvm_object;
	`uvm_object_utils(uvm_sdv_endpoint_base)
	
	string					m_type_name;
	int						m_endpoint_id = -1;
	bit						m_connected = 0;
	semaphore				m_connected_sem;
	
	uvm_sdv_connector		m_connector;
	
	function new(string name="uvm_sdv_endpoint_base");
		super.new(name);
		m_type_name = "GENERIC";
		
		m_connected_sem = new(0);
	endfunction
	
	function void set_endpoint_type(string tname);
		m_type_name = tname; 
	endfunction
	
	function void set_connector(uvm_sdv_connector connector);
		m_connector = connector;
	endfunction
	
	function void connect();
		m_connected = 1;
		m_connected_sem.put(1);
	endfunction
	
	function void set_endpoint_id(bit [31:0] endpoint_id);
		m_endpoint_id = endpoint_id;
	endfunction
	
	function string get_endpoint_type();
		return m_type_name;
	endfunction
	
	function string get_endpoint_instname();
		return get_name();
	endfunction
	
	task send_msg(uvm_sdv_connector_msg msg);
		if (!m_connected) begin
			m_connected_sem.get(1);
		end
	
		// Don't know the endpoint_id until connected
		msg.endpoint_id = m_endpoint_id;

		m_connector.m_queue_msg(this, msg);
	endtask
	
	virtual task m_msg_received(uvm_sdv_connector_msg msg);
		`uvm_fatal(get_name(), 
			$psprintf("uvm_sdv_endpoint::m_msg_received not implemented %p", this));
	endtask

endclass
