
class uvm_sdv_nonblocking_endpoint extends uvm_sdv_endpoint_base;
	`uvm_object_utils(uvm_sdv_nonblocking_endpoint)
	
	function new(string name="uvm_sdv_nonblocking_endpoint");
		super.new(name);
	endfunction
	
	virtual function void message_received(uvm_sdv_connector_msg msg);
		`uvm_fatal(get_name(), "uvm_sdv_nonblocking_endpoint::message_received not implemented");
	endfunction
	
	/**
	 * Task: m_msg_received
	 *
	 * Override from class uvm_sdv_endpoint_base
	 */
	virtual task m_msg_received(input uvm_sdv_connector_msg msg);
		message_received(msg);
	endtask
	
endclass
