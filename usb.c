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
#include "usb_descriptors.h"
#include "nvm_data.h"

HAL_GPIO_PIN(USB_DM,   A, 24);
HAL_GPIO_PIN(USB_DP,   A, 25);


#define USB_CMD(dir, rcpt, type, cmd) \
    ((USB_##cmd << 8) | (USB_##dir##_TRANSFER << 7) | (USB_##type##_REQUEST << 5) | (USB_##rcpt##_RECIPIENT << 0))

typedef struct
{
  void         (*callback)(void);
} usb_endpoint_bank_t;

typedef struct
{
  usb_endpoint_bank_t out;
  usb_endpoint_bank_t in;
} usb_endpoint_t;

typedef union
{
    UsbDeviceDescBank    bank[2];
    struct
    {
        UsbDeviceDescBank  out;
        UsbDeviceDescBank  in;
    };
} udc_mem_t;

enum
{
    USB_DEVICE_PCKSIZE_SIZE_8    = 0,
    USB_DEVICE_PCKSIZE_SIZE_16   = 1,
    USB_DEVICE_PCKSIZE_SIZE_32   = 2,
    USB_DEVICE_PCKSIZE_SIZE_64   = 3,
    USB_DEVICE_PCKSIZE_SIZE_128  = 4,
    USB_DEVICE_PCKSIZE_SIZE_256  = 5,
    USB_DEVICE_PCKSIZE_SIZE_512  = 6,
    USB_DEVICE_PCKSIZE_SIZE_1023 = 7,
};

static usb_endpoint_t usb_endpoints[8];
static int usb_config;

static udc_mem_t udc_mem[USB_EPT_NUM];
static uint32_t udc_ctrl_in_buf[16];
static uint32_t udc_ctrl_out_buf[16];

static bool udc_endpoint_configured(int ep, int dir)
{
    if (USB_IN_ENDPOINT == dir)
        return (USB_DEVICE_EPCFG_EPTYPE_DISABLED != USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE1);
    else
        return (USB_DEVICE_EPCFG_EPTYPE_DISABLED != USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE0);
}

static int udc_endpoint_get_status(int ep, int dir)
{
    if (USB_IN_ENDPOINT == dir)
        return USB->DEVICE.DeviceEndpoint[ep].EPSTATUS.bit.STALLRQ1;
    else
        return USB->DEVICE.DeviceEndpoint[ep].EPSTATUS.bit.STALLRQ0;
}

static void udc_endpoint_set_feature(int ep, int dir)
{
    if (USB_IN_ENDPOINT == dir)
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.bit.STALLRQ1 = 1;
    else
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.bit.STALLRQ0 = 1;
}

//-----------------------------------------------------------------------------
static void udc_endpoint_clear_feature(int ep, int dir)
{
    if (USB_IN_ENDPOINT == dir)
    {
        if (USB->DEVICE.DeviceEndpoint[ep].EPSTATUS.bit.STALLRQ1)
        {
            USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.STALLRQ1 = 1;

            if (USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.STALL1)
            {
                USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.STALL1 = 1;
                USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.DTGLIN = 1;
            }
        }
    }
    else
    {
        if (USB->DEVICE.DeviceEndpoint[ep].EPSTATUS.bit.STALLRQ0)
        {
            USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.STALLRQ0 = 1;

            if (USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.STALL0)
            {
                USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.STALL0 = 1;
                USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.DTGLOUT = 1;
            }
        }
    }
}

//-----------------------------------------------------------------------------
static void udc_set_address(int address)
{
    USB->DEVICE.DADD.reg = USB_DEVICE_DADD_ADDEN | USB_DEVICE_DADD_DADD(address);
}

//-----------------------------------------------------------------------------
static void udc_control_send_zlp(void)
{
    udc_mem[0].in.PCKSIZE.bit.BYTE_COUNT = 0;
    USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1 = 1;
    USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK1RDY = 1;

    while (0 == USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1);
}

//-----------------------------------------------------------------------------
static void udc_control_stall(void)
{
    USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.STALLRQ1 = 1;
}

static void udc_control_send(uint8_t *data, int size)
{
    if (size <= usb_device_descriptor.bMaxPacketSize0)
    {
        // Small payloads may be unaligned, so copy them into an aligned buffer
        memcpy(udc_ctrl_in_buf, data, size);
        udc_mem[0].in.ADDR.reg = (uint32_t)udc_ctrl_in_buf;
    }
    else
    {
        // Large payloads must be aligned
        udc_mem[0].in.ADDR.reg = (uint32_t)data;
    }

    udc_mem[0].in.PCKSIZE.bit.BYTE_COUNT = size;
    udc_mem[0].in.PCKSIZE.bit.MULTI_PACKET_SIZE = 0;

    USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1 = 1;
    USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.bit.BK1RDY = 1;

    while (0 == USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1);
}

static void udc_reset_endpoint(int ep, int dir)
{
    if (USB_IN_ENDPOINT == dir)
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE1 = USB_DEVICE_EPCFG_EPTYPE_DISABLED;
    else
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE0 = USB_DEVICE_EPCFG_EPTYPE_DISABLED;
}

static void udc_configure_endpoint(usb_endpoint_descriptor_t *desc)
{
    int ep, dir, type, size;

    ep = desc->bEndpointAddress & USB_INDEX_MASK;
    dir = desc->bEndpointAddress & USB_DIRECTION_MASK;
    type = desc->bmAttributes & 0x03;
    size = desc->wMaxPacketSize & 0x3ff;

    udc_reset_endpoint(ep, dir);

    if (size <= 8)
        size = USB_DEVICE_PCKSIZE_SIZE_8;
    else if (size <= 16)
        size = USB_DEVICE_PCKSIZE_SIZE_16;
    else if (size <= 32)
        size = USB_DEVICE_PCKSIZE_SIZE_32;
    else if (size <= 64)
        size = USB_DEVICE_PCKSIZE_SIZE_64;
    else if (size <= 128)
        size = USB_DEVICE_PCKSIZE_SIZE_128;
    else if (size <= 256)
        size = USB_DEVICE_PCKSIZE_SIZE_256;
    else if (size <= 512)
        size = USB_DEVICE_PCKSIZE_SIZE_512;
    else if (size <= 1023)
        size = USB_DEVICE_PCKSIZE_SIZE_1023;
    else
        while (1);

    if (USB_CONTROL_ENDPOINT == type)
        type = USB_DEVICE_EPCFG_EPTYPE_CONTROL;
    else if (USB_ISOCHRONOUS_ENDPOINT == type)
        type = USB_DEVICE_EPCFG_EPTYPE_ISOCHRONOUS;
    else if (USB_BULK_ENDPOINT == type)
        type = USB_DEVICE_EPCFG_EPTYPE_BULK;
    else
        type = USB_DEVICE_EPCFG_EPTYPE_INTERRUPT;

    if (USB_IN_ENDPOINT == dir)
    {
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE1 = type;
        USB->DEVICE.DeviceEndpoint[ep].EPINTENSET.bit.TRCPT1 = 1;
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.DTGLIN = 1;
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.BK1RDY = 1;
        udc_mem[ep].in.PCKSIZE.bit.SIZE = size;
    }
    else
    {
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE0 = type;
        USB->DEVICE.DeviceEndpoint[ep].EPINTENSET.bit.TRCPT0 = 1;
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSCLR.bit.DTGLOUT = 1;
        USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.bit.BK0RDY = 1;
        udc_mem[ep].out.PCKSIZE.bit.SIZE = size;
    }
}

static void usb_handle_standard_request(usb_request_t *request)
{
    switch ((request->bRequest << 8) | request->bmRequestType)
    {
        case USB_CMD(IN, DEVICE, STANDARD, GET_DESCRIPTOR):
        {
            uint8_t type = request->wValue >> 8;
            uint8_t index = request->wValue & 0xff;
            uint16_t length = request->wLength;

            if (USB_DEVICE_DESCRIPTOR == type)
            {
                length = LIMIT(length, usb_device_descriptor.bLength);

                udc_control_send((uint8_t *)&usb_device_descriptor, length);
            }
            else if (USB_CONFIGURATION_DESCRIPTOR == type)
            {
                length = LIMIT(length, usb_configuration_hierarchy.configuration.wTotalLength);

                udc_control_send((uint8_t *)&usb_configuration_hierarchy, length);
            }
            else if (USB_STRING_DESCRIPTOR == type)
            {
                if (0 == index)
                {
                    length = LIMIT(length, usb_string_descriptor_zero.bLength);

                    udc_control_send((uint8_t *)&usb_string_descriptor_zero, length);
                }
                else if (index < USB_STR_COUNT)
                {
                    char *str = usb_strings[index];
                    int len = strlen(str);

                    memset(usb_string_descriptor_buffer, 0, sizeof(usb_string_descriptor_buffer));

                    usb_string_descriptor_buffer[0] = len*2 + 2;
                    usb_string_descriptor_buffer[1] = USB_STRING_DESCRIPTOR;

                    for (int i = 0; i < len; i++)
                        usb_string_descriptor_buffer[2 + i*2] = str[i];

                    length = LIMIT(length, usb_string_descriptor_buffer[0]);

                    udc_control_send(usb_string_descriptor_buffer, length);
                }
                else
                {
                    udc_control_stall();
                }
            }
            else
                udc_control_stall();
        }  break;

        case USB_CMD(OUT, DEVICE, STANDARD, SET_ADDRESS):
        {
            udc_control_send_zlp();
            udc_set_address(request->wValue);
        } break;

        case USB_CMD(OUT, DEVICE, STANDARD, SET_CONFIGURATION):
        {
            usb_config = request->wValue;

            udc_control_send_zlp();

            if (usb_config)
            {
                int size = usb_configuration_hierarchy.configuration.wTotalLength;
                usb_descriptor_header_t *desc = (usb_descriptor_header_t *)&usb_configuration_hierarchy;

                while (size)
                {
                    if (USB_ENDPOINT_DESCRIPTOR == desc->bDescriptorType)
                        udc_configure_endpoint((usb_endpoint_descriptor_t *)desc);

                    size -= desc->bLength;
                    desc = (usb_descriptor_header_t *)((uint8_t *)desc + desc->bLength);
                }

                //usb_configuration_callback(usb_config);
            }
        } break;

        case USB_CMD(IN, DEVICE, STANDARD, GET_CONFIGURATION):
        {
            uint8_t config = usb_config;
            udc_control_send(&config, sizeof(config));
        } break;

        case USB_CMD(IN, DEVICE, STANDARD, GET_STATUS):
        case USB_CMD(IN, INTERFACE, STANDARD, GET_STATUS):
        {
            uint16_t status = 0;
            udc_control_send((uint8_t *)&status, sizeof(status));
        } break;

        case USB_CMD(IN, ENDPOINT, STANDARD, GET_STATUS):
        {
            int ep = request->wIndex & USB_INDEX_MASK;
            int dir = request->wIndex & USB_DIRECTION_MASK;
            uint16_t status = 0;

            if (udc_endpoint_configured(ep, dir))
            {
                status = udc_endpoint_get_status(ep, dir);
                udc_control_send((uint8_t *)&status, sizeof(status));
            }
            else
            {
                udc_control_stall();
            }
        } break;

        case USB_CMD(OUT, DEVICE, STANDARD, SET_FEATURE):
        {
            udc_control_stall();
        } break;

        case USB_CMD(OUT, INTERFACE, STANDARD, SET_FEATURE):
        {
            udc_control_send_zlp();
        } break;

        case USB_CMD(OUT, ENDPOINT, STANDARD, SET_FEATURE):
        {
            int ep = request->wIndex & USB_INDEX_MASK;
            int dir = request->wIndex & USB_DIRECTION_MASK;

            if (0 == request->wValue && ep && udc_endpoint_configured(ep, dir))
            {
                udc_endpoint_set_feature(ep, dir);
                udc_control_send_zlp();
            }
            else
            {
                udc_control_stall();
            }
        } break;

        case USB_CMD(OUT, DEVICE, STANDARD, CLEAR_FEATURE):
        {
            udc_control_stall();
        } break;

        case USB_CMD(OUT, INTERFACE, STANDARD, CLEAR_FEATURE):
        {
            udc_control_send_zlp();
        } break;

        case USB_CMD(OUT, ENDPOINT, STANDARD, CLEAR_FEATURE):
        {
            int ep = request->wIndex & USB_INDEX_MASK;
            int dir = request->wIndex & USB_DIRECTION_MASK;

            if (0 == request->wValue && ep && udc_endpoint_configured(ep, dir))
            {
                udc_endpoint_clear_feature(ep, dir);
                udc_control_send_zlp();
            }
            else
            {
                udc_control_stall();
            }
        } break;

            // TODO: This actually belongs to HID implementation
        case USB_CMD(IN, INTERFACE, STANDARD, GET_DESCRIPTOR):
        {
            uint16_t length = request->wLength;

            length = LIMIT(length, sizeof(usb_hid_report_descriptor));

            udc_control_send(usb_hid_report_descriptor, length);
        } break;

        default:
        {
            udc_control_stall();
        } break;
    }
}

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

            //udc_recv_callback(i);
        }

        if (flags & USB_DEVICE_EPINTFLAG_TRCPT1)
        {
            USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.bit.TRCPT1 = 1;
            USB->DEVICE.DeviceEndpoint[i].EPSTATUSCLR.bit.BK1RDY = 1;

            //udc_send_callback(i);
        }
    }
}

