/*
 * Copyright (c) 2017, Alex Taradov <alex@taradov.com>
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "samd11.h"
#include "std_hal/hal_gpio.h"
#include "uart.h"

/*- Definitions -------------------------------------------------------------*/
#define UART_BUF_SIZE            256

HAL_GPIO_PIN(UART_TX,            A, 8);
HAL_GPIO_PIN(UART_RX,            A, 9);

#define UART_SERCOM              SERCOM1
#define UART_SERCOM_PMUX         PORT_PMUX_PMUXE_C_Val
#define UART_SERCOM_GCLK_ID      SERCOM1_GCLK_ID_CORE
#define UART_SERCOM_APBCMASK     PM_APBCMASK_SERCOM1
#define UART_SERCOM_IRQ_INDEX    SERCOM1_IRQn
#define UART_SERCOM_IRQ_HANDLER  irq_handler_sercom1
#define UART_SERCOM_TXPO         1
#define UART_SERCOM_RXPO         3

/*- Types ------------------------------------------------------------------*/
typedef struct
{
  int       wr;
  int       rd;
  uint8_t   data[UART_BUF_SIZE];
} fifo_buffer_t;

/*- Variables --------------------------------------------------------------*/
static volatile fifo_buffer_t uart_rx_fifo;
static volatile fifo_buffer_t uart_tx_fifo;

/*- Implementations ---------------------------------------------------------*/

//-----------------------------------------------------------------------------
void uart_init()
{
  int chsize, form, pmode, sbmode, baud, fp;

  HAL_GPIO_UART_TX_pmuxen(UART_SERCOM_PMUX);

  HAL_GPIO_UART_RX_pullen(1);
  HAL_GPIO_UART_RX_pmuxen(UART_SERCOM_PMUX);

  PM->APBCMASK.reg |= UART_SERCOM_APBCMASK;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(UART_SERCOM_GCLK_ID) |
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

  UART_SERCOM->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
  while (UART_SERCOM->USART.CTRLA.bit.SWRST);

  uart_tx_fifo.wr = 0;
  uart_tx_fifo.rd = 0;

  uart_rx_fifo.wr = 0;
  uart_rx_fifo.rd = 0;

  baud = F_CPU / (16 * 1000000);
  fp = (F_CPU / 1000000 - 16 * baud) / 2;

  UART_SERCOM->USART.CTRLA.reg =
      SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
      SERCOM_USART_CTRLA_SAMPR(1) |
      SERCOM_USART_CTRLA_RXPO(UART_SERCOM_RXPO) |
      SERCOM_USART_CTRLA_TXPO(UART_SERCOM_TXPO);

  UART_SERCOM->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN;

  UART_SERCOM->USART.BAUD.reg =
      SERCOM_USART_BAUD_FRACFP_BAUD(baud) | SERCOM_USART_BAUD_FRACFP_FP(fp);

  UART_SERCOM->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;

  UART_SERCOM->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;

  NVIC_EnableIRQ(UART_SERCOM_IRQ_INDEX);
}

//-----------------------------------------------------------------------------
bool uart_write_byte(int byte)
{
  int wr = (uart_tx_fifo.wr + 1) % UART_BUF_SIZE;
  bool res = false;

  NVIC_DisableIRQ(UART_SERCOM_IRQ_INDEX);

  if (wr != uart_tx_fifo.rd)
  {
    uart_tx_fifo.data[uart_tx_fifo.wr] = byte;
    uart_tx_fifo.wr = wr;
    res = true;

    UART_SERCOM->USART.INTENSET.reg = SERCOM_USART_INTENSET_DRE;
  }

  NVIC_EnableIRQ(UART_SERCOM_IRQ_INDEX);

  return res;
}

//-----------------------------------------------------------------------------
bool uart_read_byte(uint8_t *byte)
{
  bool res = false;

  NVIC_DisableIRQ(UART_SERCOM_IRQ_INDEX);

  if (uart_rx_fifo.rd != uart_rx_fifo.wr)
  {
    *byte = uart_rx_fifo.data[uart_rx_fifo.rd];
    uart_rx_fifo.rd = (uart_rx_fifo.rd + 1) % UART_BUF_SIZE;
    res = true;

    NVIC_EnableIRQ(UART_SERCOM_IRQ_INDEX);
    return res;
  }

  NVIC_EnableIRQ(UART_SERCOM_IRQ_INDEX);

  return res;
}

//-----------------------------------------------------------------------------
void UART_SERCOM_IRQ_HANDLER(void)
{
  int flags = UART_SERCOM->USART.INTFLAG.reg;

  if (flags & SERCOM_USART_INTFLAG_RXC)
  {
    int byte = UART_SERCOM->USART.DATA.reg;
    int wr = (uart_rx_fifo.wr + 1) % UART_BUF_SIZE;
    uart_rx_fifo.data[uart_rx_fifo.wr] = byte;
    uart_rx_fifo.wr = wr;
  }

  if (flags & SERCOM_USART_INTFLAG_DRE)
  {
    if (uart_tx_fifo.rd == uart_tx_fifo.wr)
    {
      UART_SERCOM->USART.INTENCLR.reg = SERCOM_USART_INTENCLR_DRE;
    }
    else
    {
      UART_SERCOM->USART.DATA.reg = uart_tx_fifo.data[uart_tx_fifo.rd];
      uart_tx_fifo.rd = (uart_tx_fifo.rd + 1) % UART_BUF_SIZE;
    }
  }
}

