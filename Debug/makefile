################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./bootload.obj" \
"./ipc_interrupt.obj" \
"./main.obj" \
"./inc/DDR3/DDR3_init.obj" \
"./inc/KeyStone_common.obj" \
"../KeyStone_Test.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/lib/ti.csl.ae66" \
-l"C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/lib/ti.csl.intc.ae66" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include inc/DDR3/subdir_vars.mk
-include inc/subdir_vars.mk
-include subdir_rules.mk
-include inc/DDR3/subdir_rules.mk
-include inc/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
C6678_core_IPC_Interrupt.out \

EXE_OUTPUTS__QUOTED += \
"C6678_core_IPC_Interrupt.out" \


# All Target
all: C6678_core_IPC_Interrupt.out

# Tool invocations
C6678_core_IPC_Interrupt.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C6000 Linker'
	"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/bin/cl6x" -mv6600 --abi=eabi -g --diag_wrap=off --display_error_number --diag_warning=225 -z -m"C6678_core_IPC_Interrupt.map" -i"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/lib" -i"D:/CCSv74/Workspace/C6678_core_IPC_Interrupt/inc" -i"D:/CCSv74/ccsv7/tools/compiler/ti-cgt-c6000_8.0.4/include" -i"C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/lib" --reread_libs --display_error_number --warn_sections --diag_wrap=off --xml_link_info="C6678_core_IPC_Interrupt_linkInfo.xml" --rom_model -o "C6678_core_IPC_Interrupt.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "bootload.obj" "ipc_interrupt.obj" "main.obj" "inc\DDR3\DDR3_init.obj" "inc\KeyStone_common.obj" 
	-$(RM) "ipc_interrupt.d" "main.d" "inc\DDR3\DDR3_init.d" "inc\KeyStone_common.d" 
	-$(RM) "bootload.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

