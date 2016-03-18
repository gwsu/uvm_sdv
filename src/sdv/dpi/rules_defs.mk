
UVM_SDV_DPI_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

UVM_SDV_DPI_SRC = \
	uvm_sdv_dpi_transport.c	
	
else

endif