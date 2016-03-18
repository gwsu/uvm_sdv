

`include "uvm_macros.svh"
package simple_dpi_tests_pkg;
	import uvm_pkg::*;
	import simple_dpi_env_pkg::*;
	import uvm_sdv_pkg::*;
	import types_pkg::*;
	
    import "DPI-C" context task test_main(string connector_path);
    
	`include "simple_dpi_test_base.svh"
	`include "simple_dpi_config_test.svh"
	`include "simple_dpi_ap_test.svh"
	`include "simple_dpi_msg_test.svh"
	
endpackage
