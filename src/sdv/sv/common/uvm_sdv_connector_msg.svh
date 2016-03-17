/****************************************************************************
 *
 ****************************************************************************/
class uvm_sdv_connector_msg extends uvm_object;
	`uvm_object_utils(uvm_sdv_connector_msg);

	bit[15:0]					endpoint_id;
	bit[15:0]					msg_type;
	bit[31:0]					msg_len;
	bit[31:0]					msg_data[];

	function void put(bit[31:0] data);
		if (msg_len >= msg_data.size()) begin
			bit[31:0] tmp[] = new[msg_data.size()+64];
			// Resize the message
			foreach (msg_data[i]) begin
				tmp[i] = msg_data[i];
			end
			msg_data = tmp;
		end
		msg_data[msg_len++] = data;
	endfunction

endclass

