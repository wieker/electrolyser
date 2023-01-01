//
// Created by wieker on 10/1/20.
//

#include "timer.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "samd11.h"

void irq_handler_tc1(void)
{
    if (TC1->COUNT16.INTFLAG.reg & TC_INTFLAG_MC(1))
    {
        //HAL_GPIO_LED_toggle();
        TC1->COUNT16.INTFLAG.reg = TC_INTFLAG_MC(1);
    }
}

void timer_init(void)
{
  PM->APBCMASK.reg |= PM_APBCMASK_TC1;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC1_GCLK_ID) |
                      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  TC1->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ |
                           TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCSYNC_RESYNC;

  TC1->COUNT16.COUNT.reg = 0;

  TC1->COUNT16.CC[0].reg = (F_CPU / 1000ul / 1024) * 250;
  TC1->COUNT16.COUNT.reg = 0;

  TC1->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

  //TC1->COUNT16.INTENSET.reg = TC_INTENSET_MC(1);
  //NVIC_EnableIRQ(TC1_IRQn);
}

void timer_init_tc2(void)
{
  PM->APBCMASK.reg |= PM_APBCMASK_TC2;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC2_GCLK_ID) |
                      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  TC1->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ |
                           TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCSYNC_RESYNC;

  TC1->COUNT16.COUNT.reg = 0;

  TC1->COUNT16.CC[0].reg = (F_CPU / 1000ul / 1024) * 250;
  TC1->COUNT16.COUNT.reg = 0;

  TC1->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

  //TC1->COUNT16.INTENSET.reg = TC_INTENSET_MC(1);
  //NVIC_EnableIRQ(TC1_IRQn);
}
