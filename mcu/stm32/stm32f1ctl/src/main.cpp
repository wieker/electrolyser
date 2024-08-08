#include "stm32f10x.h"

extern "C" {
#include "ctl/defs.h"
#include <stdio.h>
}

extern "C" int main()
{
  // Switch on blue LED on STM32F407Discovery
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
  
  while (true) {
	  for (long i = 0; i < 1000000; i ++) {
	  }

  	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	  for (long i = 0; i < 1000000; i ++) {
	  }

  	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
  }

  USART1Init();
  main_loop();


  return 0;
}
