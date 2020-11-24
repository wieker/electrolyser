//
// Created by wieker on 6/10/20.
//

#include "dgw.h"
#include "gpio.h"
#include "spi_master.h"

static uint32_t get_uint32(uint8_t *data)
{
    return ((uint32_t)data[0] << 0) | ((uint32_t)data[1] << 8) |
           ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

static void set_uint32(uint8_t *data, uint32_t value)
{
    data[0] = (value >> 0) & 0xff;
    data[1] = (value >> 8) & 0xff;
    data[2] = (value >> 16) & 0xff;
    data[3] = (value >> 24) & 0xff;
}

void usb_dgw_process(uint8_t* app_request_buffer,
                     uint8_t* app_response_buffer)
{
    int cmd = app_request_buffer[0];

    if (CMD_SPI_INIT == cmd)
    {
        int freq = get_uint32(&app_request_buffer[1]);
        int mode = app_request_buffer[5];
        freq = spi_init(freq, mode);
        set_uint32(&app_response_buffer[2], freq);
    }
    else if (CMD_SPI_SS == cmd)
    {
        spi_ss(app_request_buffer[1]);
    }
    else if (CMD_SPI_TRANSFER == cmd)
    {
        for (int i = 0; i < app_request_buffer[1]; i++)
        {
            app_response_buffer[2 + i] = spi_write_byte(app_request_buffer[2 + i]);
        }
    }

    else if (CMD_GPIO_CONFIG == cmd)
    {
        int cnt = app_request_buffer[1];

        for (int i = 0; i < cnt; i++)
        {
            int index = app_request_buffer[2 + i*2];
            int conf = app_request_buffer[3 + i*2];

            gpio_configure(index, conf);
        }
    }
    else if (CMD_GPIO_READ == cmd)
    {
        int cnt = app_request_buffer[1];

        for (int i = 0; i < cnt; i++)
        {
            int index = app_request_buffer[2 + i];

            app_response_buffer[2 + i] = gpio_read(index);
        }
    }
    else if (CMD_GPIO_WRITE == cmd)
    {
        int cnt = app_request_buffer[1];

        for (int i = 0; i < cnt; i++)
        {
            int index = app_request_buffer[2 + i*2];
            int value = app_request_buffer[3 + i*2];

            gpio_write(index, value);
        }
    }
}
