
#ifndef INCLUDED_SW_TXN_H
#define INCLUDED_SW_TXN_H
#include <stdint.h>
#include "uvm_sdv.h"

typedef struct sw_txn_s {
	uint32_t			A;
	uint32_t			B;
} sw_txn;


void sw_txn_pack(uvm_sdv_packer_t *packer, void *obj);
void sw_txn_unpack(uvm_sdv_packer_t *packer, void *obj);


#endif /* INCLUDED_SW_TXN_H */
