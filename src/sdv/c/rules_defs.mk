
UVM_SDV_C_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

UVM_SDV_SRC = \
	uvm_sdv_endpoint_mgr.c 		\
	uvm_sdv_memblk_transport.c 	\
	uvm_sdv_messaging.c			\
	uvm_sdv_phasing.c			\
	uvm_sdv_analysis_port.c		\
	uvm_sdv_packer.c			\
	uvm_sdv_sequencer_driver.c	\
	uvm_sdv_sequence.c			\
	uvm_sdv_endpoint_zero.c		\
	uvm_sdv_config_db.c			\
	
	
SRC_DIRS += $(UVM_SDV_C_DIR)/common $(UVM_SDV_C_DIR)/memblk_transport

else

libuvm_sdv.a : $(UVM_SDV_SRC:.c=.o)
	rm -rf $@
	$(AR) vcq $@ $^
	
endif