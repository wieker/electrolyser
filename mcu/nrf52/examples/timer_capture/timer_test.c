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


// Peripheral channel assignments
#define PWM0_GPIOTE_CH      0
#define PWM0_PPI_CH_A       0
#define PWM0_PPI_CH_B       1
#define PWM0_TIMER_CC_NUM   0

#define PWM1_GPIOTE_CH      1
#define PWM1_PPI_CH_A       2
#define PWM1_TIMER_CC_NUM   1

#define PWMN_GPIOTE_CH      {PWM0_GPIOTE_CH, PWM1_GPIOTE_CH, 2, 3, 4}
#define PWMN_PPI_CH_A       {PWM0_PPI_CH_A, PWM1_PPI_CH_A, 3, 5, 6}
#define PWMN_PPI_CH_B       {PWM0_PPI_CH_B, PWM0_PPI_CH_B, 4, 4, 7}
#define PWMN_TIMER_CC_NUM   {PWM0_TIMER_CC_NUM, PWM1_TIMER_CC_NUM, 2, 3, 4}

static uint32_t pwmN_gpiote_ch[]    = PWMN_GPIOTE_CH;
static uint32_t pwmN_ppi_ch_a[]     = PWMN_PPI_CH_A;
static uint32_t pwmN_ppi_ch_b[]     = PWMN_PPI_CH_B;
static uint32_t pwmN_timer_cc_num[] = PWMN_TIMER_CC_NUM;

// TIMER3 reload value. The PWM frequency equals '16000000 / TIMER_RELOAD'
#define TIMER_RELOAD        (500 * 16000ULL)
// The timer CC register used to reset the timer. Be aware that not all timers in the nRF52 have 6 CC registers.
#define TIMER_RELOAD_CC_NUM 5

static const nrf_drv_timer_t timer = NRF_DRV_TIMER_INSTANCE(1);

#define GPIO_OUTPUT_PIN_NUMBER 2

static timer_dummy_handler(nrf_timer_event_t event_type, void * p_context) {}

static void gpiote_init() {
    NRF_GPIOTE->CONFIG[PWM0_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
        GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
        17 << GPIOTE_CONFIG_PSEL_Pos |
        GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos;

    NRF_GPIOTE->CONFIG[PWM1_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
        GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
        18 << GPIOTE_CONFIG_PSEL_Pos |
        GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos;
}

static void timer_init() {
    NRF_TIMER1->BITMODE                 = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER1->PRESCALER               = NRF_TIMER_FREQ_1MHz;
    NRF_TIMER1->SHORTS                  = TIMER_SHORTS_COMPARE3_STOP_Msk;
    NRF_TIMER1->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL0] = 2 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL1] = 5 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL2] = 50 * 1000000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL3] = 7 * 1000000;
}

static nrf_ppi_channel_t ppi_channel_set;
static nrf_ppi_channel_t ppi_channel_clr;
static nrf_ppi_channel_t ppi_channel_timer;

static void task_enable() {
    nrfx_ppi_channel_alloc(&ppi_channel_set);
    nrfx_ppi_channel_alloc(&ppi_channel_clr);
    nrfx_ppi_channel_alloc(&ppi_channel_timer);
    uint32_t evt_set, evt_clr, task_set, task_clr;
    //evt_set = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL0];
    //task_set = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM0_GPIOTE_CH];
    //evt_clr = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL1];
    //task_clr = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM1_GPIOTE_CH];

    //nrf_drv_ppi_channel_assign(ppi_channel_set, evt_set, task_set);
    //nrf_drv_ppi_channel_assign(ppi_channel_clr, evt_clr, task_clr);

    evt_clr = (uint32_t)&NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_RESTART];
    //task_clr = (uint32_t)&NRF_TIMER1->TASKS_START;
    task_clr = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM1_GPIOTE_CH];

    nrf_drv_ppi_channel_assign(ppi_channel_timer, evt_clr, task_clr);

    //nrf_drv_ppi_channel_enable(ppi_channel_set);
    //nrf_drv_ppi_channel_enable(ppi_channel_clr);
    nrf_drv_ppi_channel_enable(ppi_channel_timer);
}

void gpiote_capture_init(void)
{
    // Optionally, enable pullup or pulldown on the input pin
    nrf_gpio_cfg_input(13, NRF_GPIO_PIN_PULLUP);

    // The GPIOTE driver doesn't support two GPIOTE channels on the same pin, so direct register access is necessary
    NRF_GPIOTE->CONFIG[GPIOTE_CH_RESTART] = GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos |
                                            GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos |
                                            13 << GPIOTE_CONFIG_PSEL_Pos;

    // Make sure the GPIOTE capture in event is cleared. This will be  used to detect if a capture has occured.
    NRF_GPIOTE->EVENTS_IN[GPIOTE_CH_RESTART] = 0;
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


void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    NRF_LOG_INFO("Int");
    NRF_LOG_FLUSH();
}

static void led_blinking_setup()
{
    uint32_t input_evt_addr;
    uint32_t gpiote_task_addr;
    nrf_ppi_channel_t ppi1, ppi2, ppi3;
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
    nrf_drv_gpiote_in_config_t  in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
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
    //gpiote_capture_init();
    //gpiote_init();
    //timer_init();
    //task_enable();
    timer_init();
    led_blinking_setup();
    NRF_LOG_INFO("Pulse capture example started");
    NRF_LOG_FLUSH();


    for (;;)
    {
        __WFI();
    }
}



