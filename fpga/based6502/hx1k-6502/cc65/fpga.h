/*
 * fpga.h - handy info about the FPGA
 * 03-04-19 E. Brombaugh
 */

#ifndef __FPGA__
#define __FPGA__

#define GPIO_DATA (*(unsigned char *) 0x1000)
#define ACIA_CTRL (*(unsigned char *) 0x1002)
#define ACIA_DATA (*(unsigned char *) 0x1003)
#define SRAM_DATA (*(unsigned char *) 0x3000)
#define ADC_DATA (*(unsigned char *) 0x4000)
#define ACIA_SRAM_STORE (*(unsigned char *) 0x3100)

#endif
