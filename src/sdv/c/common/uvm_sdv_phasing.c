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

#include "uvm_sdv_phasing.h"
#include "uvm_sdv_packer.h"
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_endpoint_mgr_int.h"


void uvm_sdv_raise_objection(const char *name, int count)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_msg_t *tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_RAISE_OBJECTION;
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, count, 32);

	tp->send_msg(tp, tp_msg);
}

void uvm_sdv_drop_objection(const char *name, int count)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_msg_t *tp_msg = tp->alloc_msg(tp, 0);

	uvm_sdv_packer_init(&packer, tp_msg);

	tp_msg->endpoint_id = 0;
	tp_msg->msg_type = UVM_SDV_DROP_OBJECTION;
	uvm_sdv_pack_string(&packer, name);
	uvm_sdv_pack_int(&packer, count, 32);

	tp->send_msg(tp, tp_msg);
}
