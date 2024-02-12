#include "adc_logic.h"
#include "nrf.h"

#include <nrf_drv_gpiote.h>

#define GPIO_OUTPUT_PIN_NUMBER 2

void gpiote_init() {
    NRF_P0->DIRSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
}

void toggle_io(int sta)
{
    if (sta == 0) {
        NRF_P0->OUTCLR = 1 << GPIO_OUTPUT_PIN_NUMBER;
    } else {
        NRF_P0->OUTSET = 1 << GPIO_OUTPUT_PIN_NUMBER;
    }
}
