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
#include "uvm_sdv_endpoint_mgr.h"
#include "uvm_sdv_endpoint_mgr_int.h"
#include "uvm_sdv_packer.h"

static uvm_sdv_endpoint_mgr_t			*prvEndpointMgr;

/********************************************************************
 * uvm_sdv_endpoint_mgr_init()
 ********************************************************************/
void uvm_sdv_endpoint_mgr_init(uvm_sdv_transport_t *tp)
{
	prvEndpointMgr = (uvm_sdv_endpoint_mgr_t *)malloc(sizeof(uvm_sdv_endpoint_mgr_t));
	memset(prvEndpointMgr, 0, sizeof(uvm_sdv_endpoint_mgr_t));

	prvEndpointMgr->transport = tp;

	// Create the 0th endpoint
	uvm_sdv_endpoint_zero_init(&prvEndpointMgr->endpoint_zero);
}

/********************************************************************
 * uvm_sdv_endpoint_mgr_poll()
 ********************************************************************/
uint32_t uvm_sdv_endpoint_mgr_poll()
{
	uvm_sdv_endpoint_mgr_t *endpoint_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *msg_t = tp->recv_msg(tp, 0);


	if (msg_t) {
		uvm_sdv_endpoint_t *target_endpoint = endpoint_mgr->endpoint_list[msg_t->endpoint_id];
		uvm_sdv_transport_msg_t *n;

		if (target_endpoint->recv_func) {
			// Notify the endpoint
			target_endpoint->recv_func(target_endpoint->recv_func_ud, msg_t);
		} else {
			// Queue on endpoint handle
			if (target_endpoint->recv_msg_queue) {
				// Find the end of the message queue
				n = target_endpoint->recv_msg_queue;
				while (n->next) {
					n = n->next;
				}
				n->next = msg_t;
				msg_t->next = 0;
			} else {
				// No messages waiting
				target_endpoint->recv_msg_queue = msg_t;
				msg_t->next = 0;
			}
			// TODO: wake up target endpoint (?)
		}
	}

	return (msg_t != 0);
}

/********************************************************************
 * uvm_sdv_endpoint_mgr_get_transport()
 ********************************************************************/
uvm_sdv_transport_t *uvm_sdv_endpoint_mgr_get_transport(void)
{
	return prvEndpointMgr->transport;
}

/********************************************************************
 * uvm_sdv_endpoint_mgr_get()
 ********************************************************************/
uvm_sdv_endpoint_mgr_t *uvm_sdv_endpoint_mgr_get(void)
{
	return prvEndpointMgr;
}

/********************************************************************
 * uvm_sdv_endpoint_init()
 ********************************************************************/
void uvm_sdv_endpoint_init(
		uvm_sdv_endpoint_t				*endpoint,
		const char						*inst_name,
		const char						*type_name,
		uvm_sdv_endpoint_dir_e			dir,
		uvm_sdv_endpoint_recv_func		recv_func,
		void							*recv_func_ud)
{
	uvm_sdv_endpoint_mgr_t *endpoint_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_packer_t packer;
	uvm_sdv_transport_t *transport = uvm_sdv_endpoint_mgr_get_transport();
	uvm_sdv_transport_msg_t *tp_msg;
	int32_t endpoint_id = -1;
	uint32_t i;

	endpoint->inst_name = strdup(inst_name);
	endpoint->type_name = strdup(type_name);
	endpoint->endpoint_dir = dir;
	endpoint->recv_func    = recv_func;
	endpoint->recv_func_ud = recv_func_ud;
	endpoint->recv_msg_queue = 0;

	// Register the endpoint

	// Allocate an endpoint id
	for (i=0; i<endpoint_mgr->endpoint_list_len; i++) {
		if (!endpoint_mgr->endpoint_list[i]) {
			// Found an unused location
			endpoint_id = i;
			break;
		}
	}

	if (endpoint_id != -1) {
		endpoint->endpoint_id = endpoint_id;
		endpoint_mgr->endpoint_list[endpoint_id] = endpoint;
	} else if (endpoint_mgr->endpoint_list_len < endpoint_mgr->endpoint_list_max) {
		endpoint->endpoint_id = endpoint_mgr->endpoint_list_len;
		endpoint_mgr->endpoint_list[endpoint_mgr->endpoint_list_len] = endpoint;
		endpoint_mgr->endpoint_list_len++;
	} else {
		// Need to allocate more space
		uvm_sdv_endpoint_t **tmp = endpoint_mgr->endpoint_list;
		endpoint_mgr->endpoint_list = (uvm_sdv_endpoint_t **)malloc(
				sizeof(uvm_sdv_endpoint_t *)*(endpoint_mgr->endpoint_list_max+16));
		if (tmp) {
			memcpy(endpoint_mgr->endpoint_list, tmp,
					sizeof(uvm_sdv_endpoint_t *)*endpoint_mgr->endpoint_list_len);
			free(tmp);
		}
		endpoint_mgr->endpoint_list_max += 16;

		endpoint->endpoint_id = endpoint_mgr->endpoint_list_len;
		endpoint_mgr->endpoint_list[endpoint_mgr->endpoint_list_len] = endpoint;
		endpoint_mgr->endpoint_list_len++;
	}

	// Now, send the registration message if this is
	// not the 0th endpoint
	if (endpoint->endpoint_id != 0) {
		tp_msg = transport->alloc_msg(transport, 0);
		uvm_sdv_packer_init(&packer, tp_msg);

		tp_msg->endpoint_id = 0;
		tp_msg->msg_type = UVM_SDV_REGISTER_ENDPOINT;

		uvm_sdv_pack_int(&packer, endpoint->endpoint_id, 32);
		uvm_sdv_pack_string(&packer, endpoint->inst_name);
		uvm_sdv_pack_string(&packer, endpoint->type_name);
		uvm_sdv_pack_int(&packer, endpoint->endpoint_dir, 32);

		transport->send_msg(transport, tp_msg);
	}
}

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_mgr_recv_msg(
		uvm_sdv_transport_t 		*tp,
		uvm_sdv_endpoint_t 			*endpoint)
{
	uvm_sdv_endpoint_mgr_t *endpoint_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_msg_t *msg = 0;

	if (endpoint->recv_msg_queue) {
		// Already have a message
		msg = endpoint->recv_msg_queue;
		endpoint->recv_msg_queue = msg->next;
	} else {
		// Wait for a message to be received
		while (1) {
			uvm_sdv_transport_msg_t *msg_t;

			msg_t = tp->recv_msg(tp, 1);

			if (msg_t->endpoint_id == endpoint->endpoint_id) {
				// matches
				msg = msg_t;
				break;
			} else {
				uvm_sdv_endpoint_t *target_endpoint = endpoint_mgr->endpoint_list[msg_t->endpoint_id];
				uvm_sdv_transport_msg_t *n;

				if (target_endpoint->recv_func) {
					// Notify the endpoint
					target_endpoint->recv_func(target_endpoint->recv_func_ud, msg_t);
					// Release the message
					tp->free_msg(tp, msg_t);
				} else {
					// Queue on endpoint handle
					if (target_endpoint->recv_msg_queue) {
						// Find the end of the message queue
						n = target_endpoint->recv_msg_queue;
						while (n->next) {
							n = n->next;
						}
						n->next = msg_t;
						msg_t->next = 0;
					} else {
						// No messages waiting
						target_endpoint->recv_msg_queue = msg_t;
						msg_t->next = 0;
					}
					// TODO: wake up target endpoint (?)
				}
			}
		}
	}

	return msg;
}

uvm_sdv_transport_msg_t *uvm_sdv_endpoint_mgr_recv_msgid(
		uvm_sdv_transport_t 		*tp,
		uvm_sdv_msg_e				msg_type,
		int32_t						msg_id)
{
	uvm_sdv_endpoint_mgr_t *endpoint_mgr = uvm_sdv_endpoint_mgr_get();
	uvm_sdv_transport_msg_t *msg = 0;
	uvm_sdv_endpoint_t *endpoint = &endpoint_mgr->endpoint_zero.endpoint;

	if (endpoint->recv_msg_queue) {
		// Search through queued messages
		uvm_sdv_transport_msg_t *msg_p, *msg_t;

		msg_p = 0;
		msg_t = msg_p;

		while (msg_t) {
			if (msg_t->msg_type == msg_type && msg_t->msg_data[0] == msg_id) {
				// remove message
				if (msg_p) {
					// we're not dealing with the head of the list
					msg_p->next = msg_t->next;
				} else {
					// we are dealing with the head of the list
					endpoint->recv_msg_queue = msg_t->next;
				}
				msg = msg_t;
				break;
			}
			msg_p = msg_t;
			msg_t = msg_t->next;
		}
	}

	if (!msg) {
		// Wait for a message to be received
		while (1) {
			uvm_sdv_transport_msg_t *msg_t;

			msg_t = tp->recv_msg(tp, 1);

			if (msg_t->endpoint_id == 0 && msg_t->msg_type == msg_type &&
					msg_t->msg_data[0] == msg_id) {
				// matches
				msg = msg_t;
				break;
			} else {
				// Queue on the endpoint handle
				uvm_sdv_endpoint_t *target_endpoint = endpoint_mgr->endpoint_list[msg_t->endpoint_id];
				uvm_sdv_transport_msg_t *n;

				if (target_endpoint->recv_func) {
					target_endpoint->recv_func(target_endpoint->recv_func_ud, msg_t);
				} else {
					// Queue on endpoint handle
					if (target_endpoint->recv_msg_queue) {
						// Find the end of the message queue
						n = target_endpoint->recv_msg_queue;
						while (n->next) {
							n = n->next;
						}
						n->next = msg_t;
						msg_t->next = 0;
					} else {
						// No messages waiting
						target_endpoint->recv_msg_queue = msg_t;
						msg->next = 0;
					}

					// TODO: wake up target endpoint (?)
				}
			}
		}
	}

	return msg;
}

void uvm_sdv_endpoint_mgr_free_msg(uvm_sdv_transport_msg_t *msg)
{
	uvm_sdv_transport_t *tp = uvm_sdv_endpoint_mgr_get_transport();

	tp->free_msg(tp, msg);
}

void uvm_sdv_endpoint_queue_msg(uvm_sdv_endpoint_t *ep, uvm_sdv_transport_msg_t *msg)
{
	uvm_sdv_transport_msg_t *n;

	// TODO: Mutex
	if (ep->recv_msg_queue) {
		n = ep->recv_msg_queue;
		while (n->next) {
			n = n->next;
		}

		n->next = msg;
	} else {
		ep->recv_msg_queue = msg;
	}

	msg->next = 0;
}
