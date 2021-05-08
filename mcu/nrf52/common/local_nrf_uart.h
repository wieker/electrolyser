//
// Created by wieker on 5/8/21.
//

#ifndef RF_UART_LOCAL_NRF_UART_H
#define RF_UART_LOCAL_NRF_UART_H

#include <legacy/nrf_drv_uart.h>

extern nrf_drv_uart_t m_uart;

void uart_init();

#endif //RF_UART_LOCAL_NRF_UART_H
