
#pragma once



enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
};
typedef enum {
    X = 0,
    Y,
    Z
} sensor_axis_e;

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif /* M_PI */

#define RADX10 (M_PI / 1800.0f)                  // 0.001745329252f
#define RAD    (M_PI / 180.0f)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))

#define CALIBRATING_GYRO_CYCLES             1000

extern int16_t angle[2];
extern int16_t angleACC[2];
extern int16_t angleGYR[2];
extern int32_t axisPID[3];
extern int16_t heading;
extern uint16_t acc_1G;
extern float absAngle[3];
extern float relAngle[3];

extern uint32_t currentTime;
extern uint16_t calibratingG;
extern int16_t gyroData[3];
extern int16_t magADC[3];
extern int16_t accADC[3];

extern int32_t throttle;

extern float accAbsAngle[2];
extern int cycles;

extern int16_t gyroADC[3], accADC[3], accSmooth[3], magADC[3];

uint32_t micros(void);
void getEstimatedAttitude(void);

void computeIMU(void);
void imuInit(void);

void mixerInit();
void mixTable();
void writeMotors();
void stopMotors();

extern void sysInit();
uint32_t micros(void);
uint32_t millis(void);
