

`include "uvm_macros.svh"
package simple_dpi_tests_pkg;
	import uvm_pkg::*;
	import simple_dpi_env_pkg::*;
	import uvm_sdv_pkg::*;
	import types_pkg::*;
	
    import "DPI-C" context task test_main(string connector_path);
//    
//    string					error_q[$];
//    
//    function void report_error_c(string msg);
//    	$display("Error: %0s", msg);
//    	error_q.push_back(msg);
//    endfunction
//    
//    export "DPI-C" function report_error_c;
    
	`include "simple_dpi_test_base.svh"
	`include "simple_dpi_config_test.svh"
	`include "simple_dpi_ap_test.svh"
	`include "simple_dpi_msg_test.svh"
	`include "simple_dpi_seq_driver_test.svh"
    `include "simple_dpi_sequence_test.svh"
	
endpackage
