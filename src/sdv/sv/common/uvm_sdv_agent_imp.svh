/****************************************************************************
 * uvm_sdv_agent_imp.svh
 ****************************************************************************/
`ifndef INCLUDED_uvm_sdv_agent_imp_svh
`define INCLUDED_uvm_sdv_agent_imp_svh

/**
 * Class: uvm_sdv_agent_imp
 * 
 * TODO: Add class documentation
 */
class uvm_sdv_agent_imp extends uvm_port_base #(IF);

	function new(string name, uvm_component parent, uvm_port_type_e port_type, int min_size, int max_size);
		super.new(name, parent, port_type, min_size, max_size);

	endfunction


endclass

`endif /* INCLUDED_uvm_sdv_agent_imp_svh */
