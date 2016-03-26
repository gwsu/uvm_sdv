
UVM_SDV_C_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

UVM_SDV_C_COMMON_DIR := $(UVM_SDV_C_DIR)/common

ifneq (1,$(RULES))

UVM_SDV_SRC = \
	uvm_sdv_endpoint_mgr.c 		\
	uvm_sdv_messaging.c			\
	uvm_sdv_phasing.c			\
	uvm_sdv_analysis_port.c		\
	uvm_sdv_packer.c			\
	uvm_sdv_sequencer_driver.c	\
	uvm_sdv_sequence.c			\
	uvm_sdv_endpoint_zero.c		\
	uvm_sdv_config_db.c
	
UVM_SDV_MEMBLK_SRC = \
	uvm_sdv_memblk_transport.c 	

else

libuvm_sdv.a : $(UVM_SDV_SRC:.c=.o)
	rm -rf $@
	$(AR) vcq $@ $^
	
endif