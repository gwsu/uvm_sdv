/*****************************************************************************
 * uvm_sdv_messaging.h
 *****************************************************************************/
#ifndef INCLUDED_UVM_SDV_MESSAGING_H
#define INCLUDED_UVM_SDV_MESSAGING_H

#ifdef __cplusplus
extern "C" {
#endif

#define UVM_INFO(name, msg, level) \
	uvm_info(name, msg, level, __FILE__, __LINE__)

#define UVM_WARNING(name, msg) \
	uvm_warning(name, msg, __FILE__, __LINE__)

#define UVM_ERROR(name, msg) \
	uvm_error(name, msg, __FILE__, __LINE__)

#define UVM_FATAL(name, msg) \
	uvm_fatal(name, msg, __FILE__, __LINE__)

typedef enum {
	UVM_NONE = 0,
	UVM_LOW = 100,
	UVM_MEDIUM = 200,
	UVM_HIGH = 300,
	UVM_FULL = 400,
	UVM_DEBUG = 500
} uvm_verbosity;

/**
 * Formats a message using a reused buffer
 */
const char *uvm_sformat(char *buf, const char *fmt, ...);

void uvm_print(const char *msg);
void uvm_info(const char *name, const char *msg, int verbosity, const char *file, int lineno);
void uvm_warning(const char *name, const char *msg, const char *file, int lineno);
void uvm_error(const char *name, const char *msg, const char *file, int lineno);
void uvm_fatal(const char *name, const char *msg, const char *file, int lineno);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDED_UVM_SDV_MESSAGING_H */

