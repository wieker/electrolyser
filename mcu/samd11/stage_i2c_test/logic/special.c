//
// Created by wieker on 3/23/21.
//

#include "special.h"

#include <stdint.h>
#include <stdalign.h>
#include "samd11.h"
#include "std_hal/hal_gpio.h"
#include "gpio.h"
#include "usb_ifc/usb.h"

#include "spi_master.h"
#include "uart.h"
#include "i2c_master.h"


HAL_GPIO_PIN(LED,      A, 14)
#define APP_EP_SEND    1
#define APP_EP_RECV    2

int led_state = 1;
int rst_state = 0;

static alignas(4) uint8_t app_usb_recv_buffer[64];
alignas(4) uint8_t app_response_buffer[64];

uint8_t data_wake[] = { 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t data[] = { 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 };

enum {
    CMD_I2C_INIT = 0x00,
    CMD_I2C_START = 0x01,
    CMD_I2C_STOP = 0x02,
    CMD_I2C_READ = 0x03,
    CMD_I2C_WRITE = 0x04,
    CMD_I2C_PINS = 0x05,
};

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

void usb_recv_callback(void)
{
  int cmd = app_usb_recv_buffer[0];
  app_response_buffer[0] = true;
  app_response_buffer[1] = cmd;

  if (CMD_I2C_INIT == cmd)
  {
    int freq = get_uint32(&app_usb_recv_buffer[1]);
    freq = i2c_init(100000);
    set_uint32(&app_response_buffer[2], freq);
  }
  else if (CMD_I2C_START == cmd)
  {
    int addr = app_usb_recv_buffer[2];
    app_response_buffer[0] = i2c_start(addr);
  }
  else if (CMD_I2C_STOP == cmd)
  {
    app_response_buffer[0] = i2c_stop();
  }
  else if (CMD_I2C_READ == cmd)
  {
    int i;

    for (i = 0; i < app_usb_recv_buffer[1]; i++)
    {
      bool last = ((i == (app_usb_recv_buffer[1] - 1)) && app_usb_recv_buffer[2]);

      if (!i2c_read_byte(&app_response_buffer[2 + i], last))
      {
        app_response_buffer[0] = false;
        break;
      }
    }

    app_response_buffer[1] = i;
  }
  else if (CMD_I2C_WRITE == cmd)
  {
    int i;

    for (i = 0; i < app_usb_recv_buffer[1]; i++)
    {
      if (!i2c_write_byte(app_usb_recv_buffer[2 + i]))
      {
        app_response_buffer[0] = false;
        break;
      }
    }

    app_response_buffer[1] = i;
  }
  if (CMD_I2C_PINS == cmd)
  {
    i2c_pins(app_usb_recv_buffer[1], app_usb_recv_buffer[2]);
  }

    usb_send(APP_EP_SEND, app_response_buffer, sizeof(app_response_buffer));
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

void usb_configure_callback() {
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

unsigned char read_cmd[] = {0x0B, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void init_device() {
  int state = 0;
  spi_init(8000000, 0);
  spi_ss(0);

  for (int i = 0; i < 8; i ++) {
    app_response_buffer[i] = spi_write_byte(data_wake[i]);
  }
  spi_ss(1);
  for (int i = 0; i < 64; i ++) {}
  spi_ss(0);
  for (int i = 0; i < 8; i ++) {
    app_response_buffer[i] = spi_write_byte(read_cmd[i]);
  }
  if (app_response_buffer[5] != 0xFF) {
    state = 0;
  } else {
    state = 1;
  }

  spi_ss(1);
  spi_deinit();

  rst_state = state;
  gpio_write(GPIO_RST, rst_state);

  if (state == 1) {
    for (int i = 0; i < 500000000; i++) {}
    gpio_write(GPIO_CTL1, 0);
    gpio_write(GPIO_CTL2, 1);
  }
}
