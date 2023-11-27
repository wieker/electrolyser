#include "defs.h"


uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0;

int32_t errorGyroI[3] = { 0, 0, 0 };

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
    acc_delta[0] = 10 + desiredX;
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
                errorAngle = acc_delta[axis] - angle[axis];
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

void loop(void)
{
    if (throttle < 0) {
        errorGyroI[0] = errorGyroI[1] = errorGyroI[2] = 0;
    }

    computeIMU();
	// Measure loop rate just afer reading the sensors
    while ((cycleTime = (int32_t)((int32_t)(currentTime = micros()) - (int32_t)previousTime)) <= 0) {}

    if (cycleTime > 10000) {
        previousTime = currentTime;
        pidMultiWii();

        mixTable();
        writeMotors();
    }
}

int main_loop(void)
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
        manual();
    }
}
