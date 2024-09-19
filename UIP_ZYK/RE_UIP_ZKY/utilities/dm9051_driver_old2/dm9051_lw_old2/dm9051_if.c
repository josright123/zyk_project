/*
 * Copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * File: dm9051_if.c
 * This DM9051 Driver for LWIP tcp/ip stack
 * First veryification with : AT32F415
 *
 * Author: Joseph CHANG <joseph_chang@davicom.com.tw>
 * Date: 20230411
 * Date: 20240215 (Versions log please refer to 'dm9051_lw/info.txt')
 * Date: 20240614
 */

/* SPI master control is essential,
 * Do define spi specific definition, depend on your CPU's board support package library.
 * Here we had the definition header file with : "at32f415_spi.h"
 * for AT32F415 cpu.
 */
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cboard.h"
#include "dm9051_lw_cspi.h"

#if 0
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//#include "dm9051_bl_xxx.h"
#endif

#if freeRTOS
#include "lwip/sys.h"
#include "lwip/err.h"
#endif

//#include "dm9051opts.h"
//#include "dm9051_lw.h"
//#include "dm9051_lw_cboard.h"
//#include "dm9051_lw_cspi.h"

//#include "dm9051opts.h"
//#include "dm9051_lw.h"
//#include "dm9051_lw_cboard.h" //ADD
//#include "dm9051_lw_cspi.h"

//#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_link_log_reset (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_poweron_reset_pulse (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol impl_dm9051_isr_disab (referred from .\objects\dm9051_interface.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol impl_dm9051_isr_enab (referred from .\objects\dm9051_interface.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_link_log_reset (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_poweron_reset_pulse (referred from .\objects\dm9051_lw_cboard.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol lock_dm9051_core (referred from .\objects\dm9051_bl_impl.o).

//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_if.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_core_reset (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_mac_adr (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_rx_mode (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol is_valid_ether_addr (referred from .\objects\dm9051_function_block.o).
//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_core_reset (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_mac_adr (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_rx_mode (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol is_valid_ether_addr (referred from .\objects\dm9051_function_block.o).

//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_reg (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_ncr_reset (referred from .\objects\dm9051_function_block.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_phycore_on (referred from .\objects\dm9051_function_block.o).
//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_read_regs (referred from .\objects\dm9051_control_cblock.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cspi_write_reg (referred from .\objects\dm9051_control_cblock.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol cpin_poweron_reset (referred from .\objects\dm9051_function_code.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_ncr_reset (referred from .\objects\dm9051_function_code.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_phycore_on (referred from .\objects\dm9051_function_code.o).

//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm_delay_ms (referred from .\objects\dm9051_control_cblock.o).

//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol ev_rxb (referred from .\objects\dm9051_function_code.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol ev_status (referred from .\objects\dm9051_function_code.o).

//linking...
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_disab (referred from .\objects\dm_eth.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_eth_irq_enab (referred from .\objects\dm_eth.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol intr_gpio_mptr (referred from .\objects\dm_eth.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_extline_irq_enable (referred from .\objects\dm9051_function_code.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol DataObj_store (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol ETH_COUNT_VOIDFN (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol de_pin (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol devconf (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol dm9051_board_counts_display (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol get_cpin_init_dataptr (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol option_rst_common (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol rst_pin_mexist (referred from .\objects\dm9051_lw_cboard.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol gpio_pin_level (referred from .\objects\dm9051_lw_crst_cgpio.o).
//.\objects\at32f403a_freertos.axf: Error: L6218E: Undefined symbol rst_pin_exister (referred from .\objects\dm9051_lw_crst_cgpio.o).
