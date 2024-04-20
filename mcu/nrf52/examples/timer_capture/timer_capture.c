#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "app_error.h"
#include <nrf_drv_ppi.h>
#include <nrf_drv_timer.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

const nrf_drv_timer_t capture_timer = NRF_DRV_TIMER_INSTANCE(3);

#define SAMPLE_PIN                      3
#define TIMER_PRESCALER                 NRF_TIMER_FREQ_1MHz
#define GPIOTE_CH_CAPTURE               0
#define GPIOTE_CH_RESTART               1
#define GPIOTE_CH_SELFTEST              2

static void gpiote_capture_init(void)
{
    uint32_t err_code;
    static nrf_ppi_channel_t ppi_ch_gpiote_capture;
    static nrf_ppi_channel_t ppi_ch_gpiote_restart;

    // Optionally, enable pullup or pulldown on the input pin
    //nrf_gpio_cfg_input(SAMPLE_PIN, NRF_GPIO_PIN_PULLUP);

    // Allocate two PPI channels
    nrfx_ppi_channel_alloc(&ppi_ch_gpiote_capture);
    nrfx_ppi_channel_alloc(&ppi_ch_gpiote_restart);

    // Configure the capture timer
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = TIMER_PRESCALER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&capture_timer, &timer_cfg, 0);
    APP_ERROR_CHECK(err_code);

    // The GPIOTE driver doesn't support two GPIOTE channels on the same pin, so direct register access is necessary
    NRF_GPIOTE->CONFIG[GPIOTE_CH_CAPTURE] = GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos |
                                            SAMPLE_PIN << GPIOTE_CONFIG_PSEL_Pos;
    NRF_GPIOTE->CONFIG[GPIOTE_CH_RESTART] = GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos |
                                            SAMPLE_PIN << GPIOTE_CONFIG_PSEL_Pos;

    // Assign a PPI channel to capture the current timer state and store it in CC register 0
    nrfx_ppi_channel_assign(ppi_ch_gpiote_capture,
                            (uint32_t)&NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE],
                            nrf_drv_timer_capture_task_address_get(&capture_timer, 0));

    // Assign a second PPI channel to restart the timer when a new pulse is detected
    nrfx_ppi_channel_assign(ppi_ch_gpiote_restart,
                            (uint32_t)&NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_RESTART],
                            nrf_drv_timer_task_address_get(&capture_timer, NRF_TIMER_TASK_CLEAR));

    // Enable both PPI channels
    nrfx_ppi_channel_enable(ppi_ch_gpiote_capture);
    nrfx_ppi_channel_enable(ppi_ch_gpiote_restart);

    // Make sure the GPIOTE capture in event is cleared. This will be  used to detect if a capture has occured.
    NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE] = 0;

    // Start the timer
    nrfx_timer_resume(&capture_timer);
}


static uint32_t timer_capture_value_get(void)
{
    // Make sure the capture event occured before checking the capture register
    if(NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE] != 0)
    {
        // Clear the capture event
        NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE] = 0;

        // Return the stored capture value in the timer
        return nrf_drv_timer_capture_get(&capture_timer, 0);
    }
    else
    {
        // In case no capture occured, return 0
        return 0;
    }
}

/** @brief Function for main application entry.
 */
int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("Pulse capture example started");


    gpiote_capture_init();

#if SELF_TEST
    run_self_test_pin(LED_1, 678, 10);
#endif

    while (true)
    {
        uint32_t captured_pulse_length = timer_capture_value_get();

        if(captured_pulse_length > 0)
        {
            NRF_LOG_INFO("Capture value: %i us", (captured_pulse_length * (1 << TIMER_PRESCALER) / 16));
        }
        else NRF_LOG_INFO("No capture detected");

        nrf_delay_ms(500);
        NRF_LOG_FLUSH();
    }
}



