#include <nrf_delay.h>
#include <nrf_esb.h>

#include "local_nrf_uart.h"
#include "local_nrf_esb.h"

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
  uart_init();

  radio_init(1);

  while (1) {
    NRF_P0->OUTSET = 1 << 17 | 1 << 20;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    nrf_delay_ms(1000);

    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
    nrf_delay_ms(1000);

    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);

    advertise();
  }
}

