# APIs

## dm9051 edriver

1. eth_main<br>  (DM_ETH_SET_IFlag[flg_Semaphore_r'1],
                  <br>DM_ETH_GET_IFlag[true,flg_Semaphore_r'0/false])
2. dm9051<br>    (dm9051_init/dm9051_rx/dm9051_tx/dm9051_clr_ievent)
3. hal_main<br>  (dm9051_hal_spi_config/dm9051_hal_spi_xfer)
                  <br>=dm9051if_exc_data
4. hal_gpio<br>  (dm9051_hal_stdpin_config/dm9051_hal_stdpin_low/dm9051_hal_stdpin_high)
                  <br>=dm9051if_gpio_lo<br>=dm9051if_gpio_hi
				  <br>(dm9051_hal_intr_config,
				  <br> cint_enable_mcu_irq_AT,
				  <br>cint_disable_mcu_irq_AT)