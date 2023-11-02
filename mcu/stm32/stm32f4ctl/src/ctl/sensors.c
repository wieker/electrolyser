#include <math.h>
#include <stdint.h>
#include "defs.h"
#include <stdio.h>

int16_t gyroADC[3];
uint16_t calibratingG = 0;
int16_t gyroZero[3] = { 0, 0, 0 };
int16_t accADC[3];
int16_t accZero[3] = { 0, 0, 0 };
int16_t accSmooth[3];
int head;

void imuInit(void)
{
    icm20689_init();
}

static void mpuGyroRead(int16_t *gyroData)
{
    icm20689_read(gyroData);
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

static void applyGyroAccCalibration(void)
{
    int axis;
    static int32_t g[3];
    static int32_t v[3];
    static stdev_t var[3];

    if (calibratingG > 0) {
        for (axis = 0; axis < 3; axis++) {
            // Reset g[axis] at start of calibration
            if (calibratingG == CALIBRATING_GYRO_CYCLES) {
                g[axis] = 0;
                v[axis] = 0;
                devClear(&var[axis]);
            }
            // Sum up 1000 readings
            g[axis] += gyroADC[axis];
            v[axis] += accADC[axis];
            devPush(&var[axis], gyroADC[axis]);
            // Clear global variables for next reading
            gyroADC[axis] = 0;
            gyroZero[axis] = 0;
            accADC[axis] = 0;
            accZero[axis] = 0;
            if (calibratingG == 1) {
                float dev = devStandardDeviation(&var[axis]);
                gyroZero[axis] = (g[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
                accZero[axis] = (v[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
            }
        }
        calibratingG--;
    }
    for (axis = 0; axis < 3; axis++)
        gyroADC[axis] -= gyroZero[axis];
    accADC[2] -= 6;
    accADC[1] += 7;
    accADC[0] -= 9;
}

void Gyro_getADC(void)
{
    mpuGyroRead(gyroADC);
    applyGyroAccCalibration();
}

void computeIMU(void)
{
    Gyro_getADC();

    getEstimatedAttitude();

}
