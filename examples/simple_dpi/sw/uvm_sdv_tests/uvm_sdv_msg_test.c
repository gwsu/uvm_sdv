
#include "stdio.h"
#include "sw_txn.h"
#include "uvm_sdv.h"
#include "uvm_sdv_dpi_transport.h"

int msg_test_main (const char *agent_path)
{
    char buf[256];
    int i;
    const char *testname = "uvm_sdv_msg_test";
    uvm_sdv_transport_t *uvm_tp;

    uvm_tp = uvm_sdv_dpi_transport_create(agent_path);
    uvm_sdv_endpoint_mgr_init(uvm_tp);


    uvm_sdv_raise_objection(testname, 1);
    UVM_INFO(testname, uvm_sformat(buf, "Begin Test %s", testname), UVM_MEDIUM);

    for (i=0; i<5; i++) {
    	printf("%s:%d A=%d B=%d\n",
    			__FILE__, __LINE__, i, (i+5));
    }

    for (i=0; i<5; i++) {
    	UVM_INFO(testname, uvm_sformat(buf, "A=%d B=%d\n", i, (i+5)), UVM_MEDIUM);
    }

    uvm_sdv_drop_objection(testname, 1);

    return 0;
}


