/*
 * Copyright 2010 - 2012 Michael Ossmann
 *
 * This file is part of HackRF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "gpio_lpc.h"
#include <math.h>
#include "board.h"
#include "ctl/defs.h"

static struct gpio_t gpio_led[] = {
	GPIO(3,  7),
	GPIO(0,  7),
	GPIO(3,  5),
};

void delay(uint32_t duration)
{
	uint32_t i;

	for (i = 0; i < duration; i++)
		__asm__("nop");
}


uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0;

int32_t errorGyroI[3] = { 0, 0, 0 };
static int32_t errorAngleI[2] = { 0, 0 };

#define GYRO_I_MAX 500

int constrain(int amt, int low, int high)
{
    if (amt < low)
        return low;
    else if (amt > high)
        return high;
    else
        return amt;
}

int32_t yawUI = 0;
int32_t desired = 0;
int32_t lastAngle[3];
float lastAngleDiff[3];

void checkCond(int axis, int32_t angleSpeed) {
    if (throttle < 0) {
        return;
    }
    if (fabsf(relAngle[axis]) > 5 || abs(angle[ROLL]) > 100 || abs(angle[1]) > 100) {
        printf("abs %d %d %d\r\n", accADC[0], accADC[1], accADC[2]);
        printf("rotate %d %d\r\n", angleSpeed, axis);
        printf("relAngle %f %f %f %d\r\n", relAngle[0], relAngle[1], relAngle[2], cycleTime);
        stopMotors();
        printf("angle %d %d\r\n", angle[0], angle[1]);
        printf("lastAngleS %f %f %f %d\r\n", lastAngleDiff[0], lastAngleDiff[1], lastAngleDiff[2], cycleTime);
        printf("lastAngle %d %d\r\n", lastAngle[0], lastAngle[1]);
        printf("PID table %d %d %d\r\n", axisPID[0], axisPID[1], axisPID[2]);
        for (;;);
    }
    lastAngle[axis] = angle[axis];
    lastAngleDiff[axis] = relAngle[axis];
}

static void pidMultiWii(void)
{
    int32_t errorAngle = 0;
    int axis;
    int32_t delta, deltaSum;
    static int32_t delta1[3], delta2[3], acc_delta[3];
    int32_t PTerm, ITerm, DTerm;
    static int32_t lastError[3] = { 0, 0, 0 };
    int32_t AngleRateTmp, RateError;
    int32_t cfgP8[] = {20, 20, 42};
    int32_t cfgI8[] = {30, 30, 45};
    int32_t cfgD8[] = {10, 10, 0};
    int32_t cfgP8PIDLEVEL = 90;

    acc_delta[2] = 0;
    acc_delta[0] = ( desired - angle[ROLL] + 0 ) * 3;
    acc_delta[1] = ( - angle[1] + 0 ) * 3;

    // ----------PID controller----------
    for (axis = 0; axis < 3; axis++) {
        // -----Get the desired angle rate depending on flight mode
        if (axis == 2) { // YAW is always gyro-controlled (MAG correction is applied to rcCommand)
            AngleRateTmp = 0;
        } else {
            // calculate error and limit the angle to 50 degrees max inclination
            errorAngle = - angle[axis] / 10.0f;
            AngleRateTmp = (errorAngle * cfgP8PIDLEVEL) >> 4;
        }

        // --------low-level gyro-based PID. ----------
        // Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
        // -----calculate scaled error.AngleRates
        // multiplication of rcCommand corresponds to changing the sticks scaling here
        RateError = AngleRateTmp - gyroData[axis];
        checkCond(axis, 0);

        // -----calculate P component
        PTerm = (RateError * cfgP8[axis]) >> 7;
        // -----calculate I component
        // there should be no division before accumulating the error to integrator, because the precision would be reduced.
        // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
        // Time correction (to avoid different I scaling for different builds based on average cycle time)
        // is normalized to cycle time = 2048.
        errorGyroI[axis] = errorGyroI[axis] + ((RateError * cycleTime) / 20000) * cfgI8[axis];

        // limit maximum integrator value to prevent WindUp - accumulating extreme values when system is saturated.
        // I coefficient (I8) moved before integration to make limiting independent from PID settings
        errorGyroI[axis] = constrain(errorGyroI[axis], (int32_t)(-GYRO_I_MAX) << 13, (int32_t)(+GYRO_I_MAX) << 13);
        ITerm = errorGyroI[axis] >> 13;

        //-----calculate D-term
        delta = RateError - lastError[axis];  // 16 bits is ok here, the dif between 2 consecutive gyro reads is limited to 800
        lastError[axis] = RateError;

        // Correct difference by cycle time. Cycle time is jittery (can be different 2 times), so calculated difference
        // would be scaled by different dt each time. Division by dT fixes that.
        delta = (delta * ((uint16_t)0xFFFF / (cycleTime >> 4))) >> 6;
        // add moving average here to reduce noise
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;
        DTerm = (deltaSum * cfgD8[axis]) >> 8;

        // -----calculate total PID output
        axisPID[axis] = PTerm + ITerm + DTerm;
        axisPID[axis] *= 100;
    }
}

void loop(void)
{
    static uint32_t rcTime = 0;
    static uint32_t loopTime;
    static uint32_t t;
    uint32_t auxState = 0;
    bool isThrottleLow = false;

    // if ((micros() - t > 100000)) {
    //     t = micros();
    //     Mag_getADC();
    // }

	computeIMU();
	// Measure loop rate just afer reading the sensors
    while ((cycleTime = (int32_t)((int32_t)(currentTime = micros()) - (int32_t)previousTime)) <= 0) {}


    if (throttle < 0) {
        errorGyroI[0] = errorGyroI[1] = errorGyroI[2] = 0;
    }

    if (cycleTime > 20000) {
        previousTime = currentTime;
        pidMultiWii();

        mixTable();
        writeMotors();
    }
}

int main2(void)
{
    //systemInit();

    imuInit(); // Mag is initialized inside imuInit
    mixerInit(); // this will set core.useServo var depending on mixer type

    //pwmInit(0x0);

    previousTime = micros();
    calibratingG = CALIBRATING_GYRO_CYCLES;

    // loopy
    while (1) {
        loop();
        char ch = DEBUGIN();
        switch (ch) {
            case '=': {
                throttle += 100;
                break;
            }
            case '-': {
                stopMotors();
                break;
            }
            case 'w': {
                desired = 70;
                break;
            }
            case 's': {
                desired = 0;
                break;
            }
            case 'g': {
                calibratingG = CALIBRATING_GYRO_CYCLES;
                break;
            }
        }
    }
}

int main(void)
{
    sysInit();

    // init_i2c();
    // read_data(0x0d, 0x0d);
    //
    // uint8_t arr[] = {0x0B, 0x01};
    // uint8_t arr2[] = {QMC5883L_REG_CONF1, QMC5883L_MODE_CONTINUOUS | QMC5883L_ODR_200HZ | QMC5883L_OSR_512 | QMC5883L_RNG_8G};
    // Chip_I2C_MasterSend(I2C0, 0x0d, arr, 2);
    // Chip_I2C_MasterSend(I2C0, 0x0d, arr2, 2);

    // for (;;) {
    //     currentTime = micros();
    //
    //     if ((micros() - t > 100000)) {
    //         t = micros();
    //         Mag_getADC();
    //     }
    // }

	main2();

	gpio_init();
	gpio_output(&gpio_led[0]);
	gpio_output(&gpio_led[1]);
	gpio_output(&gpio_led[2]);

	/* Blink LED1/2/3 on the board. */
	while (1)
	{
		gpio_set(&gpio_led[0]);
		gpio_set(&gpio_led[1]);
		gpio_set(&gpio_led[2]);

		delay(20000);
		gpio_clear(&gpio_led[0]);
		//gpio_clear(&gpio_led[1]);
		gpio_clear(&gpio_led[2]);

		delay(200);
	}

	return 0;
}
