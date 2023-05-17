################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
driver/KeyStone_DDR_Init.obj: ../driver/KeyStone_DDR_Init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="driver/KeyStone_DDR_Init.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driver/KeyStone_common.obj: ../driver/KeyStone_common.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="driver/KeyStone_common.d_raw" --obj_directory="driver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


