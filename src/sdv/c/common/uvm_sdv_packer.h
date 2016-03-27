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
#ifndef INCLUDED_UVM_SDV_PACKER_H
#define INCLUDED_UVM_SDV_PACKER_H
#include "uvm_sdv_config.h"
#include "uvm_sdv_transport_msg.h"

typedef struct uvm_sdv_packer_s {
	uvm_sdv_transport_msg_t			*msg;
	uint32_t						idx;
} uvm_sdv_packer_t;

typedef void (*uvm_sdv_pack_func)(
		uvm_sdv_packer_t		*packer,
		void					*obj);

void uvm_sdv_packer_init(
		uvm_sdv_packer_t			*packer,
		uvm_sdv_transport_msg_t		*msg);

void uvm_sdv_ensure_space(
		uvm_sdv_transport_msg_t		*msg,
		uint32_t					sz);

#define uvm_sdv_pack_int(packer, val, bits) \
{ \
	uint32_t sz = (bits>32)?2:1; \
	uvm_sdv_transport_msg_t *msg = (packer)->msg; \
	uint64_t val_t = (uint64_t)(val); \
	if (((msg->msg_data_max)-(msg->msg_len)) < sz) { \
			uvm_sdv_ensure_space(msg, sz); \
	} \
	if (sz == 1) { \
		msg->msg_data[msg->msg_len++] = val_t; \
	} else { \
		msg->msg_data[msg->msg_len++] = val_t; \
		val_t >>= 32; \
		msg->msg_data[msg->msg_len++] = val_t; \
	} \
}

#ifdef UNDEFINED
void uvm_sdv_pack_int(
		uvm_sdv_packer_t 			*packer,
		uint64_t					val,
		uint32_t					bits);
#endif

void uvm_sdv_pack_object(
		uvm_sdv_packer_t			*packer,
		uvm_sdv_pack_func			pack_func,
		void						*obj);

void uvm_sdv_pack_string(
		uvm_sdv_packer_t 			*packer,
		const char					*str);

uint64_t uvm_sdv_unpack_int(
		uvm_sdv_packer_t 			*packer,
		uint32_t					bits);

int uvm_sdv_unpack_isnull(
		uvm_sdv_packer_t 			*packer);

uint32_t uvm_sdv_unpack_string(
		uvm_sdv_packer_t 			*packer,
		char						*str,
		uint32_t					len);

#endif /* INCLUDED_UVM_SDV_PACKER_H */
