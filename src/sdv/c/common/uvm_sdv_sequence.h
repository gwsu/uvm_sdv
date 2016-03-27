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
#ifndef INCLUDED_UVM_SDV_SEQUENCE_H
#define INCLUDED_UVM_SDV_SEQUENCE_H
#include "uvm_sdv_config.h"

/********************************************************************
 * uvm_sdv_sequence_start()
 *
 * Launches the sequence with type-name 'seq' on sequencer with
 * instance path 'seqr'.
 *
 * Returns:
 * Sequence instance id (>= 1) on success
 * 0 on failure
 ********************************************************************/
uint32_t uvm_sdv_sequence_start(
		const char				*seq,
		const char				*seqr);

/********************************************************************
 * uvm_sdv_sequence_is_running()
 *
 * Launches the sequence with type-name 'seq' on sequencer with
 * instance path 'seqr'.
 *
 * Returns:
 * Sequence instance id (>= 0) on success
 * -1 on failure
 ********************************************************************/
uint32_t uvm_sdv_sequence_is_running(uint32_t seq_id);

#endif /* INCLUDED_UVM_SDV_SEQUENCE_H */
