################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
inc/DF/df_frame.obj: ../inc/DF/df_frame.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/C6678_core_IPC_Interrupt/inc" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" --include_path="D:/CCSv74/Workspace/common" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="inc/DF/df_frame.d_raw" --obj_directory="inc/DF" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


