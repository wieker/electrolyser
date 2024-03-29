//
// Created by wieker on 3/23/21.
//

#include "special.h"

#include <stdint.h>
#include <stdalign.h>
#include <analog_periph/adc.h>
#include <analog_periph/pwm.h>
#include "samd11.h"
#include "unused/hal_gpio.h"
#include "gpio.h"
#include "usb_ifc/usb.h"
#include "../glue_periph/dma.h"


HAL_GPIO_PIN(LED,      A, 14)
#define APP_EP_SEND    1
#define APP_EP_RECV    2

int led_state = 1;
int rst_state = 0;

static alignas(4) uint8_t app_usb_recv_buffer[64];
alignas(4) uint8_t app_response_buffer[64];

uint8_t data_wake[] = { 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t data[] = { 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 };

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

volatile int lock = 1;

void usb_recv_callback(void)
{
  if (app_usb_recv_buffer[0] == 0) {
    led_state = !led_state;
    gpio_write(GPIO_LED, led_state);
  }
  if (app_usb_recv_buffer[0] == 1) {
    led_state = !led_state;
    gpio_write(GPIO_LED, led_state);
    dma_descrs();
    dma_start();
    return;
    //app_response_buffer[0] = adc_read_polling();
  }
  if (app_usb_recv_buffer[0] == 2) {
    pwm_write(app_usb_recv_buffer[2], app_usb_recv_buffer[3]);
  }
  if (app_usb_recv_buffer[0] == 3) {
    if (lock == 1) {
      return;
    }
    lock = 1;
  }
  if (app_usb_recv_buffer[0] == 4) {
    /*for (int i = 0; i < app_usb_recv_buffer[1]; i ++) {
      app_response_buffer[i] = adc_read_polling();
    }*/
  }
  if (app_usb_recv_buffer[0] == 5) {
    adc_configure(app_usb_recv_buffer[2], get_uint32(app_usb_recv_buffer + 6), get_uint32(app_usb_recv_buffer + 10));
  }


  usb_send(APP_EP_SEND, app_response_buffer, sizeof(app_response_buffer));
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

void usb_configure_callback() {
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}

void dma_complete_cb() {
  lock = 0;
  usb_send(APP_EP_SEND, app_response_buffer, sizeof(app_response_buffer));
  usb_recv(APP_EP_RECV, app_usb_recv_buffer, sizeof(app_usb_recv_buffer));
}
