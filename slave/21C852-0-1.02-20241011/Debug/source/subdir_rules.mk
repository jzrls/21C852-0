################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/%.obj: ../source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=softlib --include_path="C:/Users/lisheng/Desktop/PROJ/21C852-0/slave/21C852-0-1.02-20241011" --include_path="C:/Users/lisheng/Desktop/PROJ/21C852-0/slave/21C852-0-1.02-20241011/include" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/include" --advice:performance=all --define="_DEBUG" --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source/%.obj: ../source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=softlib --include_path="C:/Users/lisheng/Desktop/PROJ/21C852-0/slave/21C852-0-1.02-20241011" --include_path="C:/Users/lisheng/Desktop/PROJ/21C852-0/slave/21C852-0-1.02-20241011/include" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.1.LTS/include" --advice:performance=all --define="_DEBUG" --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


