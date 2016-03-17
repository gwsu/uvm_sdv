/****************************************************************************
 * uvm_sdv_sequence.h
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
