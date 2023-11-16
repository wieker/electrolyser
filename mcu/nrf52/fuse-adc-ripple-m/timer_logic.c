#include "adc_logic.h"
#include "nrf.h"

#include <nrf_drv_gpiote.h>
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"


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

#define GPIO_OUTPUT_PIN_NUMBER 17

static timer_dummy_handler(nrf_timer_event_t event_type, void * p_context) {}

void burst_mode_init(void)
{
    nrf_drv_gpiote_init();

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    nrf_drv_timer_init(&timer, &timer_cfg, timer_dummy_handler);

    uint32_t compare_evt_addr;
    uint32_t gpiote_task_addr;
    nrf_ppi_channel_t ppi_channel;
    nrf_drv_gpiote_out_config_t config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(false);

    nrf_drv_gpiote_out_init(GPIO_OUTPUT_PIN_NUMBER, &config);

    nrf_drv_timer_extended_compare(&timer, (nrf_timer_cc_channel_t)0, 200 * 1000UL, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, false);

    nrf_drv_ppi_channel_alloc(&ppi_channel);

    compare_evt_addr = nrf_drv_timer_event_address_get(&timer, NRF_TIMER_EVENT_COMPARE0);
    gpiote_task_addr = nrf_drv_gpiote_out_task_addr_get(GPIO_OUTPUT_PIN_NUMBER);

    nrf_drv_ppi_channel_assign(ppi_channel, compare_evt_addr, gpiote_task_addr);

    nrf_drv_ppi_channel_enable(ppi_channel);

    nrf_drv_gpiote_out_task_enable(GPIO_OUTPUT_PIN_NUMBER);

    nrf_drv_timer_enable(&timer);

    NRF_TIMER1->CC[PWM0_TIMER_CC_NUM] = 50 * 16000ULL;
}
