/****************************************************************************
 * uvm_sdv_nonblocking_endpoint_imp.svh
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_nonblocking_endpoint_imp_svh
`define INCLUDED_uvm_sdv_nonblocking_endpoint_imp_svh

/**
 * Class: uvm_sdv_nonblocking_endpoint_imp
 * 
 * TODO: Add class documentation
 */
class uvm_sdv_nonblocking_endpoint_imp #(type T=int) extends uvm_sdv_nonblocking_endpoint;
	
	typedef uvm_sdv_nonblocking_endpoint_imp #(T) this_type;
	
	T							m_parent;

	function new(string inst_name, T parent=null);
		super.new(inst_name);
		m_parent = parent;
		assert($cast(m_parent, parent)) else
			`uvm_fatal(get_name(), $psprintf("Parent %p is not an instance of target class", parent));
	endfunction

	/**
	 * Function: message_received
	 *
	 * Override from class uvm_sdv_nonblocking_endpoint
	 */
	virtual function void message_received(input uvm_sdv_connector_msg msg);
		m_parent.message_received(msg);
	endfunction
	
endclass

`endif /* INCLUDED_uvm_sdv_nonblocking_endpoint_imp_svh */
