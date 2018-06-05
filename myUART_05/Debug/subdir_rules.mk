################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

myClocks.obj: ../myClocks.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="myClocks.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

myGpio.obj: ../myGpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="myGpio.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

myLcd.obj: ../myLcd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="myLcd.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

myUart.obj: ../myUart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="myUart.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

unused_interrupts.obj: ../unused_interrupts.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --include_path="/home/magee/ti/ccsv8/ccs_base/msp430/include" --include_path="/home/magee/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --include_path="/home/magee/Dokumente/HSP-Tonne/git/myUART_05/driverlib/MSP430FR5xx_6xx" --advice:power="none" --advice:hw_config=all -g --define=__MSP430FR6989__ --display_error_number --diag_warning=225 --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="unused_interrupts.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


