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

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
  if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
  {
            nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);

            send_adc(m_buffer_pool[0]);
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

  nrfx_saadc_channel_init(1, &channel_config_I);

  nrfx_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);

}

/** @} */

