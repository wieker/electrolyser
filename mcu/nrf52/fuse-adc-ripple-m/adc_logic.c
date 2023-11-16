//
// Created by wieker on 6/7/21.
//

#include "adc_logic.h"

/** @file
 * @defgroup nrf_adc_example main.c
 * @{
 * @ingroup nrf_adc_example
 * @brief ADC Example Application main file.
 *
 * This file contains the source code for a sample application using ADC.
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */

#include <stdbool.h>
#include <string.h>
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_delay.h"
#include "ble_lbs.h"

volatile uint8_t state = 1;

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(1);
static const nrf_drv_timer_t m_timer2 = NRF_DRV_TIMER_INSTANCE(2);
nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t     m_ppi_channel;
static uint32_t              m_adc_evt_counter;

timer_handler(nrf_timer_event_t event_type, void * p_context);

void saadc_sampling_event_init(void)
{

  nrf_drv_ppi_init();

  nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
  timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
  nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);

  /* setup m_timer for compare event every 400ms */
  uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 1000);
  nrf_drv_timer_extended_compare(&m_timer,
                                 NRF_TIMER_CC_CHANNEL0,
                                 ticks,
                                 NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                 false);
  nrf_drv_timer_enable(&m_timer);

  uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                              NRF_TIMER_CC_CHANNEL0);
  uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

  /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
  nrf_drv_ppi_channel_alloc(&m_ppi_channel);

  nrf_drv_ppi_channel_assign(m_ppi_channel,
                             timer_compare_event_addr,
                             saadc_sample_task_addr);
}

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

void burst_mode_init(void)
{
    NRF_TIMER1->BITMODE                 = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER1->PRESCALER               = 0;
    NRF_TIMER1->SHORTS                  = TIMER_SHORTS_COMPARE0_CLEAR_Msk << TIMER_RELOAD_CC_NUM;
    NRF_TIMER1->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER1->CC[TIMER_RELOAD_CC_NUM] = TIMER_RELOAD;

    NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;

    NRF_GPIOTE->CONFIG[PWM0_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
                                         GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
                                         17 << GPIOTE_CONFIG_PSEL_Pos |
                                         GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;

    NRF_PPI->CH[PWM0_PPI_CH_A].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[PWM0_TIMER_CC_NUM];
    NRF_PPI->CH[PWM0_PPI_CH_A].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM0_GPIOTE_CH];
    NRF_PPI->CH[PWM0_PPI_CH_B].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[TIMER_RELOAD_CC_NUM];
    NRF_PPI->CH[PWM0_PPI_CH_B].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM0_GPIOTE_CH];

    NRF_PPI->CHENSET               = (1 << PWM0_PPI_CH_A) | (1 << PWM0_PPI_CH_B);

    NRF_TIMER1->TASKS_START = 1;

    NRF_TIMER1->CC[PWM0_TIMER_CC_NUM] = 50 * 16000ULL;
}

void uart_buf_timer_init(void)
{

  nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
  timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
  nrf_drv_timer_init(&m_timer2, &timer_cfg, timer_handler);

  /* setup m_timer for compare event every 400ms */
  uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer2, 20);
  nrf_drv_timer_extended_compare(&m_timer2,
                                 NRF_TIMER_CC_CHANNEL1,
                                 ticks,
                                 NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK,
                                 true);
  nrf_drv_timer_enable(&m_timer2);
}


void saadc_sampling_event_enable(void)
{
  nrf_drv_ppi_channel_enable(m_ppi_channel);
}

void send_adc(nrf_saadc_value_t *vls);

int mode = 0;
/*
 * Two modes:
 * 1. continous 1-sec shoot.
 * Store values for 1-sec, 1-min, 10-minutes, last hour, 6-hour, 24 hour
 * 2. fast 20-ms timer2 burst for 500 ms
 * use timer1 CC1 / CC2 to switch on / off GPIO during the ADC interval
 * */

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
  if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
  {
            if (mode == 0) {
              nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
            }

            send_adc(m_buffer_pool[0]);
  }
}

void change_mode(int new_mode) {

}


void saadc_init(void)
{
  nrf_saadc_channel_config_t channel_config_V =
          NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
  channel_config_V.gain = SAADC_CH_CONFIG_GAIN_Gain1;
  channel_config_V.reference = SAADC_CH_CONFIG_REFSEL_Internal;
  nrf_saadc_channel_config_t channel_config_I =
          NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);

  nrf_drv_saadc_init(NULL, saadc_callback);

  nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_10BIT);

  nrfx_saadc_channel_init(0, &channel_config_V);

  //nrfx_saadc_channel_init(1, &channel_config_I);

  nrfx_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);

}

/** @} */

