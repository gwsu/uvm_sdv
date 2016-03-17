/*****************************************************************************
 * uvm_sdv_messaging.c
 *****************************************************************************/
#include "uvm_sdv_config.h"
#include "uvm_sdv_messaging.h"
#include "uvm_sdv_packer.h"
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_endpoint_mgr_int.h"

/********************************************************************
 * uvm_sformat()
 ********************************************************************/
const char *uvm_sformat(char *buf, const char *fmt, ...)
{
	va_list ap;
	int len, i;
	va_start(ap, fmt);
	len = vsprintf(buf, fmt, ap);
	// Ensure we pad out buffer with 0's
	for (i=0; i<4; i++) {
		buf[len++] = 0;
	}

	va_end(ap);

	return buf;
}

/********************************************************************
 * uvm_print()
 ********************************************************************/
void uvm_print(const char *msg)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;

	tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_MSG_PRINT;

	uvm_sdv_pack_string(&packer, msg);

	tp->send_msg(tp, tp_msg);
}

/********************************************************************
 * uvm_warning()
 ********************************************************************/
void uvm_warning(const char *name, const char *msg, const char *file, int lineno)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;

	tp_msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_MSG_WARN;

	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_string(&packer, msg);
	if (file) {
		uvm_sdv_pack_string(&packer, file);
	} else {
		uvm_sdv_pack_string(&packer, "");
	}
	uvm_sdv_pack_int(&packer, lineno, 32);

	tp->send_msg(tp, tp_msg);
}

/********************************************************************
 * uvm_error()
 ********************************************************************/
void uvm_error(const char *name, const char *msg, const char *file, int lineno)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;

	tp_msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_MSG_ERROR;

	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_string(&packer, msg);
	if (file) {
		uvm_sdv_pack_string(&packer, file);
	} else {
		uvm_sdv_pack_string(&packer, "");
	}
	uvm_sdv_pack_int(&packer, lineno, 32);

	tp->send_msg(tp, tp_msg);
}

/********************************************************************
 * uvm_fatal()
 ********************************************************************/
void uvm_fatal(const char *name, const char *msg, const char *file, int lineno)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;

	tp_msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_MSG_FATAL;

	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_string(&packer, msg);
	if (file) {
		uvm_sdv_pack_string(&packer, file);
	} else {
		uvm_sdv_pack_string(&packer, "");
	}
	uvm_sdv_pack_int(&packer, lineno, 32);

	tp->send_msg(tp, tp_msg);
}

/********************************************************************
 * uvm_info()
 ********************************************************************/
void uvm_info(const char *name, const char *msg, int verbosity, const char *file, int lineno)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	uvm_sdv_packer_t packer;

	tp_msg = tp->alloc_msg(tp, 0);
	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_MSG_INFO;

	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_string(&packer, msg);
	if (file) {
		uvm_sdv_pack_string(&packer, file);
	} else {
		uvm_sdv_pack_string(&packer, "");
	}
	uvm_sdv_pack_int(&packer, lineno, 32);
	uvm_sdv_pack_int(&packer, verbosity, 32);

	tp->send_msg(tp, tp_msg);
}
