#include "stm32f4xx.h"
#include <math.h>
#include "defs.h"

uint32_t motor[MAX_MOTORS];

#define ENGINE_1_GPIO_PORT GPIOD
#define ENGINE_2_GPIO_PORT GPIOD
#define ENGINE_3_GPIO_PORT GPIOD
#define ENGINE_4_GPIO_PORT GPIOD

#define ENGINE_1_GPIO_PIN GPIO_Pin_12
#define ENGINE_2_GPIO_PIN GPIO_Pin_13
#define ENGINE_3_GPIO_PIN GPIO_Pin_14
#define ENGINE_4_GPIO_PIN GPIO_Pin_15

#define ENGINE_1_GPIO_PIN_SOURCE GPIO_PinSource12
#define ENGINE_2_GPIO_PIN_SOURCE GPIO_PinSource13
#define ENGINE_3_GPIO_PIN_SOURCE GPIO_PinSource14
#define ENGINE_4_GPIO_PIN_SOURCE GPIO_PinSource15

void configure_pwm() {
	GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin    = ENGINE_1_GPIO_PIN | ENGINE_2_GPIO_PIN | ENGINE_3_GPIO_PIN | ENGINE_4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;

	GPIO_Init(ENGINE_1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(ENGINE_1_GPIO_PORT, ENGINE_1_GPIO_PIN_SOURCE, GPIO_AF_TIM4);
	GPIO_PinAFConfig(ENGINE_2_GPIO_PORT, ENGINE_2_GPIO_PIN_SOURCE, GPIO_AF_TIM4);
	GPIO_PinAFConfig(ENGINE_3_GPIO_PORT, ENGINE_3_GPIO_PIN_SOURCE, GPIO_AF_TIM4);
	GPIO_PinAFConfig(ENGINE_4_GPIO_PORT, ENGINE_4_GPIO_PIN_SOURCE, GPIO_AF_TIM4);

    uint8_t mhz = 1;
    uint16_t period = mhz * 1000000 / 50;
	uint16_t PrescalerValue = (uint16_t)(SystemCoreClock / ((uint32_t)mhz * 1000000)) - 1;;

	TIM_TimeBaseInitStructure.TIM_Period = period - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode  = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);
}

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
    static uint32_t ptime = 0;
    static int odd = 0;
    TIM4->CCR1 = motor[0];
    TIM4->CCR2 = motor[1];
    TIM4->CCR3 = motor[2];
    TIM4->CCR4 = motor[3];
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
