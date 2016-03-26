
/**
 * Class: uvm_sdv_connector
 * 
 * Implements the link between the UVM environment and the 
 * testbench environment running on the embedded processor
 */
class uvm_sdv_connector extends uvm_component;
	
	`uvm_component_utils(uvm_sdv_connector)
	
	typedef enum {
		UVM_SDV_MSG_PRINT = 0, 		// SW to UVM
		UVM_SDV_MSG_INFO,			// SW to UVM
		UVM_SDV_MSG_WARN,			// SW to UVM
		UVM_SDV_MSG_ERROR,			// SW to UVM
		UVM_SDV_MSG_FATAL,			// SW to UVM
		UVM_SDV_RAISE_OBJECTION,	// SW to UVM
		UVM_SDV_DROP_OBJECTION,		// SW to UVM
		UVM_SDV_REGISTER_ENDPOINT,	// SW to UVM
		UVM_SDV_START_SEQ,			// SW to UVM
		UVM_SDV_START_SEQ_RSP,		// UVM to SW
		UVM_SDV_SEQ_DONE, 			// UVM to SW
		UVM_SDV_GET_CONFIG,			// SW to UVM
		UVM_SDV_GET_CONFIG_RSP		// UVM to SW
	} uvm_sdv_msgs;
	
	typedef enum {
		UVM_SDV_CFG_TYPE_INT,
		UVM_SDV_CFG_TYPE_STR,
		UVM_SDV_CFG_TYPE_OBJ
	} uvm_sdv_cfg_type_e;
	
	uvm_phase							m_current_phase;
	bit									m_connected;
	
	uvm_sdv_endpoint_base				m_endpoints[$];
	semaphore							m_send_sem;
	
	// Endpoints registered by the software and unconnected
	uvm_sdv_endpoint_base				m_unconnected_endpoints[$];
	
	function new(string name, uvm_component parent=null);
		super.new(name, parent);
		m_send_sem = new(1);
	endfunction

	function void register_endpoint(uvm_sdv_endpoint_base endpoint);
		uvm_sdv_endpoint_base unconnected_endpoint = null;
		
		`uvm_info("UVM_SDV_CONNECTOR", 
			$psprintf("register_endpoint: %0s", endpoint.get_name()), 
			UVM_HIGH);
	
		// Search the unconnected list
		foreach (m_unconnected_endpoints[i]) begin
			if (uvm_is_match(endpoint.get_name(), m_unconnected_endpoints[i].get_name())) begin
				unconnected_endpoint = m_unconnected_endpoints[i];
				m_unconnected_endpoints.delete(i);
				break;
			end
		end
		
		endpoint.set_connector(this);
		if (unconnected_endpoint != null) begin
			// Found an unconnected endpoint. Let's connect
			endpoint.m_endpoint_id = unconnected_endpoint.m_endpoint_id;
			m_endpoints.push_back(endpoint);
			endpoint.connect();
		end else begin
			// Nothing pending, so we're unconnected
			endpoint.m_endpoint_id = -1;
			m_unconnected_endpoints.push_back(endpoint);
		end
	endfunction
	
	/**
	 * Task: m_queue_msg
	 * Queue a message to be sent to the software
	 */
	task m_queue_msg(uvm_sdv_endpoint_base endpoint, uvm_sdv_connector_msg msg);
		m_send_sem.get(1);
		msg.endpoint_id = endpoint.m_endpoint_id;
		send_msg(msg);
		m_send_sem.put(1);
	endtask
	
	task m_queue_msg_0(uvm_sdv_connector_msg msg);
		m_send_sem.get(1);
		msg.endpoint_id = 0;
		send_msg(msg);
		m_send_sem.put(1);
	endtask
	
	task m_sequence_done(bit[31:0] seq_id);
		uvm_sdv_connector_msg msg = new;
		uvm_sdv_packer packer = new;
		
		packer.init(msg);
		msg.msg_type = UVM_SDV_SEQ_DONE;
		
		packer.pack_field_int(seq_id, 32);
		
		m_queue_msg_0(msg);
	endtask
	
	virtual task recv_msg(output uvm_sdv_connector_msg msg);
		`uvm_fatal(get_name(), "recv_msg task not implemented");
	endtask

	virtual task send_msg(uvm_sdv_connector_msg msg);
		`uvm_fatal(get_name(), "send_msg task not implemented");
	endtask


	/**
	 * run_phase()
	 *
	 * Override from class uvm_component
	 */
	virtual task run_phase(input uvm_phase phase);
		m_current_phase = phase;
		
		phase.raise_objection(this, "Wait Processor Connection");
		
		recv_thread();
	endtask

	/**
	 * Function: recv_thread
	 * 
	 * Manages receiving messages from the embedded software
	 * and propagating them to the UVM environment
	 */
	virtual task recv_thread();
		uvm_sdv_connector_msg		msg;
		
		forever begin
			recv_msg(msg);
			
			if (!m_connected) begin
				m_connected = 1;
				m_current_phase.drop_objection(this, "Wait Processor Connection");
			end
			
			if (msg.endpoint_id == 0) begin
				// endpoint0 messages are handled by the driver
				handle_msg(msg);
			end else begin
				uvm_sdv_endpoint_base endpoint = null;
				
				foreach (m_endpoints[i]) begin
					if (m_endpoints[i].m_endpoint_id == msg.endpoint_id) begin
						endpoint = m_endpoints[i];
						break;
					end
				end
				
				assert(endpoint != null) else
					`uvm_fatal(get_name(), 
						$psprintf("No registered endpoint with id %0d", msg.endpoint_id));

				if (endpoint != null) begin
					endpoint.m_msg_received(msg);
				end
			end
		end
	endtask
	
	virtual task handle_msg(uvm_sdv_connector_msg msg);
		uvm_sdv_packer packer = new();
		packer.init(msg);

		
		case (msg.msg_type)
			UVM_SDV_MSG_PRINT: begin
				string str = packer.unpack_string();
				$write(str);
			end
			
			UVM_SDV_MSG_INFO, UVM_SDV_MSG_WARN, 
			UVM_SDV_MSG_ERROR, UVM_SDV_MSG_FATAL: begin
				handle_uvm_sdv_msg(uvm_sdv_msgs'(msg.msg_type), packer);
			end
			
			UVM_SDV_RAISE_OBJECTION: begin
				string name = packer.unpack_string();
				int count = packer.unpack_field_int(32);
				m_current_phase.raise_objection(this, name, count);
			end
			
			UVM_SDV_DROP_OBJECTION: begin
				string name = packer.unpack_string();
				int count = packer.unpack_field_int(32);
				m_current_phase.drop_objection(this, name, count);
			end
			
			UVM_SDV_REGISTER_ENDPOINT: begin
				int endpoint_id = packer.unpack_field_int(32);
				string  inst_name = packer.unpack_string();
				string  type_name = packer.unpack_string();
				int dir = packer.unpack_field_int(32);
				uvm_sdv_endpoint_base endpoint = null;
				
				foreach (m_unconnected_endpoints[i]) begin
					`uvm_info("UVM_SDV_CONNECTOR", 
						$psprintf("Check %0s against %0s", inst_name, m_unconnected_endpoints[i].get_name()),
						UVM_HIGH);
					// See if there's already an endpoint waiting
					if (uvm_is_match(inst_name, m_unconnected_endpoints[i].get_name())) begin
						endpoint = m_unconnected_endpoints[i];
						m_unconnected_endpoints.delete(i);
						break;
					end
				end
				
				$display("REGISTER_ENDPOINT: %0d %0s %0s %0d",
					endpoint_id, inst_name, type_name, dir);
				
				if (endpoint != null) begin
					endpoint.m_endpoint_id = endpoint_id;
					m_endpoints.push_back(endpoint);
					endpoint.connect();
				end else begin
					endpoint = new(inst_name);
					endpoint.m_endpoint_id = endpoint_id;
					m_unconnected_endpoints.push_back(endpoint);
				end
			end
			
			UVM_SDV_START_SEQ: begin
				uvm_sdv_connector_msg msg_rsp = new;
				uvm_sdv_packer packer_rsp = new;
				bit[31:0] msg_id, seq_id, status=1;
				string seq_type, seqr_path;
				uvm_component comps[$];
				uvm_root root = uvm_root::get();
				
				msg_id = packer.unpack_field_int(32);
				seq_id = packer.unpack_field_int(32);
				seq_type = packer.unpack_string();
				seqr_path = packer.unpack_string();
			
				// See if we can find the sequencer
				root.find_all(seqr_path, comps);
				
				if (comps.size() == 0) begin
					`uvm_fatal(get_name(), $psprintf("Failed to find a sequencer matching spec %0s", seqr_path));
					status = 0;
				end else if (comps.size() > 1) begin
					`uvm_fatal(get_name(), $psprintf("Found %0d components matching seqr spec %0s", 
							comps.size(), seqr_path));
					status = 0;
				end else begin
					// Just one component
					uvm_sequencer_base seqr;
					if (!$cast(seqr, comps[0])) begin
						`uvm_fatal(get_name, $psprintf("Component %0s is not of type sequencer", comps[0].get_full_name()));
						status = 0;
					end else begin
						// Create the sequence
						uvm_object seq_o = create_object(seq_type, seq_type);
						uvm_sequence_base seq;
						
						if (seq_o == null) begin
							`uvm_fatal(get_name(), $psprintf("Failed to create an object of type %0s", seq_type));
							status = 0;
						end else if (!$cast(seq, seq_o)) begin
							`uvm_fatal(get_name(), $psprintf("Object of type %0s is not a sequence", seq_type));
							status = 0;
						end else begin
							// Launch the sequence
							uvm_sdv_sequence_runner runner = new(this, seq_id, seqr, seq);
// TODO:							$display("Launching sequence");
							fork
								runner.run();
							join_none
							#0;
						end
					end
				end
			
				packer_rsp.init(msg_rsp);
				
				packer_rsp.pack_field_int(msg_id, 32);
				packer_rsp.pack_field_int(status, 32);
	
				msg_rsp.msg_type = UVM_SDV_START_SEQ_RSP;

				// Send the response
				m_queue_msg_0(msg_rsp);
			end
			
			UVM_SDV_GET_CONFIG: begin
				int msg_id      = packer.unpack_field_int(32); 
				string cfg_name = packer.unpack_string();
				uvm_sdv_cfg_type_e cfg_type = uvm_sdv_cfg_type_e'(packer.unpack_field_int(32));
				uvm_sdv_connector_msg msg_rsp = new;
				uvm_sdv_packer packer_rsp = new;
						
				packer_rsp.init(msg_rsp);
				msg_rsp.msg_type = UVM_SDV_GET_CONFIG_RSP;
				packer_rsp.pack_field_int(msg_id, 32);
				
				case (cfg_type) 
					UVM_SDV_CFG_TYPE_INT: begin
						uvm_sdv_config_type_int val = 0;
						bit ex = uvm_config_db #(uvm_sdv_config_type_int)::get(this, "", cfg_name, val);
					
						packer_rsp.pack_field_int(ex, 32);
						packer_rsp.pack_field_int(val, 64);
					end
					
					UVM_SDV_CFG_TYPE_STR: begin
					end
					
					UVM_SDV_CFG_TYPE_OBJ: begin
						uvm_object val;
						int unsigned tmp[];
						bit ex = uvm_config_db #(uvm_sdv_config_type_object)::get(this, "", cfg_name, val);
					
						packer_rsp.pack_field_int(ex, 32);
						if (ex) begin
							void'(val.pack_ints(tmp, packer_rsp));
						end
					end
					
					default: begin
						`uvm_fatal(get_name(), $psprintf("Unsupported cfg-item type %0d", cfg_type));
					end
				endcase

				m_queue_msg_0(msg_rsp);
			end
				
			
			default: begin
				`uvm_fatal(get_name(), 
					$psprintf("Unknown UVM SDV message: %0d", msg.msg_type));
			end
		endcase
	endtask
	
	virtual function void handle_uvm_sdv_msg(uvm_sdv_msgs msg_type, uvm_packer msg);
		int lineno;
		string name     = msg.unpack_string();
		string msg_str  = msg.unpack_string();
		string file_str = msg.unpack_string();
	
		lineno = msg.unpack_field_int(32);
		
		case (msg_type)
			UVM_SDV_MSG_INFO: begin
				bit[31:0] verbosity = msg.unpack_field_int(32);
				if (uvm_report_enabled(verbosity, UVM_INFO, name)) begin
					uvm_report_info(name, msg_str, verbosity, file_str, lineno);
				end
			end
			
			UVM_SDV_MSG_WARN: begin
				uvm_report_warning(name, msg_str, UVM_NONE, file_str, lineno);
			end
			
			UVM_SDV_MSG_ERROR: begin
				uvm_report_error(name, msg_str, UVM_NONE, file_str, lineno);
			end
			
			UVM_SDV_MSG_FATAL: begin
				uvm_report_fatal(name, msg_str, UVM_NONE, file_str, lineno);
			end
		endcase
	endfunction
	
endclass
