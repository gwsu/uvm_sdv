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
#ifndef INCLUDED_UVM_SDV_TRANSPORT_MSG_H
#define INCLUDED_UVM_SDV_TRANSPORT_MSG_H

typedef struct uvm_sdv_transport_msg_s {
	uint16_t			endpoint_id;
	uint16_t			msg_type;
	uint32_t			msg_len;
	uint32_t			*msg_data;

	// Management fields used by the transport
	uint32_t							msg_data_max;
	struct uvm_sdv_transport_msg_s		*next;
} uvm_sdv_transport_msg_t;

#endif /* INCLUDED_UVM_SDV_TRANSPORT_MSG_H */
