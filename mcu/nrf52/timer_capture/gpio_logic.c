#include "adc_logic.h"
#include "nrf.h"

#include <nrf_drv_gpiote.h>
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_saadc.h"


#include "nrf_delay.h"

#define GPIO_OUTPUT_PIN_NUMBER 2

void gpiote_init() {
    NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
}

int measure(void);

uint32_t toggle_io(int sta)
{
    if (sta == 0) {
        NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
        return 0;
    } else {
        measure();
        NRF_P0->OUTSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
        return 0;
    }
}
