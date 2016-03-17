
class simple_dpi_env extends uvm_env;
	`uvm_component_utils(simple_dpi_env)
	
	uvm_sdv_dpi_connector					m_sdv_connector;
	uvm_sdv_publisher #(sw_txn)				m_sw_txn_publisher;
	uvm_sequencer #(sw_txn)					m_sw_txn_seqr;
	uvm_sdv_driver #(sw_txn)				m_sdv_driver;
	
	uvm_sequencer #(sw_txn)					m_sw_txn_seqr2;
	sw_txn_driver							m_sw_txn_driver2;
	
	
	function new(string name, uvm_component parent=null);
		super.new(name, parent);
	endfunction
	


	/**
	 * Function: build_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void build_phase(input uvm_phase phase);
		super.build_phase(phase);
		m_sdv_connector = uvm_sdv_dpi_connector::type_id::create("m_sdv_connector", this);

		m_sw_txn_publisher = uvm_sdv_publisher #(sw_txn)::type_id::create("m_sw_txn_publisher", this);
		
		m_sw_txn_seqr = new("m_sw_txn_seqr", this);
		
		m_sdv_driver = uvm_sdv_driver #(sw_txn)::type_id::create("m_sdv_driver", this);
		
		m_sw_txn_seqr2 = new("m_sw_txn_seqr2", this);
		m_sw_txn_driver2 = sw_txn_driver::type_id::create("m_sw_txn_driver2", this);

	endfunction

	/**
	 * Function: connect_phase
	 *
	 * Override from class uvm_component
	 */
	virtual function void connect_phase(input uvm_phase phase);
		super.connect_phase(phase);

		m_sdv_connector.register_endpoint(m_sw_txn_publisher.nonblocking_endpoint_imp);

		m_sdv_driver.seq_item_port.connect(m_sw_txn_seqr.seq_item_export);
	
		m_sdv_connector.register_endpoint(m_sdv_driver.m_sdv_endpoint);
		
		m_sw_txn_driver2.seq_item_port.connect(m_sw_txn_seqr2.seq_item_export);
		
	endfunction

	
	
	
endclass
