#include "board.h"
#include <math.h>
#include "defs.h"

#define SCT_PWM            LPC_SCT
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

#define LOW_PWM 100000
#define INIT_THR (-30000)

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
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR1, LOW_PWM);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR2, LOW_PWM);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR3, LOW_PWM);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR4, LOW_PWM);
	Chip_SCTPWM_Start(SCT_PWM);
}

typedef struct motorMixer_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;

static const motorMixer_t mixerQuadX[] = {
    { 1.0f, -1.0f,  0.0f,  1.0f },          // REAR
    { 1.0f,  0.0f,  1.0f, -1.0f },          // RIGHT
    { 1.0f,  1.0f,  0.0f,  1.0f },          // FRONT
    { 1.0f,  0.0f, -1.0f, -1.0f },          // LEFT
};

static uint8_t numberMotor = 0;

static motorMixer_t currentMixer[MAX_MOTORS];
int16_t motor_disarmed[MAX_MOTORS];

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
    static uint32_t ptime = 0;
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR1, motor[0]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR2, motor[1]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR3, motor[2]);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_MTR4, motor[3]);
    uint32_t ctime = millis();
    if ((ctime - ptime) > 1000) {
        //printf("abs %f %f %f / %d\r\n", absAngle[0], absAngle[0], absAngle[0], cycles);
        //printf("angle %d %d %d\r\n", angle[0], angle[1], heading);
        //printf("relAngle %f %f %f d\r\n", relAngle[0], relAngle[1], relAngle[2]);
        //printf("angle ACC %d %d\r\n", angleACC[0], angleACC[1]);
        //printf("mixer motor %d %d %d %d\r\n", motor[0], motor[1], motor[2], motor[3]);
        Board_UARTPutSTR("K");
        cycles = 0;
        ptime = ctime;
        absAngle[0] = absAngle[1] = absAngle[2] = 0;
        relAngle[0] = relAngle[1] = relAngle[2] = 0;
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
