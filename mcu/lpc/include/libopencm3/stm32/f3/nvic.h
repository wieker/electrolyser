/* This file is part of the libopencm3 project.
 *
 * It was generated by the irq2nvic_h script.
 */

#ifndef LIBOPENCM3_STM32_F3_NVIC_H
#define LIBOPENCM3_STM32_F3_NVIC_H

#include <libopencm3/cm3/nvic.h>

/** @defgroup CM3_nvic_defines_STM32F3 User interrupts for STM32 F3 series
    @ingroup CM3_nvic_defines

    @{*/

#define NVIC_NVIC_WWDG_IRQ 0
#define NVIC_PVD_IRQ 1
#define NVIC_TAMP_STAMP_IRQ 2
#define NVIC_RTC_WKUP_IRQ 3
#define NVIC_FLASH_IRQ 4
#define NVIC_RCC_IRQ 5
#define NVIC_EXTI0_IRQ 6
#define NVIC_EXTI1_IRQ 7
#define NVIC_EXTI2_TSC_IRQ 8
#define NVIC_EXTI3_IRQ 9
#define NVIC_EXTI4_IRQ 10
#define NVIC_DMA1_CH1_IRQ 11
#define NVIC_DMA1_CH2_IRQ 12
#define NVIC_DMA1_CH3_IRQ 13
#define NVIC_DMA1_CH4_IRQ 14
#define NVIC_DMA1_CH5_IRQ 15
#define NVIC_DMA1_CH6_IRQ 16
#define NVIC_DMA1_CH7_IRQ 17
#define NVIC_ADC1_2_IRQ 18
#define NVIC_USB_HP_CAN1_TX_IRQ 19
#define NVIC_USB_LP_CAN1_RX0_IRQ 20
#define NVIC_CAN1_RX1_IRQ 21
#define NVIC_CAN1_SCE_IRQ 22
#define NVIC_EXTI9_5_IRQ 23
#define NVIC_TIM1_BRK_TIM15_IRQ 24
#define NVIC_TIM1_UP_TIM16_IRQ 25
#define NVIC_TIM1_TRG_COM_TIM17_IRQ 26
#define NVIC_TIM1_CC_IRQ 27
#define NVIC_TIM2_IRQ 28
#define NVIC_TIM3_IRQ 29
#define NVIC_TIM4_IRQ 30
#define NVIC_I2C1_EV_EXTI23_IRQ 31
#define NVIC_I2C1_ER_IRQ 32
#define NVIC_I2C2_EV_EXTI24_IRQ 33
#define NVIC_I2C2_ER_IRQ 34
#define NVIC_SPI1_IRQ 35
#define NVIC_SPI2_IRQ 36
#define NVIC_USART1_EXTI25_IRQ 37
#define NVIC_USART2_EXTI26_IRQ 38
#define NVIC_USART3_EXTI28_IRQ 39
#define NVIC_EXTI15_10_IRQ 40
#define NVIC_RTC_ALARM_IRQ 41
#define NVIC_USB_WKUP_A_IRQ 42
#define NVIC_TIM8_BRK_IRQ 43
#define NVIC_TIM8_UP_IRQ 44
#define NVIC_TIM8_TRG_COM_IRQ 45
#define NVIC_TIM8_CC_IRQ 46
#define NVIC_ADC3_IRQ 47
#define NVIC_RESERVED_1_IRQ 48
#define NVIC_RESERVED_2_IRQ 49
#define NVIC_RESERVED_3_IRQ 50
#define NVIC_SPI3_IRQ 51
#define NVIC_UART4_EXTI34_IRQ 52
#define NVIC_UART5_EXTI35_IRQ 53
#define NVIC_TIM6_DAC_IRQ 54
#define NVIC_TIM7_IRQ 55
#define NVIC_DMA2_CH1_IRQ 56
#define NVIC_DMA2_CH2_IRQ 57
#define NVIC_DMA2_CH3_IRQ 58
#define NVIC_DMA2_CH4_IRQ 59
#define NVIC_DMA2_CH5_IRQ 60
#define NVIC_ETH_IRQ 61
#define NVIC_RESERVED_4_IRQ 62
#define NVIC_RESERVED_5_IRQ 63
#define NVIC_COMP123_IRQ 64
#define NVIC_COMP456_IRQ 65
#define NVIC_COMP7_IRQ 66
#define NVIC_RESERVED_6_IRQ 67
#define NVIC_RESERVED_7_IRQ 68
#define NVIC_RESERVED_8_IRQ 69
#define NVIC_RESERVED_9_IRQ 70
#define NVIC_RESERVED_10_IRQ 71
#define NVIC_RESERVED_11_IRQ 72
#define NVIC_RESERVED_12_IRQ 73
#define NVIC_USB_HP_IRQ 74
#define NVIC_USB_LP_IRQ 75
#define NVIC_USB_WKUP_IRQ 76
#define NVIC_RESERVED_13_IRQ 77
#define NVIC_RESERVED_14_IRQ 78
#define NVIC_RESERVED_15_IRQ 79
#define NVIC_RESERVED_16_IRQ 80

#define NVIC_IRQ_COUNT 81

/**@}*/

/** @defgroup CM3_nvic_isrprototypes_STM32F3 User interrupt service routines (ISR) prototypes for STM32 F3 series
    @ingroup CM3_nvic_isrprototypes

    @{*/

BEGIN_DECLS

void WEAK nvic_wwdg_isr(void);
void WEAK pvd_isr(void);
void WEAK tamp_stamp_isr(void);
void WEAK rtc_wkup_isr(void);
void WEAK flash_isr(void);
void WEAK rcc_isr(void);
void WEAK exti0_isr(void);
void WEAK exti1_isr(void);
void WEAK exti2_tsc_isr(void);
void WEAK exti3_isr(void);
void WEAK exti4_isr(void);
void WEAK dma1_ch1_isr(void);
void WEAK dma1_ch2_isr(void);
void WEAK dma1_ch3_isr(void);
void WEAK dma1_ch4_isr(void);
void WEAK dma1_ch5_isr(void);
void WEAK dma1_ch6_isr(void);
void WEAK dma1_ch7_isr(void);
void WEAK adc1_2_isr(void);
void WEAK usb_hp_can1_tx_isr(void);
void WEAK usb_lp_can1_rx0_isr(void);
void WEAK can1_rx1_isr(void);
void WEAK can1_sce_isr(void);
void WEAK exti9_5_isr(void);
void WEAK tim1_brk_tim15_isr(void);
void WEAK tim1_up_tim16_isr(void);
void WEAK tim1_trg_com_tim17_isr(void);
void WEAK tim1_cc_isr(void);
void WEAK tim2_isr(void);
void WEAK tim3_isr(void);
void WEAK tim4_isr(void);
void WEAK i2c1_ev_exti23_isr(void);
void WEAK i2c1_er_isr(void);
void WEAK i2c2_ev_exti24_isr(void);
void WEAK i2c2_er_isr(void);
void WEAK spi1_isr(void);
void WEAK spi2_isr(void);
void WEAK usart1_exti25_isr(void);
void WEAK usart2_exti26_isr(void);
void WEAK usart3_exti28_isr(void);
void WEAK exti15_10_isr(void);
void WEAK rtc_alarm_isr(void);
void WEAK usb_wkup_a_isr(void);
void WEAK tim8_brk_isr(void);
void WEAK tim8_up_isr(void);
void WEAK tim8_trg_com_isr(void);
void WEAK tim8_cc_isr(void);
void WEAK adc3_isr(void);
void WEAK reserved_1_isr(void);
void WEAK reserved_2_isr(void);
void WEAK reserved_3_isr(void);
void WEAK spi3_isr(void);
void WEAK uart4_exti34_isr(void);
void WEAK uart5_exti35_isr(void);
void WEAK tim6_dac_isr(void);
void WEAK tim7_isr(void);
void WEAK dma2_ch1_isr(void);
void WEAK dma2_ch2_isr(void);
void WEAK dma2_ch3_isr(void);
void WEAK dma2_ch4_isr(void);
void WEAK dma2_ch5_isr(void);
void WEAK eth_isr(void);
void WEAK reserved_4_isr(void);
void WEAK reserved_5_isr(void);
void WEAK comp123_isr(void);
void WEAK comp456_isr(void);
void WEAK comp7_isr(void);
void WEAK reserved_6_isr(void);
void WEAK reserved_7_isr(void);
void WEAK reserved_8_isr(void);
void WEAK reserved_9_isr(void);
void WEAK reserved_10_isr(void);
void WEAK reserved_11_isr(void);
void WEAK reserved_12_isr(void);
void WEAK usb_hp_isr(void);
void WEAK usb_lp_isr(void);
void WEAK usb_wkup_isr(void);
void WEAK reserved_13_isr(void);
void WEAK reserved_14_isr(void);
void WEAK reserved_15_isr(void);
void WEAK reserved_16_isr(void);

END_DECLS

/**@}*/

#endif /* LIBOPENCM3_STM32_F3_NVIC_H */