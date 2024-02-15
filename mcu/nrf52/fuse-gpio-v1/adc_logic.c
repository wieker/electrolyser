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

nrf_saadc_value_t     adc_buffer[SAMPLES_IN_BUFFER];

void send_adc(nrf_saadc_value_t *vls, int size);

/*
 * Two modes:
 * 1. continous 1-sec shoot.
 * Store values for 1-sec, 1-min, 10-minutes, last hour, 6-hour, 24 hour
 * 2. fast 20-ms timer2 burst for 500 ms
 * use timer1 CC1 / CC2 to switch on / off GPIO during the ADC interval
 * */

int32_t minuteV = 0;
int32_t minuteV10 = 0;
int32_t hourV = 0;
int32_t hourV6 = 0;
int32_t hourV24 = 0;

static uint32_t              m_adc_evt_counter = 0;
static uint32_t              min_counter = 0;
static uint32_t              hour_counter = 0;

nrf_saadc_value_t     bmode[100];
nrf_saadc_value_t     ring_minute[60] = {0};
nrf_saadc_value_t     ring_min10[10] = {0};
nrf_saadc_value_t     ring_hour[60] = {0};
nrf_saadc_value_t     ring_hour6[6] = {0};
nrf_saadc_value_t     ring_hour24[24] = {0};

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
  if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
  {
      minuteV = minuteV + adc_buffer[0] - ring_minute[m_adc_evt_counter % 60];
      ring_minute[m_adc_evt_counter % 60] = adc_buffer[0];
      m_adc_evt_counter ++;

      if (m_adc_evt_counter % 60 == 0) {
        minuteV10 = minuteV10 + (int16_t) (minuteV / 60) - ring_min10[min_counter % 10];
        ring_min10[min_counter % 10] = (int16_t) (minuteV / 60);
        hourV = hourV + (int16_t) (minuteV / 60) - ring_hour[min_counter % 60];
        ring_hour[min_counter % 60] = (int16_t) (minuteV / 60);
        min_counter ++;

        if (min_counter % 60 == 0) {
          hourV6 = hourV6 + (int16_t) (hourV / 60) - ring_hour6[hour_counter % 6];
          ring_hour6[hour_counter % 6] = (int16_t) (hourV / 60);
          hourV24 = hourV24 + (int16_t) (hourV / 60) - ring_hour24[hour_counter % 24];
          ring_hour24[hour_counter % 24] = (int16_t) (hourV / 60);
          hour_counter ++;
        }
      }

      adc_buffer[1] = (int16_t) (minuteV / 60);
      adc_buffer[2] = (int16_t) (minuteV10 / 10);
      adc_buffer[3] = (int16_t) (hourV / 60);
      adc_buffer[4] = (int16_t) (hourV6 / 6);
      adc_buffer[5] = (int16_t) (hourV24 / 24);
      send_adc(adc_buffer, 6);
      nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
  }
}


void saadc_init(void)
{
  nrf_saadc_channel_config_t channel_config_V =
          NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
  channel_config_V.gain = SAADC_CH_CONFIG_GAIN_Gain1_4;
  channel_config_V.reference = SAADC_CH_CONFIG_REFSEL_Internal;
  channel_config_V.pin_n = NRF_SAADC_INPUT_AIN5;
  channel_config_V.mode = NRF_SAADC_MODE_DIFFERENTIAL;

  nrf_drv_saadc_init(NULL, saadc_callback);

  nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_10BIT);

  nrfx_saadc_channel_init(0, &channel_config_V);

  nrfx_saadc_buffer_convert(adc_buffer, 1);

}

/** @} */

