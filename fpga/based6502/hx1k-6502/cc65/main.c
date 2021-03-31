/*
 * main.c - top level 6502 C code for icestick_6502
 * 03-04-19 E. Brombaugh
 * based on example code from https://cc65.github.io/doc/customizing.html
 */
 
#include <stdio.h>
#include <string.h>
#include "fpga.h"
#include "acia.h"

unsigned long cnt;
unsigned char x = 0;
char txt_buf[32];
unsigned long i;

unsigned char *cmd = &SRAM_DATA;
unsigned char *buf = (&SRAM_DATA) + 50;
char st = 0;

int main()
{
    int s = 0;
    int i;

	// Send startup message
	acia_tx_str("\n\n\rIcestick 6502 cc65 serial test\n\n\r");
	
	// enable ACIA IRQ for serial echo in background
	ACIA_CTRL = 0x80;
	asm("CLI");

    // Run forever with GPIO blink
    while(1) {
        for (i = 0; i < 1000; i ++) {

        }
      for (i = 0; i < 1000; i ++) {

      }
      for (i = 0; i < 1000; i ++) {

      }
      for (i = 0; i < 1000; i ++) {

      }

        if (s == 0) {
            acia_tx_str("hello\no");
            GPIO_DATA = st = 0xAA;
            s = 1;
        } else {
            acia_tx_str("world\nd");
            GPIO_DATA = st = 0x55;
            s = 0;
        }
    }

    //  We should never get here!
    return (0);
}

void execute() {
  int len = 0;
  int i;
  switch (*cmd) {
    case 's':
      while (*(cmd + len) != 0) {
        len ++;
      }
      buf[len - 1] = 0;
      for (i = 0; i < len - 1; i ++) {
        buf[i] = cmd[len - i - 1];
      }
      break;
    case 'l':
      acia_tx_str(buf);
  }
}

void run_cmd() {
  GPIO_DATA = st = 0xff - st;
    if (ACIA_SRAM_STORE != 0) {
      (*cmd) = ACIA_SRAM_STORE;
      cmd++;
    } else {
      (*cmd) = ACIA_SRAM_STORE;
      cmd = &SRAM_DATA;
      execute();
    }
  GPIO_DATA = st = 0xff - st;
  //acia_tx_str(&SRAM_DATA);
    //acia_tx_str("\n\n\rIcestick 6502 cc65 serial test\n\n\r");
}
