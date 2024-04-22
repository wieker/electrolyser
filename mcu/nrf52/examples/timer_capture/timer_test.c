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

static void timer_init() {
    NRF_TIMER1->BITMODE                 = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER1->PRESCALER               = NRF_TIMER_FREQ_1MHz;
    NRF_TIMER1->SHORTS                  = TIMER_SHORTS_COMPARE2_STOP_Msk;
    NRF_TIMER1->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL0] = 2 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL1] = 5 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL2] = 7 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL3] = 7 * 1000000;
}

int aa = 0;
void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    NRF_P0->DIRSET = 1 << 20;
    if (aa == 0) {
        NRF_P0->OUTCLR = 1 << 20;
        aa = 1;
    } else {
        NRF_P0->OUTSET = 1 << 20;
        aa = 0;
    }
}

static void led_blinking_setup()
{
    uint32_t input_evt_addr;
    uint32_t gpiote_task_addr;
    nrf_ppi_channel_t ppi1, ppi2, ppi3, ppi4, ppi5, ppi6;
    // Initialize PPI driver
    APP_ERROR_CHECK(nrf_drv_ppi_init());

    // Initialize the GPIOTE driver
    APP_ERROR_CHECK(nrf_drv_gpiote_init());

    // Configure GPIOTE OUT task
    nrf_drv_gpiote_out_config_t output_config =
    {
        .init_state     = NRF_GPIOTE_INITIAL_VALUE_HIGH,
        .task_pin       = true,                                                                       \
        .action         = NRF_GPIOTE_POLARITY_TOGGLE
    };
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(4, &output_config));

    // Configure GPIOTE OUT task
    nrf_drv_gpiote_out_config_t output_config2 =
    {
        .init_state     = NRF_GPIOTE_INITIAL_VALUE_HIGH,
        .task_pin       = true,                                                                       \
        .action         = NRF_GPIOTE_POLARITY_TOGGLE
    };
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(18, &output_config2));

    // Configure GPIOTE IN event
    nrf_drv_gpiote_in_config_t  in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
    APP_ERROR_CHECK(nrf_drv_gpiote_in_init(3, &in_config, in_pin_handler));

    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi1));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi1, nrf_drv_gpiote_in_event_addr_get(3), (uint32_t)&NRF_TIMER1->TASKS_START));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi1));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi2));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi2, (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL0], nrf_drv_gpiote_out_task_addr_get(4)));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi2));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi3));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi3, (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL1], nrf_drv_gpiote_out_task_addr_get(18)));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi3));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_alloc(&ppi4));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_assign(ppi4, (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL2], (uint32_t)&NRF_TIMER1->TASKS_CLEAR));
    APP_ERROR_CHECK(nrf_drv_ppi_channel_enable(ppi4));

    // Enable OUT task and IN event
    nrf_drv_gpiote_out_task_enable(4);
    nrf_drv_gpiote_out_task_enable(18);
    nrf_drv_gpiote_in_event_enable(3, true);
}

/** @brief Function for main application entry.
 */
int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();


    NRF_LOG_INFO("Pulse capture example init");
    NRF_LOG_FLUSH();
    timer_init();
    led_blinking_setup();
    NRF_LOG_INFO("Pulse capture example started");
    NRF_LOG_FLUSH();


    for (;;)
    {
        __WFI();
    }
}



