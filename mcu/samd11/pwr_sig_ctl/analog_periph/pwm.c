/*
 * Copyright (c) 2016, Alex Taradov <alex@taradov.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*- Includes ----------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "samd11.h"
#include "../unused/hal_gpio.h"
#include "pwm.h"

#include "../glue_periph/dma.h"

/*- Definitions -------------------------------------------------------------*/
HAL_GPIO_PIN(PWM_0,   A, 5)

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
void pwm_init(int prescaler, int period)
{
  HAL_GPIO_PWM_0_out();
  HAL_GPIO_PWM_0_pmuxen(HAL_GPIO_PMUX_F);

  PM->APBCMASK.reg |= PM_APBCMASK_TCC0;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TCC0_GCLK_ID) |
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  TCC0->CTRLA.reg = TCC_CTRLA_SWRST;
  while (TCC0->CTRLA.reg & TCC_CTRLA_SWRST);

  TCC0->CTRLA.reg = TCC_CTRLA_PRESCALER_DIV1024 | TCC_CTRLA_PRESCSYNC_PRESC;
  TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;
  TCC0->PER.reg = prescaler;
  TCC0->COUNT.reg = 0;
  TCC0->CC[1].reg = period;
  //TCC0->CC[1].reg = (F_CPU >> 10) * 2 - 20000;
  //TCC0->CC[2].reg = (F_CPU >> 10) * 3;
  // TODo: implement EVSYS PWM => ADC
  //TCC0->EVCTRL.reg = TCC_EVCTRL_MCEO1;
  TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;

  //TCC0->INTENSET.reg = TCC_INTENSET_MC2;
  //NVIC_EnableIRQ(TCC0_IRQn);
}

//-----------------------------------------------------------------------------
void pwm_write(int per, int capture)
{
  TCC0->CTRLA.reg &= ~TCC_CTRLA_ENABLE;
  TCC0->PER.reg = per;
  TCC0->COUNT.reg = 0;
  TCC0->CC[1].reg = capture;
  TCC0->CTRLA.reg |= TCC_CTRLA_ENABLE;
}

void irq_handler_tcc0(void)
{
  TCC0->INTFLAG.reg = TCC_INTFLAG_MC2;
}

