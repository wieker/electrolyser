//
// Created by wieker on 10/15/20.
//

#ifndef SAMD11_COM_USB_STD_CHAPTER6_H
#define SAMD11_COM_USB_STD_CHAPTER6_H

#include "samd11.h"
#include "usb.h"

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

extern udc_mem_t udc_mem[USB_EPT_NUM];
extern uint32_t udc_ctrl_in_buf[16];
extern uint32_t udc_ctrl_out_buf[16];
extern int usb_config;

void udc_reset_endpoint(int ep, int dir);
void usb_handle_standard_request(usb_request_t *request);

#endif //SAMD11_COM_USB_STD_CHAPTER6_H
