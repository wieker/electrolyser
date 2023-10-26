#include "stm32f4xx.h"

extern "C" {

#include "ctl/defs.h"


uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0;

int32_t errorGyroI[3] = { 0, 0, 0 };
int32_t errorP[3] = { 0, 0, 0 };
int32_t wannaP[3] = { 0, 0, 0 };
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
int32_t desiredX = 0;
int32_t desiredY = 0;
int32_t acc_delta[3];
int32_t lastAngle[3];
float lastAngleDiff[3];

float lastSpeed[3];
float accMomLPF[3] = {0.0f, 0.0f, 0.0f};

static void pidMultiWii(void)
{
    int32_t errorAngle = 0;
    int axis;
    int32_t delta, deltaSum;
    static int32_t delta1[3], delta2[3];
    int32_t PTerm, ITerm, DTerm;
    static int32_t lastError[3] = { 0, 0, 0 };
    int32_t AngleRateTmp, RateError;
    int32_t cfgP8 = 20;
    int32_t cfgI8 = 20;
    int32_t cfgD8 = 10;
    int32_t cfgP8PIDLEVEL = 9;

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
    }
}

int32_t startTime;
int32_t chState = 0;

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

void parse_ctl() {

        char ch = 's';
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
                chState = 1;
                startTime = millis();
                break;
            }
        }
}

void logic() {
    if ((chState == 1) && (millis() - startTime > 300)) {
        throttle = 35000;
        chState ++;
    }
    if ((chState == 2) && (millis() - startTime > 600)) {
        throttle = 40000;
        chState ++;
    }
    if ((chState == 3) && (millis() - startTime > 2600)) {
        throttle = 35000;
        chState ++;
    }
    if ((chState == 4) && (millis() - startTime > 3600)) {
        throttle = 30000;
        chState ++;
    }
    if (millis() - startTime > 5000) {
        stopMotors();
        chState = 0;
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

    int32_t ping_time;
    int32_t prev_time;
    int32_t chTime;
    // loopy
    while (1) {
        loop();
        parse_ctl();
        logic();
    }
}
}

extern "C" int main()
{
  // Switch on blue LED on STM32F407Discovery
  sysInit();

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio_out = {
    .GPIO_Pin = GPIO_Pin_13,
    .GPIO_Mode = GPIO_Mode_OUT,
    .GPIO_Speed = GPIO_High_Speed,
    .GPIO_OType = GPIO_OType_PP,
    .GPIO_PuPd = GPIO_PuPd_NOPULL
  };

  GPIO_Init(GPIOC, &gpio_out);
  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);

  main2();

   while (1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    int count = millis();
    while ((millis() - count) < 1000) ;
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    count = millis();
    while ((millis() - count) < 1000) ;
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }

  return 0;
}
