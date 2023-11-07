#include <math.h>
#include "defs.h"

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
