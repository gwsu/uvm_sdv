
UVM_SDV_DPI_SRC = \
	uvm_sdv_dpi_transport.c

libuvm_sdv_dpi.a : $(UVM_SDV_DPI_SRC:.c=.o)
	rm -f $@
	$(AR) vcq $@ $^
	
	