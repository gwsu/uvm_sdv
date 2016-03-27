/****************************************************************************
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 ****************************************************************************/
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

