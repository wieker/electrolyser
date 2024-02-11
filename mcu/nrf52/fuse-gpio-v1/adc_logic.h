//
// Created by wieker on 6/7/21.
//

#ifndef RF_UART_NRF_LOCAL_ADC_H
#define RF_UART_NRF_LOCAL_ADC_H

#include <hal/nrf_saadc.h>

#define SAMPLES_IN_BUFFER 100

extern int16_t     adc_buffer[SAMPLES_IN_BUFFER];

void saadc_sampling_event_init(void);
void saadc_sampling_event_enable(void);
void saadc_sampling_event_disable(void);
void saadc_enable_fast(void);
void saadc_disable_fast(void);
void stop_uart_timer(void);
void saadc_init(void);

void slow_event_init();
void fast_event_init();
void gpiote_init();

void uart_buf_timer_init(void);

void ble_adc_cmd(int adc_cmd);
void dump_adc(uint8_t* value, int len);

void toggle_io(int _pwm);

#endif //RF_UART_NRF_LOCAL_ADC_H
