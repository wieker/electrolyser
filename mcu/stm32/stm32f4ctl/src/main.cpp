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
    .GPIO_Pin = GPIO_Pin_15,
    .GPIO_Mode = GPIO_Mode_OUT,
    .GPIO_Speed = GPIO_High_Speed,
    .GPIO_OType = GPIO_OType_PP,
    .GPIO_PuPd = GPIO_PuPd_NOPULL
  };

  GPIO_Init(GPIOC, &gpio_out);

  GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_RESET);

  USART1Init();
  //main2();
  imuInit();
  configure_pwm();

  TIM2->CCR1 = 10000;

  char ch = EOF;
   while (1)
  {
    int count = millis();
    TIM2->CCR1 = 5000;
    while ((millis() - count) < 1000) ;
    printf("hel\r\n");
    char n = _read();
    if (EOF != n) {
        ch = n;
    }
    if (EOF != ch) {
      printf("PWM =%02x\r\n", ch);
    }
    computeIMU();
    TIM2->CCR1 = 15000;
    count = millis();
    while ((millis() - count) < 1000) ;
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }

  return 0;
}
