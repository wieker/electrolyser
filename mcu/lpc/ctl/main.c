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

#include "icm20689/accgyro_icm20689.h"
#include "icm20689/accgyro_mpu.h"

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

// cycles per microsecond
static volatile uint32_t usTicks = 0;
// current uptime for 1kHz systick timer. will rollover after 49 days. hopefully we won't care.
static volatile uint32_t sysTickUptime = 0;

// SysTick
void SysTick_Handler(void)
{
    sysTickUptime++;
}

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

#define LPC_SSP           LPC_SSP1
#define SSP_DATA_BITS                       (SSP_BITS_8)

uint8_t Rx_Buf[256];
uint8_t Tx_Buf[256];
static const uint8_t WHOAMI = 0x98;
static const uint8_t WHO_AM_I = 0x75;
static const uint8_t PWR_MGMT_1 = 0x6B;
static const uint8_t DIR_READ = 0x80;

uint8_t spi_xfer(uint8_t reg, uint8_t dt) {
	static Chip_SSP_DATA_SETUP_T xf_setup;
	xf_setup.length = 2;
	xf_setup.tx_data = Tx_Buf;
	Tx_Buf[0] = reg;
	Tx_Buf[1] = dt;
	Tx_Buf[2] = 0x04;
	Tx_Buf[2] = 0x08;
	xf_setup.rx_data = Rx_Buf;
	xf_setup.rx_cnt = xf_setup.tx_cnt = 0;
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) false);
	Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) true);
    return Rx_Buf[1];
}

uint8_t* spi_xfer6(uint8_t reg) {
	static Chip_SSP_DATA_SETUP_T xf_setup;
	xf_setup.length = 7;
	xf_setup.tx_data = Tx_Buf;
	Tx_Buf[0] = reg;
	Tx_Buf[1] = 0x00;
	Tx_Buf[2] = 0x00;
	Tx_Buf[2] = 0x00;
	xf_setup.rx_data = Rx_Buf;
	xf_setup.rx_cnt = xf_setup.tx_cnt = 0;
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) false);
	Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) true);
    return &Rx_Buf[1];
}


void imuInit(void)
{
	/* SSP initialization */
	Board_SSP_Init(LPC_SSP);

	Chip_SSP_Init(LPC_SSP);

	Chip_SSP_SetFormat(LPC_SSP, SSP_DATA_BITS, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_MODE0);

	Chip_SSP_Enable(LPC_SSP);
	Chip_SSP_SetMaster(LPC_SSP, 1);


    spi_xfer(PWR_MGMT_1, 0x80);
    delay(20000000);
    spi_xfer(WHO_AM_I | DIR_READ, 0x80);
    delay(20000000);
    DEBUGOUT("PWM write res=%02x expected=%02x\r\n", Rx_Buf[1], WHOAMI);
    spi_xfer(MPU_RA_SIGNAL_PATH_RESET, 0x03);
    delay(20000000);
    spi_xfer(MPU_RA_PWR_MGMT_1, INV_CLK_PLL);
    delay(20000000);
    spi_xfer(MPU_RA_GYRO_CONFIG, INV_FSR_2000DPS << 3);
    delay(200);
    spi_xfer(MPU_RA_ACCEL_CONFIG, INV_FSR_16G << 3);
    delay(200);
    spi_xfer(MPU_RA_CONFIG, MPU_DLPF_256HZ);
    delay(200);
    spi_xfer(MPU_RA_SMPLRT_DIV, 15);
    delay(200);
}

static void mpuGyroRead(int16_t *gyroData)
{
    //delay(20000000);
    //printf("test %02x\r\n", spi_xfer(WHO_AM_I | DIR_READ, 0x00));
    uint8_t* val = spi_xfer6(MPU_RA_GYRO_XOUT_H | DIR_READ);
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    gyroData[0] = (int16_t)((val[0] << 8) | val[1]) / 4;
    gyroData[1] = (int16_t)((val[2] << 8) | val[3]) / 4;
    gyroData[2] = (int16_t)((val[4] << 8) | val[5]) / 4;
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
    mpuGyroRead(gyroADC);
    GYRO_Common();
}

void computeIMU(void)
{
    static int16_t gyroYawSmooth = 0;
    static int cnt = 0;

    Gyro_getADC();

    gyroData[YAW] = gyroADC[YAW];
    gyroData[ROLL] = gyroADC[ROLL];
    gyroData[PITCH] = gyroADC[PITCH];

    if (cnt == 100) {
        printf("test %d %d %d\r\n", gyroData[YAW], gyroData[ROLL], gyroData[PITCH]);
        cnt = 0;
    } else {
        cnt ++;
    }
}

void SCT_PinsConfigure(void)
{
	Chip_SCU_PinMuxSet(0x2, 7, (SCU_MODE_INACT | SCU_MODE_FUNC1));
	Chip_SCU_PinMuxSet(0x2, 12, (SCU_MODE_INACT | SCU_MODE_FUNC1));
}

#define SCT_PWM            LPC_SCT

#define SCT_PWM_PIN_LED    1        /* COUT2 [index 2] Controls LED */

#define SCT_PWM_LED        2        /* Index of LED PWM */
#define SCT_PWM_RATE   50        /* PWM frequency 10 KHz */

/* Systick timer tick rate, to change duty cycle */
#define TICKRATE_HZ     1000        /* 1 ms Tick rate */

#define LED_STEP_CNT      10000        /* Change LED duty cycle every 20ms */

void configure_pwm() {
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, SCT_PWM_RATE);

	/* Setup Board specific output pin */
	SCT_PinsConfigure();

	/* Use SCT0_OUT1 pin */
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_LED, SCT_PWM_PIN_LED);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_LED, 4);

	/* Start with 0% duty cycle */
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25);
	Chip_SCTPWM_Start(SCT_PWM);
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
uint32_t motor[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

uint32_t throttle = 0;

void mixerResetMotors(void)
{

}

void mixerInit(void)
{
    numberMotor = 4;
    configure_pwm();
    throttle = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25;

    mixerResetMotors();
}

void writeMotors(void)
{
    static uint32_t oldv = 0;
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, motor[0]);
    if (oldv != motor[0]) {
        DEBUGOUT("PWM write %08x\r\n", motor[0]);
    }
    oldv = motor[0];
}

int16_t axisPID[3];

void mixTable(void)
{
    int16_t maxMotor;
    uint32_t i;

    // motors for non-servo mixes
    if (numberMotor > 1) {
        for (i = 0; i < numberMotor; i++) {
            motor[i] = throttle;// + axisPID[PITCH] * mixerQuadX[i].pitch + axisPID[ROLL] * mixerQuadX[i].roll + axisPID[YAW] * mixerQuadX[i].yaw;
        }
    }
}

static int32_t errorGyroI[3] = { 0, 0, 0 };
static int32_t errorAngleI[2] = { 0, 0 };

#define GYRO_I_MAX 256

int constrain(int amt, int low, int high)
{
    if (amt < low)
        return low;
    else if (amt > high)
        return high;
    else
        return amt;
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
    int32_t cfgP8[] = {40, 40, 85};
    int32_t cfgI8[] = {30, 30, 45};
    int32_t cfgD8[] = {23, 23, 0};

    // ----------PID controller----------
    for (axis = 0; axis < 3; axis++) {
        // -----Get the desired angle rate depending on flight mode
        AngleRateTmp = 0;
        // --------low-level gyro-based PID. ----------
        // Used in stand-alone mode for ACRO, controlled by higher level regulators in other modes
        // -----calculate scaled error.AngleRates
        // multiplication of rcCommand corresponds to changing the sticks scaling here
        RateError = AngleRateTmp - gyroData[axis];

        // -----calculate P component
        PTerm = (RateError * cfgP8[axis]) >> 7;
        // -----calculate I component
        // there should be no division before accumulating the error to integrator, because the precision would be reduced.
        // Precision is critical, as I prevents from long-time drift. Thus, 32 bits integrator is used.
        // Time correction (to avoid different I scaling for different builds based on average cycle time)
        // is normalized to cycle time = 2048.
        errorGyroI[axis] = errorGyroI[axis] + ((RateError * cycleTime) >> 11) * cfgI8[axis];

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
    }
}

void loop(void)
{
    static uint32_t rcTime = 0;
    static uint32_t loopTime;
    uint32_t auxState = 0;
    bool isThrottleLow = false;

    currentTime = micros();

	computeIMU();
	// Measure loop rate just afer reading the sensors
	currentTime = micros();
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
                throttle -= 100;
                break;
            }
        }
    }
}

int main(void)
{
	uint32_t timerFreq;

    DEBUGINIT();
	DEBUGOUT("Main enter\r\n");
	Board_Init();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);
    usTicks = SystemCoreClock / 1000000;

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
