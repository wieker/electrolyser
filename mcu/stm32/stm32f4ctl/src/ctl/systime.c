#include "stm32f4xx.h"
#include <math.h>
#include "defs.h"


#define TICKRATE_HZ     1000        /* 1 ms Tick rate */

// cycles per microsecond
static volatile uint32_t usTicks = 0;
// current uptime for 1kHz systick timer. will rollover after 49 days. hopefully we won't care.
static volatile uint32_t sysTickUptime = 0;

// Return system uptime in microseconds (rollover in 70minutes)
uint32_t micros(void)
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickUptime);
    return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

// Return system uptime in milliseconds (rollover in 49 days)
uint32_t millis(void)
{
    return sysTickUptime;
}

void sysInit() {
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);
    usTicks = SystemCoreClock / 1000000;
}

// SysTick
void SysTick_Handler(void)
{
    sysTickUptime++;
}



void delay(uint32_t duration)
{
	uint32_t i;

	for (i = 0; i < duration; i++)
		__asm__("nop");
}
