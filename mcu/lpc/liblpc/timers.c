/*
 * @brief Blinky example using a timer and interrupt
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdio.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TICKRATE_HZ1 10
#define TICKRATE_HZ2 10000
#define TICKRATE_HZ3 1

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

extern void write_data(uint8_t slaveAddr, uint8_t addr, uint8_t data);

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
void TIMER1_IRQHandler(void)
{
    static bool On = false;
    static int counter = 0;

    if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) {
        Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
        On = (bool) !On;
        Board_LED_Set(0, On);
    }
}

void TIMER2_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER2, 0)) {
        Chip_TIMER_ClearMatch(LPC_TIMER2, 0);
        On = (bool) !On;
        Board_LED_Set(1, On);
    }
}

void TIMER3_IRQHandler(void)
{
    static bool On = false;

    if (Chip_TIMER_MatchPending(LPC_TIMER3, 3)) {
        Chip_TIMER_ClearMatch(LPC_TIMER3, 3);
        On = (bool) !On;
        Board_LED_Set(1, On);
    }
}

void timer1_isr(void) {
    DEBUGSTR("blink 1!\r\n");
    TIMER1_IRQHandler();
}

void timer2_isr(void) {
    DEBUGSTR("blink 2!\r\n");
    TIMER2_IRQHandler();
}

void timer0_isr(void) {
    DEBUGSTR("blink 0!\r\n");
}

void timer3_isr(void) {
    DEBUGSTR("blink 3!\r\n");
    TIMER3_IRQHandler();
}

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main_timer(void)
{
    uint32_t timerFreq1;
    uint32_t timerFreq2;
    uint32_t timerFreq3;

	//SystemCoreClockUpdate();
	//Board_Init();

	/* Enable timer 1 clock and reset it */
    Chip_TIMER_Init(LPC_TIMER1);
    Chip_RGU_TriggerReset(RGU_TIMER1_RST);
    while (Chip_RGU_InReset(RGU_TIMER1_RST)) {}

    Chip_TIMER_Init(LPC_TIMER2);
    Chip_RGU_TriggerReset(RGU_TIMER2_RST);
    while (Chip_RGU_InReset(RGU_TIMER2_RST)) {}

    Chip_TIMER_Init(LPC_TIMER3);
    Chip_RGU_TriggerReset(RGU_TIMER3_RST);
    while (Chip_RGU_InReset(RGU_TIMER3_RST)) {}

	/* Get timer 1 peripheral clock rate */
    timerFreq1 = Chip_Clock_GetRate(CLK_MX_TIMER1);
    timerFreq2 = Chip_Clock_GetRate(CLK_MX_TIMER2);
    timerFreq3 = Chip_Clock_GetRate(CLK_MX_TIMER3);

	/* Timer setup for match and interrupt at TICKRATE_HZ1 */
    Chip_TIMER_Reset(LPC_TIMER1);
    Chip_TIMER_TIMER_SetCountClockSrc(LPC_TIMER1, TIMER_CAPSRC_BOTH_CAPN, 2);
    Chip_TIMER_ClearCapture(LPC_TIMER1, 2);
    Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
    Chip_TIMER_SetMatch(LPC_TIMER1, 1, 1);
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
    Chip_TIMER_Enable(LPC_TIMER1);

    Chip_TIMER_Reset(LPC_TIMER2);
    Chip_TIMER_SetMatch(LPC_TIMER2, 0, (timerFreq2 / TICKRATE_HZ2));
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER2, 0);
    Chip_TIMER_ExtMatchControlSet(LPC_TIMER2, 1, TIMER_EXTMATCH_TOGGLE, 0);
    Chip_TIMER_Enable(LPC_TIMER2);

    Chip_TIMER_Reset(LPC_TIMER3);
    Chip_TIMER_MatchEnableInt(LPC_TIMER3, 3);
    Chip_TIMER_SetMatch(LPC_TIMER3, 3, (timerFreq2 / TICKRATE_HZ3));
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER3, 3);
    Chip_TIMER_ExtMatchControlSet(LPC_TIMER3, 1, TIMER_EXTMATCH_TOGGLE, 3);
    Chip_TIMER_Enable(LPC_TIMER3);

	/* Enable timer interrupt */
    NVIC_EnableIRQ(TIMER1_IRQn);
    NVIC_ClearPendingIRQ(TIMER1_IRQn);
    NVIC_EnableIRQ(TIMER2_IRQn);
    NVIC_ClearPendingIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER3_IRQn);
    NVIC_ClearPendingIRQ(TIMER3_IRQn);

    LPC_GIMA->CAP0_IN[1][2] = 0x00;
    Chip_SCU_PinMuxSet(2, 13, (SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC1));

    DEBUGSTR("Blinky example using timer 1!\r\n");
    DEBUGOUT("Timer 1 clock     = %d Hz\r\n", timerFreq1);
    DEBUGOUT("Timer 1 tick rate = %d Hz\r\n", TICKRATE_HZ1);

    DEBUGSTR("Blinky example using timer 2!\r\n");
    DEBUGOUT("Timer 2 clock     = %d Hz\r\n", timerFreq2);
    DEBUGOUT("Timer 2 tick rate = %d Hz\r\n", TICKRATE_HZ2);

    DEBUGSTR("Blinky example using timer 3!\r\n");
    DEBUGOUT("Timer 3 clock     = %d Hz\r\n", timerFreq3);
    DEBUGOUT("Timer 3 tick rate = %d Hz\r\n", TICKRATE_HZ3);
}
