
#********************************************************************
#* Compile rules
#********************************************************************

LD_LIBRARY_PATH := $(SVF_LIBDIR)/sc:$(LD_LIBRARY_PATH)

CXXFLAGS += -I$(SYSTEMC)/include -I$(SOCBLOX)/svf/sc
CXXFLAGS += -I$(BUILD_DIR)/obj_dir
CXXFLAGS += -I$(VERILATOR_ROOT)/include
CXXFLAGS += -I$(VERILATOR_ROOT)/include/vltstd

ifeq (1,$(VERILATOR_TRACE_EN))
CXXFLAGS += -DVL_TRACE_EN
TRACE=--trace
endif

ifeq ($(SIMX),1)
include V$(TB).mk
endif

-include verilator.d

#VL_DEBUG_FLAGS=
#VL_DEBUG_FLAGS=--debug
#VL_DEBUG_FLAGS=--debug --gdbbt

OS := $(shell uname -o)
ARCH := $(shell uname -m)
ifeq (Cygwin,$(OS))
ifeq (x86_64,$(ARCH)) 
  SYSTEMC_ARCH=cygwin64
else
  SYSTEMC_ARCH=cygwin
endif
else # Linux
ifeq (x86_64,$(ARCH)) 
  SYSTEMC_ARCH=linux64
else
  SYSTEMC_ARCH=linux
endif
endif

vlog_build : $(VERILATOR_DEPS)
	rm -rf obj_dir
	verilator $(TRACE) --sc -Wno-fatal -MMD $(VL_DEBUG_FLAGS) \
		--top-module $(TOP_MODULE) \
		$(VL_VLOG_ARGS) $(VLOG_ARGS) 
	sed -e 's/^[^:]*: /VERILATOR_DEPS=/' obj_dir/V$(TB)__ver.d > verilator.d
	touch $@

build : simx_build $(EXE_TARGETS) target_build
		
simx_build : vlog_build $(LIB_TARGETS) $(TESTBENCH_OBJS) 	
	$(MAKE) SOCBLOX=$(SOCBLOX) TB=$(TB) SIMX=1 TESTBENCH_OBJS="$(TESTBENCH_OBJS)" \
	    VERILATOR_TRACE_EN=$(VERILATOR_TRACE_EN) BFM_LIBS="$(BFM_LIBS)" \
	    LIBSVF_LINK="$(LIBSVF_LINK)" \
	    LIBSVF_SC_LINK="$(LIBSVF_SC_LINK)" \
	    -C obj_dir -f $(COMMON_SIM_MK) \
		$(BUILD_DIR)/simx
	touch $@


#ifeq (true,$(VERBOSE))
#$(BUILD_DIR)/simx : $(VK_GLOBAL_OBJS) V$(TB)__ALL.a $(TESTBENCH_OBJS) $(BUILD_DIR)/objs/$(TB).o
#	echo "ARCH=$(ARCH)"
#	$(CXX) -o $(BUILD_DIR)/simx \
#	    	$(BUILD_DIR)/objs/$(TB).o \
#			$(TESTBENCH_OBJS) \
#			-Wl,--whole-archive \
#			V$(TB)__ALL.a \
#			-Wl,--no-whole-archive \
#			$(VK_GLOBAL_OBJS) \
#			$(foreach l,$(filter %.so, $(BFM_LIBS)), -L$(dir $(l)) -l$(subst lib,,$(basename $(notdir $(l))))) \
#			$(LIBSVF_SC_LINK) \
#			$(SYSTEMC)/lib-$(SYSTEMC_ARCH)/libsystemc.a -lpthread
#else
ifeq (true,$(DYNLINK))
$(BUILD_DIR)/simx : $(VK_GLOBAL_OBJS) V$(TB)__ALL.a $(TESTBENCH_OBJS) $(BUILD_DIR)/objs/$(TB).o
	$(Q)echo "link simx"
	$(Q)$(CXX) -o $(BUILD_DIR)/simx \
	    	$(BUILD_DIR)/objs/$(TB).o \
			$(TESTBENCH_OBJS) \
			-Wl,--whole-archive \
			V$(TB)__ALL.a \
			-Wl,--no-whole-archive \
			$(VK_GLOBAL_OBJS) \
			$(foreach l,$(filter %.so, $(BFM_LIBS)), -L$(dir $(l)) -l$(subst lib,,$(basename $(notdir $(l))))) \
			$(LIBSVF_SC_LINK) \
			$(SYSTEMC)/lib-$(SYSTEMC_ARCH)/libsystemc.a -lpthread
else
$(BUILD_DIR)/simx : $(VK_GLOBAL_OBJS) V$(TB)__ALL.a $(TESTBENCH_OBJS) $(BUILD_DIR)/objs/$(TB).o
	$(Q)echo "Static link simx"
	$(Q)$(CXX) -o $(BUILD_DIR)/simx \
	    	$(BUILD_DIR)/objs/$(TB).o \
			$(TESTBENCH_OBJS) \
			-Wl,--whole-archive \
			V$(TB)__ALL.a \
			-Wl,--no-whole-archive \
			$(VK_GLOBAL_OBJS) \
			$(BFM_LIBS) \
			$(LIBSVF_SC_LINK) \
			$(SYSTEMC)/lib-$(SYSTEMC_ARCH)/libsystemc.a -lpthread
endif
		
ifeq (true,$(VERBOSE))			
$(BUILD_DIR)/objs/$(TB).o : $(SIM_DIR)/../tb/$(TB).cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^
else	
$(BUILD_DIR)/objs/$(TB).o : $(SIM_DIR)/../tb/$(TB).cpp
	@echo "$(CXX) `basename $^`"
	@$(CXX) -c $(CXXFLAGS) -o $@ $^
endif

ifeq (true,$(VERBOSE))
else	
%.o : %.cpp
	@echo "$(CXX) `basename $^`"
	@$(CXX) -c $(CXXFLAGS) -o $@ $^
endif

ifeq ($(DEBUG),true)
RT_TRACE_FLAGS = -trace -tracefile vlt_dump.fst
# RT_TRACE_FLAGS = -trace -tracefile vlt_dump.vcd
endif

#VALGRIND=valgrind --tool=memcheck
# GDB=gdbserver localhost:6000 

#********************************************************************
#* Simulation settings
#********************************************************************

run : sim_datafiles
	$(GDB) $(VALGRIND) $(BUILD_DIR)/simx +TESTNAME=$(TESTNAME) +TIMEOUT=$(TIMEOUT) \
		$(PLUSARGS) \
          -f sim.f $(RT_TRACE_FLAGS) 2>&1 | tee simx.log

sim_datafiles:
	@all_datafiles="$(SIM_DATAFILES)"; \
	if test "x$$all_datafiles" != "x"; then \
                echo "Note: Collecting Simulation Data Files"; \
		for src in $$all_datafiles; do \
                  dst=`echo $$src | sed -e 's@^.*=\(.*\)$$@\1@'`; \
                  dst_b=`basename $$src`; \
                  if test "$$dst" = "$$src"; then \
                    cp $$src $$dst_b; \
                    echo "  SimDatafile: $$src"; \
                  else \
                    src=`echo $$src | sed -e 's@^\(.*\)=.*$$@\1@'`; \
                    cp $$src $$dst; \
                    echo "  SimDatafile: $$src => $$dst"; \
                  fi \
                done; \
        else \
          echo "Note: No Simulation Data Files"; \
        fi

#sim_datafiles:
#	all_datafiles="$(SIM_DATAFILES) $(call get_plusarg,SIM_DATAFILE,$(PLUSARGS))"; \
#	echo "all_datafiles: $$all_datafiles"; \
#	if test "x$$all_datafiles" != "x"; then \
#		for src in $$all_datafiles; do \
#                  echo "src=$$src" ; \
#                  cp $$src `basename $$src`;  \
#                done \
#        fi

