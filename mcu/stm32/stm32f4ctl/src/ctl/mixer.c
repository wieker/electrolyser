#include <math.h>
#include "defs.h"

#include <stdio.h>

uint32_t motor[MAX_MOTORS];

typedef struct motorMixer_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;

static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f, -1.0f, -1.0f },          // REAR
    { 1.0f, -1.0f,  1.0f,  1.0f },          // RIGHT
    { 1.0f,  1.0f,  1.0f, -1.0f },          // FRONT
    { 1.0f,  1.0f, -1.0f,  1.0f },          // LEFT
};

static uint8_t numberMotor = 0;

static motorMixer_t currentMixer[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

#define LOW_PWM 1000
#define INIT_THR (-300)

int32_t throttle = INIT_THR;

void mixerResetMotors(void)
{

}

void stopMotors()
{
    int i;
    throttle = INIT_THR;
    for (i = 0; i < numberMotor; i++) {
        motor[i] = LOW_PWM + INIT_THR;
    }
    writeMotors();
}


void mixerInit(void)
{
    numberMotor = 4;
    configure_pwm();
    throttle = INIT_THR;

    mixerResetMotors();
}

void writeMotors(void)
{
    writePWM();
    static uint32_t ptime = 0;
    static int odd = 0;
    uint32_t ctime = millis();
    if ((ctime - ptime) > 1000) {
        //printf("abs %f %f %f / %d\r\n", absAngle[0], absAngle[0], absAngle[0], cycles);
        printf("angle %d %d %d\r\n", angle[0], angle[1], heading);
        printf("desired %d %d\r\n", desiredX, desiredY);
        //printf("relAngle %f %f %f d\r\n", relAngle[0], relAngle[1], relAngle[2]);
        //printf("angle ACC %d %d\r\n", angleACC[0], angleACC[1]);
        printf("gyroADC %d %d %d\r\n", gyroADC[0], gyroADC[1], gyroADC[2]);
        printf("mixer motor %d %d %d %d\r\n", motor[0], motor[1], motor[2], motor[3]);
        printf("vibr motor %f %f %f\r\n", cumulativeG, cumulativeX, cumulativeY);
        // printf("OK %d %s %d => %d %d\r\n", ctime,
        //        odd == 0 ? "X" : "Y", odd == 0 ? desiredX : desiredY,
        //        (int32_t) angle[odd],
        //        (int)(100 * cumulativeG));
        odd = (odd + 1) % 2;
        cycles = 0;
        ptime = ctime;
        cumulativeG = 0.0f;
        cumulativeX = 0.0f;
        cumulativeY = 0.0f;
    }
}

int32_t axisPID[3];

void mixTable(void)
{
    int16_t maxMotor;
    uint32_t i;

    // motors for non-servo mixes
    if (numberMotor > 1) {
        for (i = 0; i < numberMotor; i++) {
            motor[i] = LOW_PWM + throttle * mixerQuadX[i].throttle + axisPID[PITCH] * mixerQuadX[i].pitch + axisPID[ROLL] * mixerQuadX[i].roll + axisPID[YAW] * mixerQuadX[i].yaw;
            //motor[i] = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25;
            //DEBUGOUT("PWM write %d: %d %f %f %f\r\n", i, throttle, axisPID[PITCH] * mixerQuadX[i].pitch,
            //         axisPID[ROLL] * mixerQuadX[i].roll, axisPID[YAW] * mixerQuadX[i].yaw);
        }
    }
}
