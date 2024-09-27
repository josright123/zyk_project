/**
  **************************************************************************
  * @file     dm9051_env.h
  * @version  v1.0
  * @date     2023-04-28
  * @brief    header file of dm9051 environment config
  **************************************************************************
  */
#ifndef __DM9051_ENV_H
#define __DM9051_ENV_H

#include "dm9051opts.h"
#include "cboard/dm9051_lw_mcu_default_IN.h"
#include "dm9051_lw.h"

int env_init_setup(uint16_t *id);

uint16_t env_evaluate_rxb(uint8_t rxb);
const uint8_t *env_reset_process(const uint8_t *macaddr, enable_t en);
uint16_t env_err_rsthdlr(char *err_explain_str, uint32_t valuecode);

#endif //__DM9051_ENV_H
