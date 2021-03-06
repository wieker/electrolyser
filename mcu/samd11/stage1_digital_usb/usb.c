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
#include <stdbool.h>
#include <string.h>
#include "samd11.h"
#include "usb.h"
#include "utils.h"
#include "hal_gpio.h"
#include "nvm_data.h"
#include "usb_std_chapter6.h"

HAL_GPIO_PIN(USB_DM,   A, 24);
HAL_GPIO_PIN(USB_DP,   A, 25);

udc_mem_t udc_mem[USB_EPT_NUM];
uint32_t udc_ctrl_in_buf[16];
uint32_t udc_ctrl_out_buf[16];

void usb_init(void)
{
  usb_config = 0;

    HAL_GPIO_USB_DM_pmuxen(PORT_PMUX_PMUXE_G_Val);
    HAL_GPIO_USB_DP_pmuxen(PORT_PMUX_PMUXE_G_Val);

    PM->APBBMASK.reg |= PM_APBBMASK_USB;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_ID(USB_GCLK_ID) |
                        GCLK_CLKCTRL_GEN(0);

    USB->DEVICE.CTRLA.bit.SWRST = 1;
    while (USB->DEVICE.SYNCBUSY.bit.SWRST);

    USB->DEVICE.PADCAL.bit.TRANSN = NVM_READ_CAL(USB_TRANSN);
    USB->DEVICE.PADCAL.bit.TRANSP = NVM_READ_CAL(USB_TRANSP);
    USB->DEVICE.PADCAL.bit.TRIM   = NVM_READ_CAL(USB_TRIM);

    memset((uint8_t *)udc_mem, 0, sizeof(udc_mem));
    USB->DEVICE.DESCADD.reg = (uint32_t)udc_mem;

    USB->DEVICE.CTRLA.bit.MODE = USB_CTRLA_MODE_DEVICE_Val;
    USB->DEVICE.CTRLA.bit.RUNSTDBY = 1;
    USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;
    USB->DEVICE.CTRLB.bit.DETACH = 0;

    USB->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_EORST;
    USB->DEVICE.DeviceEndpoint[0].EPINTENSET.bit.RXSTP = 1;

    USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;

    for (int i = 0; i < USB_EPT_NUM; i++)
    {
        udc_reset_endpoint(i, USB_IN_ENDPOINT);
        udc_reset_endpoint(i, USB_OUT_ENDPOINT);
    }

    NVIC_EnableIRQ(USB_IRQn);
}

void irq_handler_usb(void)
{
    int epint, flags;

    if (USB->DEVICE.INTFLAG.bit.EORST)
    {
        USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
        USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN;

        for (int i = 0; i < USB_EPT_NUM; i++)
        {
            udc_reset_endpoint(i, USB_IN_ENDPOINT);
            udc_reset_endpoint(i, USB_OUT_ENDPOINT);
        }

        USB->DEVICE.DeviceEndpoint[0].EPCFG.reg =
                USB_DEVICE_EPCFG_EPTYPE0(USB_DEVICE_EPCFG_EPTYPE_CONTROL) |
                USB_DEVICE_EPCFG_EPTYPE1(USB_DEVICE_EPCFG_EPTYPE_CONTROL);
        USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK0RDY = 1;
        USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.bit.BK1RDY = 1;

        udc_mem[0].in.ADDR.reg = (uint32_t)udc_ctrl_in_buf;
        udc_mem[0].in.PCKSIZE.bit.SIZE = USB_DEVICE_PCKSIZE_SIZE_64;
        udc_mem[0].in.PCKSIZE.bit.BYTE_COUNT = 0;
        udc_mem[0].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;

        udc_mem[0].out.ADDR.reg = (uint32_t)udc_ctrl_out_buf;
        udc_mem[0].out.PCKSIZE.bit.SIZE = USB_DEVICE_PCKSIZE_SIZE_64;
        udc_mem[0].out.PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
        udc_mem[0].out.PCKSIZE.bit.BYTE_COUNT = 0;

        USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.bit.BK0RDY = 1;
        USB->DEVICE.DeviceEndpoint[0].EPINTENSET.bit.RXSTP = 1;
    }

    if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP)
    {
        USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP = 1;
        USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.bit.BK0RDY = 1;

        usb_handle_standard_request((usb_request_t *)udc_ctrl_out_buf);
    }

    epint = USB->DEVICE.EPINTSMRY.reg;

    for (int i = 0; epint && i < USB_EPT_NUM; i++)
    {
        if (0 == (epint & (1 << i)))
            continue;

        epint &= ~(1 << i);

        flags = USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg;

        if (flags & USB_DEVICE_EPINTFLAG_TRCPT0)
        {
            USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.bit.TRCPT0 = 1;
            USB->DEVICE.DeviceEndpoint[i].EPSTATUSSET.bit.BK0RDY = 1;

            usb_recv_callback();
        }

        if (flags & USB_DEVICE_EPINTFLAG_TRCPT1)
        {
            USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.bit.TRCPT1 = 1;
            USB->DEVICE.DeviceEndpoint[i].EPSTATUSCLR.bit.BK1RDY = 1;

            //udc_send_callback(i);
        }
    }
}

//-----------------------------------------------------------------------------
void usb_send(int ep, uint8_t *data, int size)
{
    udc_mem[ep].in.ADDR.reg = (uint32_t)data;
    udc_mem[ep].in.PCKSIZE.bit.BYTE_COUNT = size;
    udc_mem[ep].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;

    USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.bit.BK1RDY = 1;
}

//-----------------------------------------------------------------------------
void usb_recv(int ep, uint8_t *data, int size)
{
    udc_mem[ep].out.ADDR.reg = (uint32_t)data;
    udc_mem[ep].out.PCKSIZE.bit.MULTI_PACKET_SIZE = size;
    udc_mem[ep].out.PCKSIZE.bit.BYTE_COUNT = 0;

    USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.BK0RDY = 1;
}

