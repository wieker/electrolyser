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

void send_timer_value(uint32_t cdata0, uint32_t cdata1);
void mark_gpio_toggle();

int msr_activated = 0;
volatile int gpio_queued = 0;
volatile int dispatched = 0;

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if (1 == msr_activated && 4 == pin) {
        send_timer_value(
            nrf_drv_timer_capture_get(&capture_timer, 1),
            nrf_drv_timer_capture_get(&capture_timer, 0)
        );
        msr_activated = 0;
    }
    mark_gpio_toggle();
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

    nrf_drv_gpiote_in_config_t  in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    APP_ERROR_CHECK(nrf_drv_gpiote_in_init(4, &in_config, in_pin_handler));
    nrf_drv_gpiote_in_event_enable(4, true);
    APP_ERROR_CHECK(nrf_drv_gpiote_in_init(5, &in_config, in_pin_handler));
    nrf_drv_gpiote_in_event_enable(5, true);


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
    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi_ch_gpiote_ca2));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi_ch_gpiote_ca2,
                                               nrf_drv_gpiote_in_event_addr_get(5),
                                               nrf_drv_timer_capture_task_address_get(&capture_timer, 1)));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi_ch_gpiote_ca2));


    nrfx_timer_resume(&capture_timer);
}


/** @brief Function for main application entry.
 */
int measure(void)
{
    //APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    //NRF_LOG_DEFAULT_BACKENDS_INIT();
    msr_activated = 1;

    NRF_LOG_INFO("Pulse capture example started");

    nrfx_timer_clear(&capture_timer);

    return 0;
}



