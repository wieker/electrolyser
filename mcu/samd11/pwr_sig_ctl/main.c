/*
 * Copyright (c) 2016-2017, Alex Taradov <alex@taradov.com>
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
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
#include "samd11.h"
#include "hal_gpio.h"
#include "nvm_data.h"
#include "gpio.h"
#include "usb.h"
#include "pwm.h"
#include "adc.h"

#include "dma.h"
#include "timer.h"
#include "evsys.h"

/*- Definitions -------------------------------------------------------------*/
HAL_GPIO_PIN(LED,      A, 14)

#define APP_EP_SEND    1
#define APP_EP_RECV    2

/*- Variables ---------------------------------------------------------------*/
static alignas(4) uint8_t app_usb_recv_buffer[64];
alignas(4) uint8_t app_response_buffer[64];

/*- Implementations ---------------------------------------------------------*/

static void sys_init(void)
{
  uint32_t coarse, fine;

  SYSCTRL->OSC8M.bit.PRESC = 0;

  SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
      SYSCTRL_INTFLAG_DFLLRDY;

  NVMCTRL->CTRLB.bit.RWS = 2;

  coarse = NVM_READ_CAL(DFLL48M_COARSE_CAL);
  fine = NVM_READ_CAL(DFLL48M_FINE_CAL);

  SYSCTRL->DFLLCTRL.reg = 0; // See Errata 9905
  while (0 == (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY));

  SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_MUL(48000);
  SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine);

  SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE | SYSCTRL_DFLLCTRL_USBCRM |
      SYSCTRL_DFLLCTRL_BPLCKC | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_MODE;

  while (0 == (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY));

  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC(GCLK_SOURCE_DFLL48M) |
      GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

static uint32_t get_uint32(uint8_t *data)
{
  return ((uint32_t)data[3] << 0) | ((uint32_t)data[2] << 8) |
         ((uint32_t)data[1] << 16) | ((uint32_t)data[0] << 24);
}

static uint32_t get_uint16(uint8_t *data)
{
  return ((uint16_t)data[0] << 0) | ((uint16_t)data[1] << 8);
}

static void set_uint32(uint8_t *data, uint32_t value)
{
  data[3] = (value >> 0) & 0xff;
  data[2] = (value >> 8) & 0xff;
  data[1] = (value >> 16) & 0xff;
  data[0] = (value >> 24) & 0xff;
}

static void set_uint16(uint8_t *data, uint16_t value)
{
  data[0] = (value >> 0) & 0xff;
  data[1] = (value >> 8) & 0xff;
}

static alignas(4) uint8_t app_recv_buffer[64];

//-----------------------------------------------------------------------------
void usb_recv_callback(void)
{
  //pwm_write((F_CPU / 1000ul / 1024) * 250 * app_usb_recv_buffer[0]);

  //dma_start(DMAC_CHANNEL_0, (const void *) &ADC->RESULT.reg, app_response_buffer, 64);
  //int voltage = getV();
  //set_uint16(app_response_buffer, voltage);

  //usb_send(APP_EP_SEND, app_response_buffer, sizeof(app_response_buffer));

  //usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

void usb_configure_callback() {
  usb_recv(APP_EP_RECV, app_recv_buffer, sizeof(app_recv_buffer));
}

void dma_complete_cb() {
  usb_send(APP_EP_SEND, app_response_buffer, sizeof(app_response_buffer));
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

int main(void)
{
  sys_init();
  timer_init();
  evsys_init();

  usb_init();
  adc_init();
  //gpio_init();

  dma_init();
  dma_start();
  pwm_init(0, 10);
  adc_read();

  //HAL_GPIO_LED_out();
  //HAL_GPIO_LED_clr();

  while (1)
  {
  }

  return 0;
}

