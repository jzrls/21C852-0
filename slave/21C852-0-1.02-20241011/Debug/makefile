################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"../cmd/F28335_nonBIOS.cmd" \
"./source/App_AD_RDC.obj" \
"./source/App_Arith.obj" \
"./source/App_Control.obj" \
"./source/App_Ev.obj" \
"./source/App_IsrFunction.obj" \
"./source/App_Main.obj" \
"./source/App_MainLoop.obj" \
"./source/App_PI.obj" \
"./source/App_Ref_Trans.obj" \
"./source/App_Scib.obj" \
"./source/App_Spi.obj" \
"./source/F2812_eCAN.obj" \
"./source/F28335_AsmCode.obj" \
"./source/F28335_CSMPasswords.obj" \
"./source/F28335_GlobalVariableDefs.obj" \
"./source/F28335_InitECan.obj" \
"./source/F28335_InitEPwm.obj" \
"./source/F28335_InitGpio.obj" \
"./source/F28335_InitPie.obj" \
"./source/F28335_Initialize.obj" \
"./source/Mk_CRC.obj" \
"./source/Table.obj" \
"../cmd/F28335_Flash.cmd" \
"../lib/IQmath.lib" \
"../lib/rts2800_ml.lib" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include cmd/subdir_vars.mk
-include lib/subdir_vars.mk
-include source/subdir_vars.mk
-include cmd/subdir_rules.mk
-include lib/subdir_rules.mk
-include source/subdir_rules.mk
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
21C852-0-1.02-20241011.out \

EXE_OUTPUTS__QUOTED += \
"21C852-0-1.02-20241011.out" \

BIN_OUTPUTS += \
21C852-0-1.02-20241011.hex \

BIN_OUTPUTS__QUOTED += \
"21C852-0-1.02-20241011.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "21C852-0-1.02-20241011.out"

# Tool invocations
21C852-0-1.02-20241011.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=softlib --advice:performance=all --define="_DEBUG" --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"21C852-0-1.02-20241011.map" --stack_size=1024 --warn_sections -i"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/lib" -i"C:/Users/lisheng/Desktop/PROJ/21C852-0/slave/21C852-0-1.02-20241011" -i"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="21C852-0-1.02-20241011_linkInfo.xml" --rom_model -o "21C852-0-1.02-20241011.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

21C852-0-1.02-20241011.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/hex2000"  -o "21C852-0-1.02-20241011.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "source\App_AD_RDC.obj" "source\App_Arith.obj" "source\App_Control.obj" "source\App_Ev.obj" "source\App_IsrFunction.obj" "source\App_Main.obj" "source\App_MainLoop.obj" "source\App_PI.obj" "source\App_Ref_Trans.obj" "source\App_Scib.obj" "source\App_Spi.obj" "source\F2812_eCAN.obj" "source\F28335_AsmCode.obj" "source\F28335_CSMPasswords.obj" "source\F28335_GlobalVariableDefs.obj" "source\F28335_InitECan.obj" "source\F28335_InitEPwm.obj" "source\F28335_InitGpio.obj" "source\F28335_InitPie.obj" "source\F28335_Initialize.obj" "source\Mk_CRC.obj" "source\Table.obj" 
	-$(RM) "source\App_AD_RDC.d" "source\App_Arith.d" "source\App_Control.d" "source\App_Ev.d" "source\App_IsrFunction.d" "source\App_Main.d" "source\App_MainLoop.d" "source\App_PI.d" "source\App_Ref_Trans.d" "source\App_Scib.d" "source\App_Spi.d" "source\F2812_eCAN.d" "source\F28335_GlobalVariableDefs.d" "source\F28335_InitECan.d" "source\F28335_InitEPwm.d" "source\F28335_InitGpio.d" "source\F28335_InitPie.d" "source\F28335_Initialize.d" "source\Mk_CRC.d" 
	-$(RM) "source\F28335_AsmCode.d" "source\F28335_CSMPasswords.d" "source\Table.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

