
include $(ROOTDIR)/common/plusargs.mk

UVM_SDV:=$(ROOTDIR)/../src/sdv
include $(UVM_SDV)/c/rules_defs.mk
include $(UVM_SDV)/dpi/rules_defs.mk

SCRIPTS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

TB=simple_dpi_tb

SRC_DIRS += $(SIMPLE_DPI)/tests/sw

# Must build DPI libraries
DPI_LIBRARIES += $(call get_plusarg,DPI_LIBRARY,$(PLUSARGS))
#LIB_TARGETS += $(foreach lib,$(call get_plusarg,DPI_LIBRARY,$(PLUSARGS)),$(lib)$(DLLEXT))
LIB_TARGETS += $(foreach lib,$(DPI_LIBRARIES),$(lib)$(DLLEXT))

CFLAGS += \
  -I$(SIMPLE_DPI)/tests/sw \
  -I$(UVM_SDV)/c/common \
  -I$(UVM_SDV)/dpi

	
include $(ROOTDIR)/common/common_sim.mk

# SIM_DATAFILES += $(call get_plusarg,SW_IMAGE,$(PLUSARGS))
# UVM_TESTNAME := $(call get_plusarg,UVM_TESTNAME,$(PLUSARGS))

$(BUILD_DIR)/dpi/%$(DLLEXT) : \
	$(BUILD_DIR)/dpi/%.o \
	$(BUILD_DIR)/dpi/sw_txn.o \
	$(UVM_SDV_SRC:.c=.o) \
	$(UVM_SDV_DPI_SRC:.c=.o) 
	echo "UVM_SDV_C_DIR=$(UVM_SDV_C_DIR)"
	ls $(UVM_SDV_C_DIR)/common
	$(MK_DPI)

%.o : $(UVM_SDV)/c/common/%.c
	echo "BUILD"
	$(CC) -c $(CFLAGS) -o $@ $^

%.o : $(UVM_SDV)/dpi/%.c
	$(CC) -c $(CFLAGS) -o $@ $^
	
$(BUILD_DIR)/dpi/%.o : $(SIMPLE_DPI)/tests/sw/%.c
	@if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(CC) -c $(CFLAGS) -o $@ $^

	