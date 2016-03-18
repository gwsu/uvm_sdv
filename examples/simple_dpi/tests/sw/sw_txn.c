
#include "sw_txn.h"


void sw_txn_pack(uvm_sdv_packer_t *packer, void *obj)
{
	sw_txn *txn = (sw_txn *)obj;
	uvm_sdv_pack_int(packer, txn->A, 32);
	uvm_sdv_pack_int(packer, txn->B, 32);
}

void sw_txn_unpack(uvm_sdv_packer_t *packer, void *obj)
{
	sw_txn *txn = (sw_txn *)obj;
	txn->A = uvm_sdv_unpack_int(packer, 32);
	txn->B = uvm_sdv_unpack_int(packer, 32);
}
