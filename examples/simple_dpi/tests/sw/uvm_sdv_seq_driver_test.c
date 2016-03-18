
#include "stdio.h"
#include "sw_txn.h"
#include "uvm_sdv.h"
#include "uvm_sdv_dpi_transport.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

DLL_EXPORT int seq_driver_test_main (const char *agent_path)
{
    sw_txn txn;
    uvm_sdv_sequencer_driver_t sw_txn_seqr;
    const char *testname = "uvm_sdv_seq_driver_test";
    int i;
    char buf[256];
    uvm_sdv_transport_t *uvm_tp;

    uvm_tp = uvm_sdv_dpi_transport_create(agent_path);
    uvm_sdv_endpoint_mgr_init(uvm_tp);

    uvm_sdv_raise_objection(testname, 1);
    UVM_INFO(testname, uvm_sformat(buf, "Begin Test %s", testname), UVM_MEDIUM);

    uvm_sdv_sequencer_driver_init(&sw_txn_seqr, "*.m_sdv_driver", &sw_txn_unpack, 0);
    for (i=0; i<5; i++) {
    	uvm_sdv_sequencer_driver_get_next_item(&sw_txn_seqr, &txn);
    	UVM_INFO(testname, uvm_sformat(buf, "SEQ_ITEM: A=%d B=%d", txn.A, txn.B), UVM_MEDIUM);
    	if (txn.A+5 != txn.B) {
    		UVM_ERROR(testname, uvm_sformat(buf, "Expect A+5 == B; A=%d B=%d", txn.A, txn.B));
    	}
    	uvm_sdv_sequencer_driver_item_done(&sw_txn_seqr, 0);
    }

    uvm_sdv_drop_objection(testname, 1);

    return 0;
}


