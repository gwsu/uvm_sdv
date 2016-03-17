/****************************************************************************
 * uvm_sdv_config_db.h
 ****************************************************************************/
#ifndef INCLUDED_UVM_SDV_CONFIG_DB_H
#define INCLUDED_UVM_SDV_CONFIG_DB_H
#include "uvm_sdv_config.h"
#include "uvm_sdv_packer.h"

uint32_t uvm_sdv_config_db_get_int(
		const char 		*name,
		uint64_t 			*val);

uint32_t uvm_sdv_config_db_get_string(
		const char 		*name,
		const char			**val);

uint32_t uvm_sdv_config_db_get_object(
		const char 		*name,
		uvm_sdv_pack_func 	pack_func,
		void				*obj);

#endif /* INCLUDED_UVM_SDV_CONFIG_DB_H */
