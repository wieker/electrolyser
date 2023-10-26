#include <stdio.h>
#include "stm32f4xx.h"

void USART1Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef  USART_ClockInitStructure;
    //enable bus clocks
    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //Configure USART1 Tx (PA.09) as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Configure USART1 Rx (PA.10) as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_ClockStructInit(&USART_ClockInitStructure);
    USART_ClockInit(USART1, &USART_ClockInitStructure);
    USART_StructInit(&USART_InitStructure);
    //Configure USART1 basic and asynchronous paramters
    USART_Init(USART1, &USART_InitStructure);
    //Enable USART1
    USART_Cmd(USART1, ENABLE);
}

int _write(int handle, char* data, int size) {
	unsigned int i;
	for (i = 0; i < size; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, data[i]);
	}

	return size;
}

