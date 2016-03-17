

class sw_txn extends uvm_sequence_item;
	`uvm_object_utils(sw_txn)
	
	rand bit[31:0]					A;
	rand bit[31:0]					B;
	
	constraint c {
		A inside {[1:10]};
		B inside {[1:10]};
	}

	/**
	 * Function: do_pack
	 *
	 * Override from class uvm_object
	 */
	virtual function void do_pack(input uvm_packer packer);
		packer.pack_field_int(A, 32);
		packer.pack_field_int(B, 32);
	endfunction

	/**
	 * Function: do_unpack
	 *
	 * Override from class uvm_object
	 */
	virtual function void do_unpack(input uvm_packer packer);
		A = packer.unpack_field_int(32);
		B = packer.unpack_field_int(32);
	endfunction
	
endclass

