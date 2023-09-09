#include <nrf_delay.h>
#include <nrf_esb.h>

#include "local_nrf_esb.h"
#include <legacy/nrf_drv_uart.h>
#include "ble_config/sdk_config.h"

extern nrf_drv_uart_t m_uart;

void uart_init();
char dt[100];
uint8_t rxx[10];
int dtpos = 2;
int v = 0;

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
  if (dtpos > 0) {
    dt[1] = '0' + v % 10;
    v ++;
    radio_packet_send((uint8_t *) dt, dtpos);
    dtpos = 2;
    dt[2] = 0;
  }
  //nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "txt\n", 4);
}

void radio_packet_recv(uint8_t *packet, uint32_t packet_length) {
  nrfx_uart_tx(&m_uart.uart, packet + 2, packet_length - 2);
}

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
  uart_init();

  radio_init(1);
  dt[0] = 'p';
  dt[1] = 'd';

  while (1) {
    NRF_P0->OUTSET = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    nrf_delay_ms(100);

    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
    nrf_delay_ms(100);

    advertise();
  }
}

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void evh(nrfx_uart_event_t const * p_event,
         void *                    p_context)
{
    if (p_event->type == 1) {
      if (dtpos + p_event->data.rxtx.bytes < 100) {
        cpyN(p_event->data.rxtx.bytes, p_event->data.rxtx.p_data, dt + dtpos);
        dt[p_event->data.rxtx.bytes + dtpos] = 0;
        dtpos += p_event->data.rxtx.bytes;
      }
    }
  nrfx_uart_rx(&m_uart.uart, rxx, 1);
}

void uart_init()
{
  nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
  config.pseltxd  = 6;
  config.pselrxd  = 8;
  config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
  config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
  config.baudrate = (nrf_uart_baudrate_t)NRFX_UART_DEFAULT_CONFIG_BAUDRATE;
  nrfx_uart_init(&m_uart.uart,
                 (nrfx_uart_config_t const *)&config,
                 evh);
  //nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "init00\r\n", 8);
  nrfx_uart_rx_enable(&m_uart.uart);
  nrfx_uart_rx(&m_uart.uart, rxx, 1);
}

