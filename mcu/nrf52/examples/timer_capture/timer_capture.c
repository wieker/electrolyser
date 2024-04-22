#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "app_error.h"
#include <nrf_drv_ppi.h>
#include <nrf_drv_uart.h>
#include <nrf_drv_timer.h>
#include <nrf_drv_gpiote.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

const nrf_drv_timer_t capture_timer = NRF_DRV_TIMER_INSTANCE(1);

#define SAMPLE_PIN                      5
#define TIMER_PRESCALER                 NRF_TIMER_FREQ_1MHz
#define GPIOTE_CH_CAPTURE               0
#define GPIOTE_CH_RESTART               1

#define GPIO_OUTPUT_PIN_NUMBER 2

void send_timer_value(uint32_t cdata);

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    send_timer_value(nrf_drv_timer_capture_get(&capture_timer, 0));
}

void gpiote_capture_init(void)
{
    uint32_t err_code;
    static nrf_ppi_channel_t ppi_ch_gpiote_ca1;
    static nrf_ppi_channel_t ppi_ch_gpiote_ca2;
    APP_ERROR_CHECK(nrf_drv_ppi_init());

    // Initialize the GPIOTE driver
    APP_ERROR_CHECK(nrf_drv_gpiote_init());

    // Optionally, enable pullup or pulldown on the input pin
    nrf_drv_gpiote_in_config_t  in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    APP_ERROR_CHECK(nrf_drv_gpiote_in_init(4, &in_config, in_pin_handler));


    // Configure the capture timer
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = TIMER_PRESCALER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&capture_timer, &timer_cfg, 0);
    APP_ERROR_CHECK(err_code);


    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi_ch_gpiote_ca1));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi_ch_gpiote_ca1,
                                               nrf_drv_gpiote_in_event_addr_get(4),
                                               nrf_drv_timer_capture_task_address_get(&capture_timer, 0)));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi_ch_gpiote_ca1));

    nrf_drv_gpiote_in_event_enable(4, true);
}


/** @brief Function for main application entry.
 */
int measure(void)
{
    //APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    //NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("Pulse capture example started");


    nrfx_timer_resume(&capture_timer);

    return 0;
}



