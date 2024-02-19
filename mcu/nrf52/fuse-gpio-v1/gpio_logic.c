#include "adc_logic.h"
#include "nrf.h"

#include <nrf_drv_gpiote.h>
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_saadc.h"

#define GPIO_OUTPUT_PIN_NUMBER 2
#define PWM0_GPIOTE_CH      0

static nrf_ppi_channel_t ppi_channel_set;
static nrf_ppi_channel_t ppi_channel_clr;
static nrf_ppi_channel_t ppi_channel_adc;

void gpiote_init() {
    //NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    //NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;

    NRF_GPIOTE->CONFIG[PWM0_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
    GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
    GPIO_OUTPUT_PIN_NUMBER << GPIOTE_CONFIG_PSEL_Pos |
    GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos;

    nrf_drv_ppi_channel_alloc(&ppi_channel_set);
    nrf_drv_ppi_channel_alloc(&ppi_channel_clr);
    nrf_drv_ppi_channel_alloc(&ppi_channel_adc);
}


static void timer_init() {
    NRF_TIMER2->BITMODE                 = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER2->PRESCALER               = 0;
    NRF_TIMER2->SHORTS                  = TIMER_SHORTS_COMPARE3_CLEAR_Msk;
    NRF_TIMER2->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER2->CC[NRF_TIMER_CC_CHANNEL0] = 1200;
    NRF_TIMER2->CC[NRF_TIMER_CC_CHANNEL1] = 1600;
    NRF_TIMER2->CC[NRF_TIMER_CC_CHANNEL3] = 2000;
}

static void task_enable() {
    uint32_t evt_set, evt_clr, task_set, task_clr;
    evt_set = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL0];
    task_set = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM0_GPIOTE_CH];
    evt_clr = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL3];
    task_clr = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM0_GPIOTE_CH];

    nrf_drv_ppi_channel_assign(ppi_channel_set, evt_set, task_set);
    nrf_drv_ppi_channel_assign(ppi_channel_clr, evt_clr, task_clr);

  uint32_t timer_compare_event_addr = (uint32_t)&NRF_TIMER2->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL1];
  uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

  nrf_drv_ppi_channel_assign(ppi_channel_adc,
                             timer_compare_event_addr,
                             saadc_sample_task_addr);

    nrf_drv_ppi_channel_enable(ppi_channel_set);
    nrf_drv_ppi_channel_enable(ppi_channel_clr);
    nrf_drv_ppi_channel_enable(ppi_channel_adc);
}

void toggle_io(int sta)
{
    if (sta == 0) {
        //NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
        NRF_TIMER2->TASKS_STOP = 1;
    } else {
        //NRF_P0->OUTSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
        timer_init();
        task_enable();
        NRF_TIMER2->TASKS_START = 1;
    }
}
