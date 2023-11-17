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

#define GPIO_OUTPUT_PIN_NUMBER 18

static timer_dummy_handler(nrf_timer_event_t event_type, void * p_context) {}

static void gpiote_init() {
    NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER | 1 << 20;

    NRF_GPIOTE->CONFIG[PWM0_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
        GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
        GPIO_OUTPUT_PIN_NUMBER << GPIOTE_CONFIG_PSEL_Pos |
        GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;

    NRF_GPIOTE->CONFIG[PWM1_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos |
        GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos |
        20 << GPIOTE_CONFIG_PSEL_Pos |
        GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;
}

static void timer_init() {
    NRF_TIMER1->BITMODE                 = TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER1->PRESCALER               = 0;
    NRF_TIMER1->SHORTS                  = TIMER_SHORTS_COMPARE5_CLEAR_Msk;
    NRF_TIMER1->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL0] = 150 * 16000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL1] = 250 * 16000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL2] = 330 * 16000;
    NRF_TIMER1->CC[NRF_TIMER_CC_CHANNEL3] = 360 * 16000;

    NRF_TIMER1->TASKS_START = 1;
}

static void task_enable() {
    uint32_t evt_set, evt_clr, task_set, task_clr;
    nrf_ppi_channel_t ppi_channel_set;
    nrf_ppi_channel_t ppi_channel_clr;

    nrf_drv_ppi_channel_alloc(&ppi_channel_set);
    nrf_drv_ppi_channel_alloc(&ppi_channel_clr);

    evt_set = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL1];
    task_set = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM0_GPIOTE_CH];
    evt_clr = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL0];
    task_clr = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM0_GPIOTE_CH];

    nrf_drv_ppi_channel_assign(ppi_channel_set, evt_set, task_set);
    nrf_drv_ppi_channel_assign(ppi_channel_clr, evt_clr, task_clr);

    nrf_drv_ppi_channel_enable(ppi_channel_set);
    nrf_drv_ppi_channel_enable(ppi_channel_clr);

    nrf_drv_ppi_channel_alloc(&ppi_channel_set);
    nrf_drv_ppi_channel_alloc(&ppi_channel_clr);

    evt_set = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL3];
    task_set = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM1_GPIOTE_CH];
    evt_clr = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[NRF_TIMER_CC_CHANNEL2];
    task_clr = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM1_GPIOTE_CH];

    nrf_drv_ppi_channel_assign(ppi_channel_set, evt_set, task_set);
    nrf_drv_ppi_channel_assign(ppi_channel_clr, evt_clr, task_clr);

    nrf_drv_ppi_channel_enable(ppi_channel_set);
    nrf_drv_ppi_channel_enable(ppi_channel_clr);
}

void burst_mode_init(void)
{
    gpiote_init();
    timer_init();
    task_enable();

    //NRF_TIMER1->CC[PWM0_TIMER_CC_NUM] = 50 * 16000ULL;
}
