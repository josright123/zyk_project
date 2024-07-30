################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/demo/src/dm_eth.c \
../utilities/demo/src/netconf.c \
../utilities/demo/src/netconf1.c 

OBJS += \
./utilities/demo/src/dm_eth.o \
./utilities/demo/src/netconf.o \
./utilities/demo/src/netconf1.o 

C_DEPS += \
./utilities/demo/src/dm_eth.d \
./utilities/demo/src/netconf.d \
./utilities/demo/src/netconf1.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/demo/src/dm_eth.o: ../utilities/demo/src/dm_eth.c utilities/demo/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c18 '-DDEBUG_EFM=1' '-DMGM240PB32VNA=1' '-DSL_BOARD_NAME="BRD4317A"' '-DSL_BOARD_REV="A03"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' '-DCMSIS_NVIC_VIRTUAL=1' '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' '-DSL_MEMORY_POOL_LIGHT=1' '-DSL_CODE_COMPONENT_CORE=core' '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\src\include" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\dm9051_driver\dm9051_lw" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\demo\inc" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port\arch" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\config" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\autogen" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/Device/SiliconLabs/MGM24/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/board/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/Core/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/RTOS2/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/configuration_over_swo/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/debug/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_init/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/emlib/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/cmsis/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/peripheral/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc/arm" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/leddrv/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip/src/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip-contrib/ports/freertos/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/toolchain/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/system/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/sleeptimer/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"utilities/demo/src/dm_eth.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/demo/src/netconf.o: ../utilities/demo/src/netconf.c utilities/demo/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c18 '-DDEBUG_EFM=1' '-DMGM240PB32VNA=1' '-DSL_BOARD_NAME="BRD4317A"' '-DSL_BOARD_REV="A03"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' '-DCMSIS_NVIC_VIRTUAL=1' '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' '-DSL_MEMORY_POOL_LIGHT=1' '-DSL_CODE_COMPONENT_CORE=core' '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\src\include" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\dm9051_driver\dm9051_lw" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\demo\inc" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port\arch" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\config" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\autogen" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/Device/SiliconLabs/MGM24/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/board/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/Core/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/RTOS2/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/configuration_over_swo/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/debug/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_init/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/emlib/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/cmsis/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/peripheral/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc/arm" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/leddrv/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip/src/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip-contrib/ports/freertos/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/toolchain/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/system/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/sleeptimer/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"utilities/demo/src/netconf.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utilities/demo/src/netconf1.o: ../utilities/demo/src/netconf1.c utilities/demo/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c18 '-DDEBUG_EFM=1' '-DMGM240PB32VNA=1' '-DSL_BOARD_NAME="BRD4317A"' '-DSL_BOARD_REV="A03"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' '-DCMSIS_NVIC_VIRTUAL=1' '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' '-DSL_MEMORY_POOL_LIGHT=1' '-DSL_CODE_COMPONENT_CORE=core' '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer' -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\src\include" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\dm9051_driver\dm9051_lw" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\utilities\demo\inc" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\middlewares\freertos_tcpip_stack\port\arch" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\config" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\autogen" -I"C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/Device/SiliconLabs/MGM24/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/board/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/clock_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/Core/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/CMSIS/RTOS2/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/configuration_over_swo/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/debug/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/device_init/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/emlib/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/cmsis/Include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/peripheral/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/interrupt_manager/inc/arm" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/driver/leddrv/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip/src/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//util/third_party/lwip/lwip-contrib/ports/freertos/include" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/memory_manager/src" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/common/toolchain/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/system/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/sleeptimer/inc" -I"C:/Users/user/SimplicityStudio/SDKs/simplicity_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"utilities/demo/src/netconf1.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


