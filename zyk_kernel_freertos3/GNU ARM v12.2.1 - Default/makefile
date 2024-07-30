################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/lwip_web2403v2_freelw/subdir.mk
-include utilities/dm9051_driver/dm9051_lw/subdir.mk
-include utilities/demo/src/subdir.mk
-include utilities/demo/subdir.mk
-include utilities/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/lwip/lwip-contrib/ports/freertos/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/MemMang/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/subdir.mk
-include simplicity_sdk_2024.6.0/util/third_party/freertos/cmsis/Source/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/udelay/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/system/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/memory_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/devices/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/device_init/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/service/clock_manager/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/peripheral/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/emlib/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/driver/leddrv/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/driver/debug/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/common/src/subdir.mk
-include simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/MGM24/Source/subdir.mk
-include simplicity_sdk_2024.6.0/platform/CMSIS/RTOS2/Source/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/subdir.mk
-include simplicity_sdk_2024.6.0/hardware/board/src/subdir.mk
-include middlewares/freertos_tcpip_stack/src/netif/subdir.mk
-include middlewares/freertos_tcpip_stack/port/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := zyk_kernel_freertos_3
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: zyk_kernel_freertos_3.axf

# Tool invocations
zyk_kernel_freertos_3.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\basic_2Git\TEST\v5_workspace_All\zyk_kernel_freertos3\autogen\linkerfile.ld" -Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="zyk_kernel_freertos_3.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o zyk_kernel_freertos_3.axf -Wl,--start-group "./app.o" "./blink.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_simple_led_instances.o" "./middlewares/freertos_tcpip_stack/port/ethernetif.o" "./middlewares/freertos_tcpip_stack/src/netif/ethernet.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_control_gpio.o" "./simplicity_sdk_2024.6.0/hardware/board/src/sl_board_init.o" "./simplicity_sdk_2024.6.0/hardware/driver/configuration_over_swo/src/sl_cos.o" "./simplicity_sdk_2024.6.0/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o" "./simplicity_sdk_2024.6.0/platform/CMSIS/RTOS2/Source/os_systick.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/MGM24/Source/startup_mgm24.o" "./simplicity_sdk_2024.6.0/platform/Device/SiliconLabs/MGM24/Source/system_mgm24.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_assert.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_core_cortexm.o" "./simplicity_sdk_2024.6.0/platform/common/src/sl_syscalls.o" "./simplicity_sdk_2024.6.0/platform/driver/debug/src/sl_debug_swo.o" "./simplicity_sdk_2024.6.0/platform/driver/leddrv/src/sl_led.o" "./simplicity_sdk_2024.6.0/platform/driver/leddrv/src/sl_simple_led.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_burtc.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_cmu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_core.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_emu.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_gpio.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_msc.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_prs.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_system.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_timer.o" "./simplicity_sdk_2024.6.0/platform/emlib/src/em_usart.o" "./simplicity_sdk_2024.6.0/platform/peripheral/src/sl_hal_sysrtc.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init.o" "./simplicity_sdk_2024.6.0/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_init/src/sl_device_init_emu_s2.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/clocks/sl_device_clock_efr32xg24.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg24.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/src/sl_device_clock.o" "./simplicity_sdk_2024.6.0/platform/service/device_manager/src/sl_device_peripheral.o" "./simplicity_sdk_2024.6.0/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_pool.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_region.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sl_memory_manager_retarget.o" "./simplicity_sdk_2024.6.0/platform/service/memory_manager/src/sli_memory_manager_common.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o" "./simplicity_sdk_2024.6.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_init.o" "./simplicity_sdk_2024.6.0/platform/service/system/src/sl_system_kernel.o" "./simplicity_sdk_2024.6.0/platform/service/udelay/src/sl_udelay.o" "./simplicity_sdk_2024.6.0/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/cmsis/Source/cmsis_os2.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/croutine.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/event_groups.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/list.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/queue.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/stream_buffer.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/tasks.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/timers.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.o" "./simplicity_sdk_2024.6.0/util/third_party/freertos/kernel/portable/MemMang/heap_3.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/api_lib.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/api_msg.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/err.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/if_api.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/netbuf.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/netdb.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/netifapi.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/sockets.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/api/tcpip.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/altcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/altcp_alloc.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/altcp_tcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/def.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/dns.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/inet_chksum.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/init.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ip.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/mem.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/memp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/netif.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/pbuf.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/raw.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/stats.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/sys.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/tcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/tcp_in.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/tcp_out.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/timeouts.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/udp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/autoip.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/dhcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/etharp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/icmp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/igmp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/ip4.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/ip4_addr.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/core/ipv4/ip4_frag.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/auth.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/ccp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/chap-md5.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/chap-new.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/chap_ms.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/demand.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/eap.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/ecp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/eui64.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/fsm.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/ipcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/ipv6cp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/lcp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/magic.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/mppe.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/multilink.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/ppp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/pppapi.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/pppcrypt.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/pppoe.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/pppol2tp.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/pppos.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/upap.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/utils.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/vj.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/arc4.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/des.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/md4.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/md5.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip/src/netif/ppp/polarssl/sha1.o" "./simplicity_sdk_2024.6.0/util/third_party/lwip/lwip-contrib/ports/freertos/sys_arch.o" "./utilities/demo/freertos_tasks_debug.o" "./utilities/demo/src/dm_eth.o" "./utilities/demo/src/netconf.o" "./utilities/demo/src/netconf1.o" "./utilities/dm9051_cboard.o" "./utilities/dm9051_cboard_conf.o" "./utilities/dm9051_cint.o" "./utilities/dm9051_crst_cgpio.o" "./utilities/dm9051_cspi.o" "./utilities/dm9051_driver/dm9051_lw/dm9051_lw.o" "./utilities/dm9051_driver/dm9051_lw/dm9051_lw_conf.o" "./utilities/dm9051_driver/dm9051_lw/dm9051_lw_control_cblock.o" "./utilities/dm9051_driver/dm9051_lw/dm9051_lw_function_code.o" "./utilities/dm9051_driver/dm9051_lw/dm9051_rtos.o" "./utilities/lwip_web2403v2_freelw/fs_jjdynamic.o" "./utilities/lwip_web2403v2_freelw/httpd.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: zyk_kernel_freertos_3.out'
	arm-none-eabi-objcopy "zyk_kernel_freertos_3.axf" "zyk_kernel_freertos_3.out"
	@echo ' '

	@echo 'Building hex file: zyk_kernel_freertos_3.hex'
	arm-none-eabi-objcopy -O ihex "zyk_kernel_freertos_3.axf" "zyk_kernel_freertos_3.hex"
	@echo ' '

	@echo 'Building bin file: zyk_kernel_freertos_3.bin'
	arm-none-eabi-objcopy -O binary "zyk_kernel_freertos_3.axf" "zyk_kernel_freertos_3.bin"
	@echo ' '

	@echo 'Building s37 file: zyk_kernel_freertos_3.s37'
	arm-none-eabi-objcopy -O srec "zyk_kernel_freertos_3.axf" "zyk_kernel_freertos_3.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "zyk_kernel_freertos_3.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) zyk_kernel_freertos_3.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
