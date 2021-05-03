#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>

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

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
  uart_init();

  while (1) {
    NRF_P0->OUTSET = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    nrf_delay_ms(300);

    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
    nrf_delay_ms(300);

    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
  }
}

