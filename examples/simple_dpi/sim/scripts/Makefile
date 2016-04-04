
include $(SIMSCRIPTS_DIR)/mkfiles/plusargs.mk

UVM_SDV:=$(ROOTDIR)/../src/sdv
include $(UVM_SDV)/c/rules_defs.mk
include $(UVM_SDV)/dpi/rules_defs.mk

SCRIPTS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

TB=simple_dpi_tb

SRC_DIRS += $(SIMPLE_DPI)/tests/sw

# Must build DPI libraries
DPI_LIBRARIES += $(call get_plusarg,DPI_LIBRARY,$(PLUSARGS))
#LIB_TARGETS += $(foreach lib,$(call get_plusarg,DPI_LIBRARY,$(PLUSARGS)),$(lib)$(DLLEXT))
LIB_TARGETS += $(foreach lib,$(DPI_LIBRARIES),$(lib)$(DPIEXT))

CFLAGS += \
  -I$(SIMPLE_DPI)/tests/sw \
  -I$(UVM_SDV)/c/common \
  -I$(UVM_SDV)/dpi

	
include $(SIMSCRIPTS_DIR)/mkfiles/common_sim.mk

# SIM_DATAFILES += $(call get_plusarg,SW_IMAGE,$(PLUSARGS))
# UVM_TESTNAME := $(call get_plusarg,UVM_TESTNAME,$(PLUSARGS))

$(BUILD_DIR)/dpi/%$(DPIEXT) : \
	$(BUILD_DIR)/dpi/obj/%.o \
	$(BUILD_DIR)/dpi/obj/sw_txn.o \
	$(UVM_SDV_SRC:.c=.o) \
	$(UVM_SDV_DPI_SRC:.c=.o) 
	$(MK_DPI)

%.o : $(UVM_SDV)/c/common/%.c
	$(CC) -c $(CFLAGS) -o $@ $^

%.o : $(UVM_SDV)/dpi/%.c
	$(CC) -c $(CFLAGS) -o $@ $^
	
$(BUILD_DIR)/dpi/obj/%.o : $(SIMPLE_DPI)/tests/sw/%.c
	@if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	$(CC) -c $(CFLAGS) -o $@ $^

	
