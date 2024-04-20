#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "app_error.h"
#include <nrf_drv_ppi.h>
#include <nrf_drv_uart.h>
#include <nrf_drv_timer.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_systick.h"




/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure LED-pins as outputs. */
    NRF_P0->DIRSET = 1 << 17;
    NRF_P0->OUTCLR = 1 << 17;

    /* Init systick driver */
    nrf_drv_systick_init();

    /* Toggle LEDs. */
    while (true)
    {
        NRF_P0->OUTSET = 1 << 17;
        nrf_drv_systick_delay_ms(500);
        NRF_P0->OUTCLR = 1 << 17;
        nrf_drv_systick_delay_ms(500);
    }
}




