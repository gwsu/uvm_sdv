/****************************************************************************
 * uvm_sdv_memblk_transport.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_MEMBLK_TRANSPORT_H
#define INCLUDED_UVM_SDV_MEMBLK_TRANSPORT_H
#include "uvm_sdv_transport.h"

uvm_sdv_transport_t *uvm_sdv_memblk_transport_create(
		uint32_t			*memblk,
		uint32_t			memblk_sz);

#endif /* INCLUDED_UVM_SDV_MEMBLK_TRANSPORT_H */
