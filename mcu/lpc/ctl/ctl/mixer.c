#include "board.h"
#include <math.h>
#include "defs.h"

#define SCT_PWM            LPC_SCT
#define MAX_MOTORS             4
uint32_t motor[MAX_MOTORS];

void SCT_PinsConfigure(void)
{
	Chip_SCU_PinMuxSet(0x2, 7, (SCU_MODE_INACT | SCU_MODE_FUNC1));
	Chip_SCU_PinMuxSet(0x2, 12, (SCU_MODE_INACT | SCU_MODE_FUNC1));
	Chip_SCU_PinMuxSet(0x1, 7, (SCU_MODE_INACT | SCU_MODE_FUNC2));
	Chip_SCU_PinMuxSet(0x2, 9, (SCU_MODE_INACT | SCU_MODE_FUNC1));
	Chip_SCU_PinMuxSet(0x1, 8, (SCU_MODE_INACT | SCU_MODE_FUNC2));
}

#define SCT_PWM_PIN_LED    1        /* COUT2 [index 2] Controls LED */

#define SCT_PWM_MTR1        2        /* Index of RIGHT PWM */
#define SCT_PWM_MTR2        3        /* Index of REAR PWM */
#define SCT_PWM_MTR3        4        /* Index of LEFT PWM */
#define SCT_PWM_MTR4        5        /* Index of LEFT PWM */
#define SCT_PWM_RATE   50        /* PWM frequency 10 KHz */

#define LED_STEP_CNT      10000        /* Change LED duty cycle every 20ms */

void configure_pwm() {
	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, SCT_PWM_RATE);

	/* Setup Board specific output pin */
	SCT_PinsConfigure();

	/* Use SCT0_OUT1 pin */
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_MTR1, SCT_PWM_PIN_LED);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_MTR1, 4);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_MTR2, 3);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_MTR3, 13);
	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_MTR4, 12);

	/* Start with 0% duty cycle */
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR1, Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR2, Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR3, Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR4, Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25);
	Chip_SCTPWM_Start(SCT_PWM);
}

typedef struct motorMixer_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;

static const motorMixer_t mixerQuadX[] = {
    { 1.0f,  0.0f,  1.0f, -1.6f },          // RIGHT
    { 1.0f, -1.0f,  0.0f,  0.8f },          // REAR
    { 1.0f,  0.0f, -1.0f, -1.6f },          // LEFT
    { 1.0f,  2.0f,  0.0f,  0.8f },          // FRONT
};

static uint8_t numberMotor = 0;

static motorMixer_t currentMixer[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

uint32_t throttle = 0;

void mixerResetMotors(void)
{

}

void stopMotors()
{
    throttle = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25;
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
    static uint32_t ptime = 0;
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR1, motor[0]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR2, motor[1]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR3, motor[2]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR4, motor[3]);
    uint32_t ctime = millis();
    //printf("test %d\r\n", ctime);
    if ((ctime - ptime) > 1000) {
        printf("gyr %d %d %d\r\n", gyroData[YAW], gyroData[ROLL], gyroData[PITCH]);
        printf("mag %d %d %d\r\n", magADC[YAW], magADC[ROLL], magADC[PITCH]);
        printf("acc %d %d %d\r\n", accADC[YAW], accADC[ROLL], accADC[PITCH]);
        int low = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/20;
        DEBUGOUT("PWM write to motor %d %d\r\n", throttle, motor[0] - throttle);
        DEBUGOUT("PWM PID value %d %d\r\n", low, axisPID[0]);
        printf("calculated S A G %d %d %d\r\n", angle[0] / 10, angleACC[0] / 10, angleGYR[0] / 10);
        printf("abs %f rel %f\r\n", absAngle[0], relAngle[0]);
        ptime = ctime;
        //absAngle[0] = absAngle[1] = absAngle[2] = 0;
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
            //motor[i] = throttle + axisPID[PITCH] * mixerQuadX[i].pitch + axisPID[ROLL] * mixerQuadX[i].roll + axisPID[YAW] * mixerQuadX[i].yaw;
            motor[i] = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM)/25;
            //DEBUGOUT("PWM write %d: %d %f %f %f\r\n", i, throttle, axisPID[PITCH] * mixerQuadX[i].pitch,
            //         axisPID[ROLL] * mixerQuadX[i].roll, axisPID[YAW] * mixerQuadX[i].yaw);
        }
    }

    motor[0] = throttle - 3.0f * axisPID[0];
    motor[2] = throttle + 0.5f * axisPID[0];
}