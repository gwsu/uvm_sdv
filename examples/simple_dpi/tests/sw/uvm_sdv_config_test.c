
#include "stdio.h"
#include "sw_txn.h"
#include "uvm_sdv.h"
#include "uvm_sdv_dpi_transport.h"
#include "sw_test.h"


DLL_EXPORT int test_main(const char *agent_path)
{
    char buf[256];
    uint32_t has;
    uint64_t cfg_val;
    const char *testname = "uvm_sdv_config_test";
    sw_txn txn;
    uvm_sdv_transport_t *uvm_tp;

    uvm_tp = uvm_sdv_dpi_transport_create(agent_path);
    uvm_sdv_endpoint_mgr_init(uvm_tp);

    uvm_sdv_raise_objection(testname, 1);
    UVM_INFO(testname, uvm_sformat(buf, "Begin Test %s", testname), UVM_MEDIUM);

    has = uvm_sdv_config_db_get_int("TEST_CONFIG", &cfg_val);

    if (!has) {
    	UVM_FATAL(testname, "config \"TEST_CONFIG\" not found");
    }

    UVM_INFO(testname, uvm_sformat(buf, "cfg_val=0x%08x", cfg_val), UVM_MEDIUM);

    if (cfg_val != 0x55aabbcc) {
    	UVM_FATAL(testname, uvm_sformat(buf, "config \"TEST_CONFIG\" != 0x%08x (0x%08x)",
    			0x55aabbcc, cfg_val));
    }

    has = uvm_sdv_config_db_get_object(
    		"TEST_CONFIG_TXN", &sw_txn_unpack, &txn);

    if (!has) {
    	UVM_FATAL(testname, "config \"TEST_CONFIG_TXN\" not found");
    }

    UVM_INFO(testname, uvm_sformat(buf, "txn.A=%d txn.B=%d", txn.A, txn.B), UVM_MEDIUM);

    if (txn.A != 5 || txn.B != 27) {
    	UVM_FATAL(testname, uvm_sformat(buf, "config \"TEST_CONFIG_TXN\" has incorrect values"));
    }

    UVM_INFO(testname, "PASS: uvm_sdv_config_test", UVM_LOW);

    uvm_sdv_drop_objection(testname, 1);

    return 0;
}


