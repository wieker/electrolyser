#include "../ctl/defs.h"

#include "stm32f4xx.h"

static char buf[32];
static int b = 0, e = 0;

void USART1Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef  USART_ClockInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //enable bus clocks
    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //Configure USART1 Tx (PA.09) as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Configure USART1 Rx (PA.10) as input floating
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    //GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //USART_ClockStructInit(&USART_ClockInitStructure);
    //USART_ClockInit(USART1, &USART_ClockInitStructure);
    USART_StructInit(&USART_InitStructure);
    //Configure USART1 basic and asynchronous paramters
    USART_InitStructure.USART_BaudRate = 115200;
    USART_Init(USART1, &USART_InitStructure);

    // Configure Rx buffer not empty interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    //Enable USART1
    USART_Cmd(USART1, ENABLE);
}

int _write(int handle, char* data, int size) {
	unsigned int i;
	for (i = 0; i < size; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, data[i]);
	}
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return size;
}

int _read(int handle, char* data, int size) {
	if (e != b) {
        data[0] = buf[b];
        b = (b + 1) % 32;
		return 1;
	}
	return -1;
}

void __attribute__((used)) USART1_IRQHandler(void)
{
    buf[e] = (int) USART_ReceiveData(USART1);
    e = (e + 1) % 32;
}

