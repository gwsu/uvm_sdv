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
#ifndef INCLUDED_UVM_SDV_TRANSPORT_H
#define INCLUDED_UVM_SDV_TRANSPORT_H
#include "uvm_sdv_config.h"
#include "uvm_sdv_transport_msg.h"


/**
 * Struct: uvm_sdv_transport_s
 */
typedef struct uvm_sdv_transport_s {
	void									*prv_data;

	uvm_sdv_transport_msg_t *(*recv_msg)(
			struct uvm_sdv_transport_s		*tp,
			uint32_t						block);

	/**
	 * Sends or queues the message. The message
	 * data structure is transfers to the transport, and
	 * may not be subsequently changed by the caller.
	 */
	int (*send_msg)(
			struct uvm_sdv_transport_s		*tp,
			uvm_sdv_transport_msg_t			*msg);

	uvm_sdv_transport_msg_t *(*alloc_msg)(
			struct uvm_sdv_transport_s		*tp,
			uint32_t						msg_len);

	void (*free_msg)(
			struct uvm_sdv_transport_s		*tp,
			uvm_sdv_transport_msg_t			*msg);

} uvm_sdv_transport_t;


#endif /* INCLUDED_UVM_SDV_TRANSPORT_H */
