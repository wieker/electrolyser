#include <nrf_delay.h>
#include "ble_config/sdk_config.h"
#include <legacy/nrf_drv_uart.h>

extern nrf_drv_uart_t m_uart;
uint8_t rxx[10];

void uart_init();

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
  uart_init();

  while (1) {
    NRF_P0->OUTSET = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    nrf_delay_ms(100);

    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
    nrf_delay_ms(100);
  }
}

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void evh(nrfx_uart_event_t const * p_event,
         void *                    p_context)
{
    if (p_event->type == 1) {
      if (p_event->data.rxtx.bytes < 100) {
        (void) p_event->data.rxtx.p_data;
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
  nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "init00\r\n", 8);
  nrfx_uart_rx_enable(&m_uart.uart);
  nrfx_uart_rx(&m_uart.uart, rxx, 1);
}

