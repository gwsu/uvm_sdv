
#include "stdio.h"
#include "sw_txn.h"
#include "uvm_sdv.h"
#include "uvm_sdv_dpi_transport.h"

int ap_test_main (const char *agent_path)
{
    char buf[256];
    sw_txn txn;
    uvm_sdv_analysis_port_t sw_txn_ap;
    int i;
    const char *testname = "uvm_sdv_ap_test";
    uvm_sdv_transport_t *uvm_tp;

    uvm_tp = uvm_sdv_dpi_transport_create(agent_path);
    uvm_sdv_endpoint_mgr_init(uvm_tp);

    uvm_sdv_raise_objection(testname, 1);
    UVM_INFO(testname, uvm_sformat(buf, "Begin Test %s", testname), UVM_MEDIUM);

    uvm_sdv_analysis_port_init(&sw_txn_ap, "*.m_sw_txn_publisher", &sw_txn_pack);
    for (i=0; i<5; i++) {
    	txn.A = i;
    	txn.B = (i+5);
    	(void)txn;
    	uvm_sdv_analysis_port_write(&sw_txn_ap, &txn);
    }

    uvm_sdv_drop_objection(testname, 1);

    return 0;
}


