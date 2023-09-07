#include <nrf_delay.h>
#include <nrf_esb.h>

#include <nrf_local_adc.h>

#include "local_nrf_esb.h"

#include <legacy/nrf_drv_uart.h>
#include "ble_config/sdk_config.h"

extern nrf_drv_uart_t m_uart;

void uart_init();
char dt[100];
uint8_t rxx[10];

int strlen(char* string) {
  int i = 0;
  for (; *string; i ++) {
    string ++;
  }
  return i;
}

void cpy(uint8_t* string, char* dest) {
  int i = 0;
  do {
    *dest ++ = *string;
    i ++;
  } while (*string ++ && i < 100);
}

void cpyN(int n, uint8_t* string, char* dest) {
  int i = 0;
  while (i < n) {
    *dest ++ = *string ++;
    i ++;
  }
}

void advertise() {
  // char data[5];
  // nrf_saadc_value_t vV = m_buffer_pool[0][0];
  // nrf_saadc_value_t vI = m_buffer_pool[0][1];
  // data[0] = (vV / 16) < 10 ? (vV / 16) + '0' : (vV / 16) + '7';
  // data[1] = (vV % 16) < 10 ? (vV % 16) + '0' : (vV % 16) + '7';
  // data[2] = (vI / 16) < 10 ? (vI / 16) + '0' : (vI / 16) + '7';
  // data[3] = (vI % 16) < 10 ? (vI % 16) + '0' : (vI % 16) + '7';
  // data[4] = '\n';
  radio_packet_send((uint8_t *) dt, strlen(dt));
  //nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "txt\n", 4);
}

void radio_packet_recv(uint8_t *packet, uint32_t packet_length) {
  nrfx_uart_tx(&m_uart.uart, packet, packet_length);
  //cpy(packet, dt);
}

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20 | 1 << 30 | 1 << 31;
  uart_init();

  radio_init(1);

  NRF_P0->OUTSET = 1 << 30;


  //saadc_init();
  //saadc_sampling_event_init();
  //saadc_sampling_event_enable();

  while (1) {
    NRF_P0->OUTSET = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    NRF_P0->OUTSET = 1 << 31;
    nrf_delay_ms(1000);

    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 31;
    nrf_delay_ms(1000);

    //nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);

    advertise();
  }
}

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void evh(nrfx_uart_event_t const * p_event,
         void *                    p_context)
{
    if (p_event->type == 1) {
      cpyN(p_event->data.rxtx.bytes, p_event->data.rxtx.p_data, dt);
      dt[p_event->data.rxtx.bytes] = 0;
      advertise();
    }
  nrfx_uart_rx(&m_uart.uart, rxx, 1);
}

void uart_init()
{
  nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
  config.pseltxd  = 28;
  config.pselrxd  = 29;
  config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
  config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
  config.baudrate = (nrf_uart_baudrate_t)NRFX_UART_DEFAULT_CONFIG_BAUDRATE;
  nrfx_uart_init(&m_uart.uart,
                 (nrfx_uart_config_t const *)&config,
                 evh);
  nrfx_uart_rx(&m_uart.uart, rxx, 1);
}

