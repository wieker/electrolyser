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
#include "board_api.h"

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

#define CALIBRATING_GYRO_CYCLES             1000

enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
};

int16_t gyroData[3] = { 0, 0, 0 };
int16_t gyroADC[3];
uint16_t calibratingG = 0;
int16_t gyroZero[3] = { 0, 0, 0 };
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0;


void imuInit(void)
{
}

typedef struct stdev_t {
    float m_oldM, m_newM, m_oldS, m_newS;
    int m_n;
} stdev_t;

static void devClear(stdev_t *dev)
{
    dev->m_n = 0;
}

static void devPush(stdev_t *dev, float x)
{
    dev->m_n++;
    if (dev->m_n == 1) {
        dev->m_oldM = dev->m_newM = x;
        dev->m_oldS = 0.0f;
    } else {
        dev->m_newM = dev->m_oldM + (x - dev->m_oldM) / dev->m_n;
        dev->m_newS = dev->m_oldS + (x - dev->m_oldM) * (x - dev->m_newM);
        dev->m_oldM = dev->m_newM;
        dev->m_oldS = dev->m_newS;
    }
}

static float devVariance(stdev_t *dev)
{
    return ((dev->m_n > 1) ? dev->m_newS / (dev->m_n - 1) : 0.0f);
}

static float devStandardDeviation(stdev_t *dev)
{
    return sqrtf(devVariance(dev));
}

static void GYRO_Common(void)
{
    int axis;
    static int32_t g[3];
    static stdev_t var[3];

    if (calibratingG > 0) {
        for (axis = 0; axis < 3; axis++) {
            // Reset g[axis] at start of calibration
            if (calibratingG == CALIBRATING_GYRO_CYCLES) {
                g[axis] = 0;
                devClear(&var[axis]);
            }
            // Sum up 1000 readings
            g[axis] += gyroADC[axis];
            devPush(&var[axis], gyroADC[axis]);
            // Clear global variables for next reading
            gyroADC[axis] = 0;
            gyroZero[axis] = 0;
            if (calibratingG == 1) {
                float dev = devStandardDeviation(&var[axis]);
                gyroZero[axis] = (g[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
            }
        }
        calibratingG--;
    }
    for (axis = 0; axis < 3; axis++)
        gyroADC[axis] -= gyroZero[axis];
}

void Gyro_getADC(void)
{
    // range: +/- 8192; +/- 2000 deg/sec
    //gyro.read(gyroADC);
    GYRO_Common();
}

void computeIMU(void)
{
    static int16_t gyroYawSmooth = 0;

    Gyro_getADC();

    gyroData[YAW] = gyroADC[YAW];
    gyroData[ROLL] = gyroADC[ROLL];
    gyroData[PITCH] = gyroADC[PITCH];
}

typedef struct motorMixer_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;

static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f,  1.0f, -1.0f },          // REAR_R
    { 1.0f, -1.0f, -1.0f,  1.0f },          // FRONT_R
    { 1.0f,  1.0f,  1.0f,  1.0f },          // REAR_L
    { 1.0f,  1.0f, -1.0f, -1.0f },          // FRONT_L
};
#define MAX_MOTORS             4

static uint8_t numberMotor = 0;

static motorMixer_t currentMixer[MAX_MOTORS];
int16_t motor[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

void mixerResetMotors(void)
{
    int i;
    // set disarmed motor values
    for (i = 0; i < MAX_MOTORS; i++)
        motor_disarmed[i] = 0;
}

void mixerInit(void)
{
    int i;
    numberMotor = 4;
    // copy motor-based mixers
	for (i = 0; i < numberMotor; i++)
		currentMixer[i] = mixerQuadX[i];

    mixerResetMotors();
}

void writeMotors(void)
{
    uint8_t i;

    for (i = 0; i < numberMotor; i++) {
		//pwmWriteMotor(i, motor[i]);
	}
}

int16_t axisPID[3];

void mixTable(void)
{
    int16_t maxMotor;
    uint32_t i;

    // motors for non-servo mixes
    if (numberMotor > 1) {
        for (i = 0; i < numberMotor; i++) {
            motor[i] = axisPID[PITCH] * currentMixer[i].pitch + axisPID[ROLL] * currentMixer[i].roll + axisPID[YAW] * currentMixer[i].yaw;
        }
    }
}

static int32_t errorGyroI[3] = { 0, 0, 0 };
static int32_t errorAngleI[2] = { 0, 0 };

static void pidMultiWii(void)
{
    int axis, prop;
    int32_t error, errorAngle;
    int32_t PTerm, ITerm, PTermACC = 0, ITermACC = 0, PTermGYRO = 0, ITermGYRO = 0, DTerm;
    static int16_t lastGyro[3] = { 0, 0, 0 };
    static int32_t delta1[3], delta2[3];
    int32_t deltaSum;
    int32_t delta;

    // **** PITCH & ROLL & YAW PID ****
    for (axis = 0; axis < 3; axis++) {
		errorGyroI[axis] -= gyroData[axis];
		ITermGYRO = (errorGyroI[axis] / 125) >> 6;
		PTerm = PTermGYRO;
		ITerm = ITermGYRO;

        PTerm -= (int32_t)gyroData[axis] / 10 / 8; // 32 bits is needed for calculation
        delta = gyroData[axis] - lastGyro[axis];
        lastGyro[axis] = gyroData[axis];
        deltaSum = delta1[axis] + delta2[axis] + delta;
        delta2[axis] = delta1[axis];
        delta1[axis] = delta;
        DTerm = (deltaSum) / 32;
        axisPID[axis] = PTerm + ITerm - DTerm;
    }
}

void loop(void)
{
    static uint32_t rcTime = 0;
    static uint32_t loopTime;
    uint32_t auxState = 0;
    bool isThrottleLow = false;

    //currentTime = micros();

	computeIMU();
	// Measure loop rate just afer reading the sensors
//	currentTime = micros();
	cycleTime = (int32_t)(currentTime - previousTime);
	previousTime = currentTime;

	pidMultiWii();

	mixTable();
	writeMotors();
}

int main2(void)
{
    //systemInit();

    imuInit(); // Mag is initialized inside imuInit
    mixerInit(); // this will set core.useServo var depending on mixer type

    //pwmInit(0x0);

    //previousTime = micros();
    calibratingG = CALIBRATING_GYRO_CYCLES;

    // loopy
    while (1) {
        loop();
    }
}

int main(void)
{
    DEBUGINIT();
	DEBUGOUT("Main enter\r\n");
	main_ssp();
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
