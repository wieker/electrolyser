#include "adc_logic.h"
#include "nrf.h"

#include <nrf_drv_gpiote.h>
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_saadc.h"

#define GPIO_OUTPUT_PIN_NUMBER 2

const nrf_drv_timer_t capture_timer = NRF_DRV_TIMER_INSTANCE(2);
#define SAMPLE_PIN1                      3
#define SAMPLE_PIN2                      4
#define TIMER_PRESCALER                 NRF_TIMER_FREQ_16MHz
#define GPIOTE_CH_CAPTURE1               0
#define GPIOTE_CH_CAPTURE2               1

void gpiote_init() {
    NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
}

static void gpiote_capture_init(void)
{
    uint32_t err_code;
    static nrf_ppi_channel_t ppi_ch_gpiote_capture1;
    static nrf_ppi_channel_t ppi_ch_gpiote_capture2;

    // Optionally, enable pullup or pulldown on the input pin
    nrf_gpio_cfg_input(SAMPLE_PIN1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(SAMPLE_PIN2, NRF_GPIO_PIN_PULLUP);

    // Allocate two PPI channels
    nrfx_ppi_channel_alloc(&ppi_ch_gpiote_capture1);
    nrfx_ppi_channel_alloc(&ppi_ch_gpiote_capture2);

    // Configure the capture timer
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.frequency = TIMER_PRESCALER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&capture_timer, &timer_cfg, 0);
    APP_ERROR_CHECK(err_code);

    // The GPIOTE driver doesn't support two GPIOTE channels on the same pin, so direct register access is necessary
    NRF_GPIOTE->CONFIG[GPIOTE_CH_CAPTURE1] = GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos |
                                            SAMPLE_PIN1 << GPIOTE_CONFIG_PSEL_Pos;
    NRF_GPIOTE->CONFIG[GPIOTE_CH_CAPTURE2] = GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos |
                                            SAMPLE_PIN2 << GPIOTE_CONFIG_PSEL_Pos;

    // Assign a PPI channel to capture the current timer state and store it in CC register 0
    nrfx_ppi_channel_assign(ppi_ch_gpiote_capture1,
                            (uint32_t)&NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE1],
                            nrf_drv_timer_capture_task_address_get(&capture_timer, 0));

    // Assign a second PPI channel to restart the timer when a new pulse is detected
    nrfx_ppi_channel_assign(ppi_ch_gpiote_capture2,
                            (uint32_t)&NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE2],
                            nrf_drv_timer_capture_task_address_get(&capture_timer, 1));

    // Enable both PPI channels
    nrfx_ppi_channel_enable(ppi_ch_gpiote_capture1);
    nrfx_ppi_channel_enable(ppi_ch_gpiote_capture2);

    // Make sure the GPIOTE capture in event is cleared. This will be  used to detect if a capture has occured.
    NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE1] = 0;
    NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE2] = 0;

    // Start the timer
    nrfx_timer_resume(&capture_timer);
}

static uint32_t timer_capture_value_get(void)
{
    // Make sure the capture event occured before checking the capture register
    if(NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE1] != 0)
    {
        // Clear the capture event
        NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_CAPTURE1] = 0;

        // Return the stored capture value in the timer
        return nrf_drv_timer_capture_get(&capture_timer, 0);
    }
    else
    {
        // In case no capture occured, return 0
        return 0;
    }
}

void toggle_io(int sta)
{
    if (sta == 0) {
        NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
        //NRF_TIMER2->TASKS_STOP = 1;
    } else {
        NRF_P0->OUTSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
        //timer_init();
        //task_enable();
        //NRF_TIMER2->TASKS_START = 1;
    }
}
