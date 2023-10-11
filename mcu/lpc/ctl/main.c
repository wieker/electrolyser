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
int32_t errorP[3] = { 0, 0, 0 };
int32_t wannaP[3] = { 0, 0, 0 };
static int32_t errorAngleI[2] = { 0, 0 };

#define GYRO_I_MAX 5000

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
int32_t desiredX = 0;
int32_t desiredY = 0;
int32_t acc_delta[3];
int32_t lastAngle[3];
float lastAngleDiff[3];

void checkCond(int axis, int32_t angleSpeed) {
    if (throttle < 0) {
        return;
    }
    if (abs(angle[ROLL]) > 100 || abs(angle[1]) > 100) {
        printf("abs %d %d %d\r\n", accADC[0], accADC[1], accADC[2]);
        printf("mag %d %d %d\r\n", magADC[YAW], magADC[ROLL], magADC[PITCH]);
        printf("acc %d %d %d\r\n", accADC[YAW], accADC[ROLL], accADC[PITCH]);
        printf("rotate %d %d\r\n", angleSpeed, axis);
        printf("relAngle %f %f %f %d\r\n", relAngle[0], relAngle[1], relAngle[2], cycleTime);
        stopMotors();
        printf("lastAngleS %f %f %f %d\r\n", lastAngleDiff[0], lastAngleDiff[1], lastAngleDiff[2], cycleTime);
        printf("lastAngle %d %d\r\n", lastAngle[0], lastAngle[1]);
        printf("angle %d %d %d\r\n", angle[0], angle[1], heading);
        printf("relAngle %f %f %f d\r\n", relAngle[0], relAngle[1], relAngle[2]);
        printf("angle ACC %d %d\r\n", angleACC[0], angleACC[1]);
        printf("mixer motor %d %d %d %d\r\n", motor[0], motor[1], motor[2], motor[3]);
        printf("mixer PID %d %d %d\r\n", axisPID[0], axisPID[1], axisPID[2]);
        printf("mixer PIDI %d %d %d\r\n", errorGyroI[0] >> 13, errorGyroI[1] >> 13, errorGyroI[2] >> 13);
        printf("mixer PIDP %d %d %d\r\n", errorP[0], errorP[1], errorP[2]);
        for (;;);
    }
    lastAngle[axis] = angle[axis];
    lastAngleDiff[axis] = relAngle[axis];
}

float lastSpeed[3];
float accMomLPF[3] = {0.0f, 0.0f, 0.0f};

void calcAngleSpeed(int axis, int32_t a) {
    float angleSpeed = relAngle[axis] * 1000000 / cycleTime;
    float angleAccel = (angleSpeed - lastSpeed[axis]) * 1000000 / cycleTime;
    lastSpeed[axis] = angleSpeed;
    float angleMoment = errorP[axis] == 0 ? 0 : angleAccel / errorP[axis];
    accMomLPF[axis] = accMomLPF[axis] * (1.0f - (1.0f / 50)) + angleMoment * (1.0f / 50);
}

static void pidMultiWii(void)
{
    int32_t errorAngle = 0;
    int axis;
    int32_t delta, deltaSum;
    static int32_t delta1[3], delta2[3];
    int32_t PTerm, ITerm, DTerm;
    static int32_t lastError[3] = { 0, 0, 0 };
    int32_t AngleRateTmp, RateError;
    int32_t cfgP8 = 200;
    int32_t cfgI8 = 200;
    int32_t cfgD8 = 100;
    int32_t cfgP8PIDLEVEL = 15;

    acc_delta[2] = 0;
    //acc_delta[0] = 10 * desiredX - angle[0];
    //acc_delta[1] = 10 * desiredY - angle[1];
    acc_delta[0] = desiredX;
    acc_delta[1] = desiredY;

    // ----------PID controller----------
    for (axis = 0; axis < 3; axis++) {
        // -----Get the desired angle rate depending on flight mode
        if (axis == 2) { // YAW is always gyro-controlled (MAG correction is applied to rcCommand)
            AngleRateTmp = 0;
        } else {
            // calculate error and limit the angle to 50 degrees max inclination
            if (cMode == 3) {
                AngleRateTmp = 0;
            } else if (cMode == 4) {
                AngleRateTmp = acc_delta[axis] * 4;
            } else if (cMode == 5) {
                AngleRateTmp = gyroADC[axis];
            } else {
                errorAngle = 10 * acc_delta[axis] - angle[axis];
                AngleRateTmp = (errorAngle * cfgP8PIDLEVEL) >> 4;
            }
        }

        // --------low-level gyro-based PID. ----------
        // Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
        // -----calculate scaled error.AngleRates
        // multiplication of rcCommand corresponds to changing the sticks scaling here
        RateError = AngleRateTmp - gyroADC[axis];

        //checkCond(axis, 0);
        //calcAngleSpeed(axis, 0);

        // -----calculate P component
        PTerm = (RateError * cfgP8) >> 7;
        errorP[axis] = PTerm;
        // -----calculate I component
        // there should be no division before accumulating the error to integrator, because the precision would be reduced.
        // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
        // Time correction (to avoid different I scaling for different builds based on average cycle time)
        // is normalized to cycle time = 2048.
        errorGyroI[axis] = errorGyroI[axis] + ((RateError * cycleTime) / 10000) * cfgI8;

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
        DTerm = (deltaSum * cfgD8) >> 8;

        // -----calculate total PID output
        axisPID[axis] = PTerm + ITerm + DTerm;
        axisPID[axis] *= 10;
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

    if (cycleTime > 10000) {
        previousTime = currentTime;
        pidMultiWii();

        mixTable();
        writeMotors();
    }
}
int chState = 0;

int main2(void)
{
    //systemInit();

    imuInit(); // Mag is initialized inside imuInit
    mixerInit(); // this will set core.useServo var depending on mixer type

    //pwmInit(0x0);

    previousTime = micros();
    calibratingG = CALIBRATING_GYRO_CYCLES;

    int32_t ping_time;
    int32_t prev_time;
    int32_t chTime;
    // loopy
    while (1) {
        loop();
        char ch = DEBUGIN();
        switch (ch) {
            case '=': {
                throttle += 5000;
                //chState = 1;
                break;
            }
            case '-': {
                throttle -= 1000;
                break;
            }
            case '0': {
                stopMotors();
                chState = 0;
                break;
            }
            case 'p': {
                prev_time = millis();
                break;
            }
            case 'a': {
                desiredX --;
                break;
            }
            case 's': {
                desiredY --;
                break;
            }
            case 'd': {
                desiredX ++;
                break;
            }
            case 'w': {
                desiredY ++;
                break;
            }
            case '1': {
                cMode = 0;
                break;
            }
            case '2': {
                cMode = 1;
                break;
            }
            case '3': {
                cMode = 2;
                break;
            }
            case '4': {
                cMode = 3;
                break;
            }
            case '5': {
                cMode = 4;
                break;
            }
            case '6': {
                cMode = 5;
                break;
            }
            case 'g': {
                calibratingG = CALIBRATING_GYRO_CYCLES;
                break;
            }
            case 'v': {
                throttle = 0;
                break;
            }
            case 'm': {
                throttle = 30000;
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
