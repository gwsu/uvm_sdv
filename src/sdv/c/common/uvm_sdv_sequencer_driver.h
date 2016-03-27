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
#ifndef INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H
#define INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H
#include "uvm_sdv_packer.h"
#include "uvm_sdv_endpoint_mgr.h"

typedef struct uvm_sdv_sequencer_driver_s {
	uvm_sdv_endpoint_t			endpoint;
	uvm_sdv_pack_func			req_pack_f;
	uvm_sdv_pack_func			rsp_pack_f;
} uvm_sdv_sequencer_driver_t;

void uvm_sdv_sequencer_driver_init(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		const char						*name,
		uvm_sdv_pack_func				req_pack_f,
		uvm_sdv_pack_func				rsp_pack_f);

void uvm_sdv_sequencer_driver_get_next_item(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item);

void uvm_sdv_sequencer_driver_item_done(
		uvm_sdv_sequencer_driver_t		*seqr_driver,
		void							*item);

#endif /* INCLUDED_UVM_SDV_SEQUENCER_DRIVER_H */
