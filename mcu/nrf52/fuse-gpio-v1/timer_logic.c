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

#define GPIO_OUTPUT_PIN_NUMBER 2

int pwm = 0;

static timer_dummy_handler(nrf_timer_event_t event_type, void * p_context) {}

void gpiote_init() {
    NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
}

void toggle_io(int sta)
{
    NRF_TIMER2->CC[NRF_TIMER_CC_CHANNEL1] = pwm * 16000 + 2;
    if (sta == 0) {
        NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
    } else {
        NRF_P0->OUTSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    }
}
