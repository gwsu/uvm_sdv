
#include "stdio.h"
#include "sw_txn.h"
#include "uvm_sdv.h"
#include "uvm_sdv_dpi_transport.h"

int sequence_test_main (const char *agent_path)
{
    const char *testname = "uvm_sdv_sequence_test";
    int i, cnt;
    char buf[256];
    uvm_sdv_transport_t *uvm_tp;

    uvm_tp = uvm_sdv_dpi_transport_create(agent_path);
    uvm_sdv_endpoint_mgr_init(uvm_tp);

    uvm_sdv_raise_objection(testname, 1);
    UVM_INFO(testname, uvm_sformat(buf, "Begin Test %s", testname), UVM_MEDIUM);

    for (cnt=0; cnt<4; cnt++) {
    	uint32_t seq_id = uvm_sdv_sequence_start("sw_txn_seq", "*.m_sw_txn_seqr2");
    	uint32_t is_alive;

    	if (seq_id == 0) {
    		UVM_FATAL(testname, "Failed to start sequence");
    	} else {
    		UVM_INFO(testname, uvm_sformat(buf, "Started sequence %d", seq_id), UVM_MEDIUM);
    	}

    	for (i=0; i<2; i++) {
    		UVM_INFO(testname, uvm_sformat(buf, "Message %d", i), UVM_MEDIUM);
    	}

    	is_alive = uvm_sdv_sequence_is_running(seq_id);

    	if (is_alive) {
    		UVM_FATAL(testname, "Sequence is still running");
    	}
    }

    uvm_sdv_drop_objection(testname, 1);

    return 0;
}


