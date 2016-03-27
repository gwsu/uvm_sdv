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
#include "uvm_sdv_endpoint_zero.h"
#include "uvm_sdv_transport.h"
#include "uvm_sdv_endpoint_mgr_int.h"
#include "uvm_sdv_packer.h"

static void uvm_sdv_endpoint_zero_recv(void *ud, uvm_sdv_transport_msg_t *msg);
void uvm_sdv_sequence_ended(uint32_t seq_id);


void uvm_sdv_endpoint_zero_init(uvm_sdv_endpoint_zero_t *endpoint)
{
	uvm_sdv_endpoint_init(&endpoint->endpoint, "", "",
			UVM_SDV_ENDPOINT_BIDIR, &uvm_sdv_endpoint_zero_recv, endpoint);

	endpoint->msg_id = 0;
}

uint32_t uvm_sdv_endpoint_zero_alloc_msgid(uvm_sdv_endpoint_zero_t *endpoint)
{
	// TODO: mutex
	int32_t ret;

	ret = endpoint->msg_id;
	endpoint->msg_id = ((endpoint->msg_id+1) & 0x7FFFFFFF);

	return ret;
}

static void uvm_sdv_endpoint_zero_recv(void *ud, uvm_sdv_transport_msg_t *msg)
{
	uvm_sdv_endpoint_zero_t *ep_zero = (uvm_sdv_endpoint_zero_t *)ud;
	uvm_sdv_packer_t packer;

	(void)ep_zero;

	uvm_sdv_packer_init(&packer, msg);

	switch (msg->msg_type) {
		case UVM_SDV_SEQ_DONE: {
			// a sequence has completed
			uint32_t seq_id;

			seq_id = uvm_sdv_unpack_int(&packer, 32);

//			printf("Sequence %d ended\n", seq_id);

			uvm_sdv_sequence_ended(seq_id);

			// Release the message
			uvm_sdv_endpoint_mgr_free_msg(msg);
		} break;

		default: {
			// Queue the message for later
			printf("Error: unexpected endpoint-zero response message: msg_type=%d msg_id=%d\n",
					msg->msg_type, msg->msg_data[0]);
			uvm_sdv_endpoint_queue_msg(&ep_zero->endpoint, msg);

		} break;
	}
}
