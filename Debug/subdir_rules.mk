################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
bootload.obj: ../bootload.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/C6678_core_IPC_Interrupt/inc" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="bootload.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ipc_interrupt.obj: ../ipc_interrupt.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/C6678_core_IPC_Interrupt/inc" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="ipc_interrupt.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/C6678_core_IPC_Interrupt/inc" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


