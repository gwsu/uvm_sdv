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
#include "uvm_sdv_analysis_port.h"
#include "uvm_sdv_endpoint_mgr_int.h"

void uvm_sdv_analysis_port_init(
		uvm_sdv_analysis_port_t			*ap,
		const char						*name,
		uvm_sdv_pack_func				pack_f)
{
	ap->pack_func = pack_f;
	uvm_sdv_endpoint_init(&ap->endpoint, name, "AP",
			UVM_SDV_ENDPOINT_SW2UVM, 0, 0);
}

/**
 * Writes a transaction to an analysis port within the
 * UVM environment
 */
void uvm_sdv_analysis_port_write(
		uvm_sdv_analysis_port_t			*ap,
		void							*obj)
{
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, msg);

	msg->endpoint_id = ap->endpoint.endpoint_id;
	// TODO: message type (?)
	msg->msg_type = 0;
	ap->pack_func(&packer, obj);

	tp->send_msg(tp, msg);
}
