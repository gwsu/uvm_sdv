/****************************************************************************
 * simple_dpi_tb.sv
 ****************************************************************************/
`ifndef INCLUDED_simple_dpi_tb_sv
`define INCLUDED_simple_dpi_tb_sv

/**
 * Module: simple_dpi_tb
 * 
 * TODO: Add module documentation
 */
`include "uvm_macros.svh"
module simple_dpi_tb;
	import uvm_pkg::*;
	import simple_dpi_tests_pkg::*;
	
	initial begin
		run_test();
	end

endmodule

`endif /* INCLUDED_simple_dpi_tb_sv */
