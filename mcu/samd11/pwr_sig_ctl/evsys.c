//
// Created by wieker on 10/11/20.
//

#include "evsys.h"
#include "samd11.h"



void evsys_init() {// TODo: implement EVSYS PWM => ADC
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(EVSYS_GCLK_ID_0) |
                      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(EVSYS_GCLK_ID_1) |
                      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  EVSYS->USER.reg = EVSYS_USER_CHANNEL(1) | EVSYS_USER_USER(EVSYS_ID_USER_ADC_START);
  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_TCC0_MCX_1) | EVSYS_CHANNEL_PATH_ASYNCHRONOUS |
                       EVSYS_CHANNEL_CHANNEL(0) | EVSYS_CHANNEL_EDGSEL(0);

  EVSYS->USER.reg = EVSYS_USER_CHANNEL(2) | EVSYS_USER_USER(EVSYS_ID_USER_ADC_SYNC);
  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EVGEN(EVSYS_ID_GEN_DMAC_CH_0) | EVSYS_CHANNEL_PATH_ASYNCHRONOUS |
                       EVSYS_CHANNEL_CHANNEL(1) | EVSYS_CHANNEL_EDGSEL(0);
}
