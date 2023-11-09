#include "stm32f4xx.h"

#include "../ctl/defs.h"


#define ENGINE_1_GPIO_PORT GPIOA
#define ENGINE_2_GPIO_PORT GPIOA
#define ENGINE_3_GPIO_PORT GPIOA
#define ENGINE_4_GPIO_PORT GPIOA

#define ENGINE_1_GPIO_PIN GPIO_Pin_0
#define ENGINE_2_GPIO_PIN GPIO_Pin_1
#define ENGINE_3_GPIO_PIN GPIO_Pin_2
#define ENGINE_4_GPIO_PIN GPIO_Pin_3

#define ENGINE_1_GPIO_PIN_SOURCE GPIO_PinSource0
#define ENGINE_2_GPIO_PIN_SOURCE GPIO_PinSource1
#define ENGINE_3_GPIO_PIN_SOURCE GPIO_PinSource2
#define ENGINE_4_GPIO_PIN_SOURCE GPIO_PinSource3

void configure_pwm() {
	GPIO_InitTypeDef            GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef           TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin    = ENGINE_1_GPIO_PIN | ENGINE_2_GPIO_PIN | ENGINE_3_GPIO_PIN | ENGINE_4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;

	GPIO_Init(ENGINE_1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(ENGINE_1_GPIO_PORT, ENGINE_1_GPIO_PIN_SOURCE, GPIO_AF_TIM2);
	GPIO_PinAFConfig(ENGINE_2_GPIO_PORT, ENGINE_2_GPIO_PIN_SOURCE, GPIO_AF_TIM2);
	GPIO_PinAFConfig(ENGINE_3_GPIO_PORT, ENGINE_3_GPIO_PIN_SOURCE, GPIO_AF_TIM2);
	GPIO_PinAFConfig(ENGINE_4_GPIO_PORT, ENGINE_4_GPIO_PIN_SOURCE, GPIO_AF_TIM2);

    uint8_t mhz = 1;
	uint8_t freq = 50;
    uint16_t period = mhz * 1000000 / freq;
	uint16_t PrescalerValue = (uint16_t)(SystemCoreClock / ((uint32_t)mhz * 1000000)) - 1;;

	TIM_TimeBaseInitStructure.TIM_Period = period - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode  = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

void writePWM(void) {
    TIM2->CCR1 = motor[0];
    TIM2->CCR2 = motor[1];
    TIM2->CCR3 = motor[2];
    TIM2->CCR4 = motor[3];
}

