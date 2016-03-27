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
#include "uvm_sdv_sequencer_driver.h"
#include "uvm_sdv_endpoint_mgr_int.h"
#include "uvm_sdv_endpoint_mgr.h"

void uvm_sdv_sequencer_driver_init(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		const char						*name,
		uvm_sdv_pack_func				req_pack_f,
		uvm_sdv_pack_func				rsp_pack_f)
{
	seqr_driver->req_pack_f = req_pack_f;
	seqr_driver->rsp_pack_f = rsp_pack_f;

	uvm_sdv_endpoint_init(&seqr_driver->endpoint, name, "SEQR_DRV",
			UVM_SDV_ENDPOINT_BIDIR, 0, 0);
}

void uvm_sdv_sequencer_driver_get_next_item(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item)
{
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *msg = 0;

	// Receive message
	msg = uvm_sdv_endpoint_mgr_recv_msg(tp, &seqr_driver->endpoint);

	uvm_sdv_packer_init(&packer, msg);

	seqr_driver->req_pack_f(&packer, item);

	// Free message
	tp->free_msg(tp, msg);
}

void uvm_sdv_sequencer_driver_item_done(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item)
{
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);

	msg->endpoint_id = seqr_driver->endpoint.endpoint_id;

	uvm_sdv_packer_init(&packer, msg);
	uvm_sdv_pack_object(&packer, seqr_driver->rsp_pack_f, item);

	// Send back response
	tp->send_msg(tp, msg);
}
