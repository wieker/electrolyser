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
nrf_saadc_value_t     adc_buffer[SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t     m_ppi_channel_slow;
static nrf_ppi_channel_t     m_ppi_channel_fast;
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
}

void slow_event_init(void)
{
  uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                              NRF_TIMER_CC_CHANNEL0);
  uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

  /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
  nrf_drv_ppi_channel_alloc(&m_ppi_channel_slow);

  nrf_drv_ppi_channel_assign(m_ppi_channel_slow,
                             timer_compare_event_addr,
                             saadc_sample_task_addr);
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

void fast_event_init(void)
{
  uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer2,
                                                                              NRF_TIMER_CC_CHANNEL1);
  uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

  /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
  nrf_drv_ppi_channel_alloc(&m_ppi_channel_fast);

  nrf_drv_ppi_channel_assign(m_ppi_channel_fast,
                             timer_compare_event_addr,
                             saadc_sample_task_addr);
}


void saadc_sampling_event_enable(void)
{
  nrf_drv_ppi_channel_enable(m_ppi_channel_slow);
}

void saadc_sampling_event_disable(void)
{
  nrf_drv_ppi_channel_disable(m_ppi_channel_slow);
}

void saadc_enable_fast() {
  nrf_drv_ppi_channel_enable(m_ppi_channel_fast);
}

void saadc_disable_fast() {
  nrf_drv_ppi_channel_disable(m_ppi_channel_fast);
}

void stop_uart_timer() {
    NRF_TIMER2->TASKS_STOP = 1;
}

void send_adc(nrf_saadc_value_t *vls, int size);

int mode = 0;
/*
 * Two modes:
 * 1. continous 1-sec shoot.
 * Store values for 1-sec, 1-min, 10-minutes, last hour, 6-hour, 24 hour
 * 2. fast 20-ms timer2 burst for 500 ms
 * use timer1 CC1 / CC2 to switch on / off GPIO during the ADC interval
 * */

float minuteV, min10V, hourV, hour6V, hour24V;
nrf_saadc_value_t     bmode[100];

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
  if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
  {
    if (mode == 0) {
      nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
      minuteV = (minuteV * 59 + (float) (adc_buffer[0])) / 60;
      min10V = (min10V * 599 + (float) (adc_buffer[0])) / 600;
      hourV = (hourV * 3599 + (float) (adc_buffer[0])) / 3600;
      hour6V = (hour6V * 21599 + (float) (adc_buffer[0])) / 21600;
      hour24V = (hour24V * 86399 + (float) (adc_buffer[0])) / 86400;
      adc_buffer[1] = (int16_t) minuteV;
      adc_buffer[2] = (int16_t) min10V;
      adc_buffer[3] = (int16_t) hourV;
      adc_buffer[4] = (int16_t) hour6V;
      adc_buffer[5] = (int16_t) hour24V;
      send_adc(adc_buffer, 6);
    }
    if (mode == 2) {
        burst_mode_deinit();
        dump_adc("finished\n", 9);
        mode = 0;
        nrfx_saadc_buffer_convert(adc_buffer, 1);
    }
    if (mode == 1) {
      dump_adc("started\n", 9);
      burst_mode_init();
      nrf_drv_saadc_buffer_convert(bmode, 15);
      //mode = 2;
    }
  }
}

void ble_adc_cmd(int adc_cmd) {
  if (adc_cmd == 0x00) {
        uint8_t nus_string[50];
        for (int i = 0; i< 6; i ++) {
        int bytes_to_send = sprintf(nus_string,
                                "CH%d: %d\r\n",
                                i,
                                adc_buffer[i]
                                );

          dump_adc(nus_string, bytes_to_send);
        }
  }
  if (adc_cmd == 0x01) {
    mode = 1;
  }
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

  nrfx_saadc_buffer_convert(adc_buffer, 1);

}

/** @} */

