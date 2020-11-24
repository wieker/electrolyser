//
// Created by wieker on 6/10/20.
//

#ifndef SAMD11_SWD_DGW_H
#define SAMD11_SWD_DGW_H

#include <stdint.h>

enum
{
    CMD_I2C_INIT     = 0x00,
    CMD_I2C_START    = 0x01,
    CMD_I2C_STOP     = 0x02,
    CMD_I2C_READ     = 0x03,
    CMD_I2C_WRITE    = 0x04,
    CMD_I2C_PINS     = 0x05,

    CMD_SPI_INIT     = 0x10,
    CMD_SPI_SS       = 0x11,
    CMD_SPI_TRANSFER = 0x12,

    CMD_GPIO_CONFIG  = 0x50,
    CMD_GPIO_READ    = 0x51,
    CMD_GPIO_WRITE   = 0x52,

    CMD_DAC_INIT     = 0x60,
    CMD_DAC_WRITE    = 0x61,

    CMD_ADC_INIT     = 0x70,
    CMD_ADC_READ     = 0x71,

    CMD_PWM_INIT     = 0x80,
    CMD_PWM_WRITE    = 0x81,

    CMD_GET_VERSION  = 0xf0,
};

#define APP_MAGIC      0x78656c41
#define APP_VERSION    1

void usb_dgw_process(uint8_t* app_usb_recv_buffer,
                     uint8_t* app_response_buffer);

#endif //SAMD11_SWD_DGW_H
