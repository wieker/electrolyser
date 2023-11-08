//
// Created by wieker on 6/7/21.
//

#ifndef RF_UART_NRF_LOCAL_ADC_H
#define RF_UART_NRF_LOCAL_ADC_H

#include <hal/nrf_saadc.h>

#define SAMPLES_IN_BUFFER 6

extern nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];

void saadc_sampling_event_init(void);
void saadc_sampling_event_enable(void);
void saadc_init(void);

#endif //RF_UART_NRF_LOCAL_ADC_H
