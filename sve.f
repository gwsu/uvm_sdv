
// -F ./src/sdv/sv/sve.f

+incdir+${UVM_HOME}/src
${UVM_HOME}/src/uvm_pkg.sv

+incdir+${PROJECT_LOC}/src/sdv/sv
${PROJECT_LOC}/src/sdv/sv/common/uvm_sdv_pkg.sv
${PROJECT_LOC}/src/sdv/sv/dpi_connector/uvm_sdv_dpi_pkg.sv
${PROJECT_LOC}/src/sdv/sv/memblk_connector/uvm_sdv_memblk_connector_pkg.sv

-f ${SIMPLE_DPI}/sv/types/types.f
-f ${SIMPLE_DPI}/tb/simple_dpi_env.f
-f ${SIMPLE_DPI}/tests/simple_dpi_tests.f
-f ${SIMPLE_DPI}/tb/simple_dpi_tb.f

