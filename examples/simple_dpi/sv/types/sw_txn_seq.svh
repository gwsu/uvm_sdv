
class sw_txn_seq extends uvm_sequence #(uvm_sequence_item);
	
	`uvm_object_utils(sw_txn_seq)
	
	rand sw_txn txn = sw_txn::type_id::create();
	
	/****************************************************************
	 * new()
	 ****************************************************************/
	function new(string name="sw_txn_seq");
		super.new(name);
	endfunction
	
	/****************************************************************
	 * body()
	 ****************************************************************/
	task body();
		start_item(txn);
		finish_item(txn);
	endtask

endclass



