#include "stm32f4xx.h"

extern "C" {
#include "ctl/defs.h"
#include <stdio.h>
}

extern "C" int main()
{
  // Switch on blue LED on STM32F407Discovery
  sysInit();

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef gpio_out = {
    .GPIO_Pin = GPIO_Pin_13,
    .GPIO_Mode = GPIO_Mode_OUT,
    .GPIO_Speed = GPIO_High_Speed,
    .GPIO_OType = GPIO_OType_PP,
    .GPIO_PuPd = GPIO_PuPd_NOPULL
  };

  GPIO_Init(GPIOC, &gpio_out);

  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);

  USART1Init();
  main_loop();


  return 0;
}
