/* This file is part of the libopencm3 project.
 *
 * It was generated by the irq2nvic_h script.
 */

#ifndef LIBOPENCM3_LPC13xx_NVIC_H
#define LIBOPENCM3_LPC13xx_NVIC_H

#include <libopencm3/cm3/nvic.h>

/** @defgroup CM3_nvic_defines_LPC13xx User interrupts for LPC 13xx series
    @ingroup CM3_nvic_defines

    @{*/

#define NVIC_PIO0_0_IRQ 0
#define NVIC_PIO0_1_IRQ 1
#define NVIC_PIO0_2_IRQ 2
#define NVIC_PIO0_3_IRQ 3
#define NVIC_PIO0_4_IRQ 4
#define NVIC_PIO0_5_IRQ 5
#define NVIC_PIO0_6_IRQ 6
#define NVIC_PIO0_7_IRQ 7
#define NVIC_PIO0_8_IRQ 8
#define NVIC_PIO0_9_IRQ 9
#define NVIC_PIO0_10_IRQ 10
#define NVIC_PIO0_11_IRQ 11
#define NVIC_PIO1_0_IRQ 12
#define NVIC_PIO1_1_IRQ 13
#define NVIC_PIO1_2_IRQ 14
#define NVIC_PIO1_3_IRQ 15
#define NVIC_PIO1_4_IRQ 16
#define NVIC_PIO1_5_IRQ 17
#define NVIC_PIO1_6_IRQ 18
#define NVIC_PIO1_7_IRQ 19
#define NVIC_PIO1_8_IRQ 20
#define NVIC_PIO1_9_IRQ 21
#define NVIC_PIO1_10_IRQ 22
#define NVIC_PIO1_11_IRQ 23
#define NVIC_PIO2_0_IRQ 24
#define NVIC_PIO2_1_IRQ 25
#define NVIC_PIO2_2_IRQ 26
#define NVIC_PIO2_3_IRQ 27
#define NVIC_PIO2_4_IRQ 28
#define NVIC_PIO2_5_IRQ 29
#define NVIC_PIO2_6_IRQ 30
#define NVIC_PIO2_7_IRQ 31
#define NVIC_PIO2_8_IRQ 32
#define NVIC_PIO2_9_IRQ 33
#define NVIC_PIO2_10_IRQ 34
#define NVIC_PIO2_11_IRQ 35
#define NVIC_PIO3_0_IRQ 36
#define NVIC_PIO3_1_IRQ 37
#define NVIC_PIO3_2_IRQ 38
#define NVIC_PIO3_3_IRQ 39
#define NVIC_I2C0_IRQ 40
#define NVIC_CT16B0_IRQ 41
#define NVIC_CT16B1_IRQ 42
#define NVIC_CT32B0_IRQ 43
#define NVIC_CT32B1_IRQ 44
#define NVIC_SSP0_IRQ 45
#define NVIC_UART_IRQ 46
#define NVIC_USB_IRQ 47
#define NVIC_USB_FIQ_IRQ 48
#define NVIC_ADC_IRQ 49
#define NVIC_WDT_IRQ 50
#define NVIC_BOD_IRQ 51
#define NVIC_PIO3_IRQ 53
#define NVIC_PIO2_IRQ 54
#define NVIC_PIO1_IRQ 55
#define NVIC_SSP1_IRQ 56

#define NVIC_IRQ_COUNT 57

/**@}*/

/** @defgroup CM3_nvic_isrprototypes_LPC13xx User interrupt service routines (ISR) prototypes for LPC 13xx series
    @ingroup CM3_nvic_isrprototypes

    @{*/

BEGIN_DECLS

void WEAK pio0_0_isr(void);
void WEAK pio0_1_isr(void);
void WEAK pio0_2_isr(void);
void WEAK pio0_3_isr(void);
void WEAK pio0_4_isr(void);
void WEAK pio0_5_isr(void);
void WEAK pio0_6_isr(void);
void WEAK pio0_7_isr(void);
void WEAK pio0_8_isr(void);
void WEAK pio0_9_isr(void);
void WEAK pio0_10_isr(void);
void WEAK pio0_11_isr(void);
void WEAK pio1_0_isr(void);
void WEAK pio1_1_isr(void);
void WEAK pio1_2_isr(void);
void WEAK pio1_3_isr(void);
void WEAK pio1_4_isr(void);
void WEAK pio1_5_isr(void);
void WEAK pio1_6_isr(void);
void WEAK pio1_7_isr(void);
void WEAK pio1_8_isr(void);
void WEAK pio1_9_isr(void);
void WEAK pio1_10_isr(void);
void WEAK pio1_11_isr(void);
void WEAK pio2_0_isr(void);
void WEAK pio2_1_isr(void);
void WEAK pio2_2_isr(void);
void WEAK pio2_3_isr(void);
void WEAK pio2_4_isr(void);
void WEAK pio2_5_isr(void);
void WEAK pio2_6_isr(void);
void WEAK pio2_7_isr(void);
void WEAK pio2_8_isr(void);
void WEAK pio2_9_isr(void);
void WEAK pio2_10_isr(void);
void WEAK pio2_11_isr(void);
void WEAK pio3_0_isr(void);
void WEAK pio3_1_isr(void);
void WEAK pio3_2_isr(void);
void WEAK pio3_3_isr(void);
void WEAK i2c0_isr(void);
void WEAK ct16b0_isr(void);
void WEAK ct16b1_isr(void);
void WEAK ct32b0_isr(void);
void WEAK ct32b1_isr(void);
void WEAK ssp0_isr(void);
void WEAK uart_isr(void);
void WEAK usb_isr(void);
void WEAK usb_fiq_isr(void);
void WEAK adc_isr(void);
void WEAK wdt_isr(void);
void WEAK bod_isr(void);
void WEAK pio3_isr(void);
void WEAK pio2_isr(void);
void WEAK pio1_isr(void);
void WEAK ssp1_isr(void);

END_DECLS

/**@}*/

#endif /* LIBOPENCM3_LPC13xx_NVIC_H */
