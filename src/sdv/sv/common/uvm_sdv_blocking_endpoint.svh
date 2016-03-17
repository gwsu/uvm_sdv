
class uvm_sdv_blocking_endpoint extends uvm_sdv_endpoint_base;
	`uvm_object_utils(uvm_sdv_blocking_endpoint)
	
	mailbox #(uvm_sdv_connector_msg)	m_mb;
	
	function new(string name="uvm_sdv_blocking_endpoint");
		super.new(name);
		m_mb = new(16);
	endfunction
	
	task recv_msg(output uvm_sdv_connector_msg msg);
		m_mb.get(msg);
	endtask

	/**
	 * Task: m_msg_received
	 *
	 * Override from class uvm_sdv_endpoint_base
	 */
	virtual task m_msg_received(input uvm_sdv_connector_msg msg);
		m_mb.put(msg);
	endtask
	
endclass
	