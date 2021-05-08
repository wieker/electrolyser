//
// Created by wieker on 5/8/21.
//

#include "local_nrf_uart.h"
#include <sdk_config.h>

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void uart_init()
{
  nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
  config.pseltxd  = 13;
  config.pselrxd  = NRF_UART_PSEL_DISCONNECTED;
  config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
  config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
  config.baudrate = (nrf_uart_baudrate_t)NRF_LOG_BACKEND_UART_BAUDRATE;
  nrfx_uart_init(&m_uart.uart,
                 (nrfx_uart_config_t const *)&config,
                 NULL);
}
