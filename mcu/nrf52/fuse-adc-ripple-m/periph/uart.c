#include "uart.h"

#include <nrf_drv_uart.h>

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void evh(nrfx_uart_event_t const * p_event,
         void *                    p_context)
{
    if (p_event->type == 1) {
        // here
    }
  nrfx_uart_rx(&m_uart.uart, "a", 1);
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
  nrfx_uart_rx(&m_uart.uart, "a", 1);
}

