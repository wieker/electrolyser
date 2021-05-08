//
// Created by wieker on 5/8/21.
//

#ifndef RF_UART_LOCAL_NRF_ESB_H
#define RF_UART_LOCAL_NRF_ESB_H
#include <nrf_esb.h>

uint32_t radio_init(uint16_t network_id);
void advertise();

#endif //RF_UART_LOCAL_NRF_ESB_H
