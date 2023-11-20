#include <nrf_delay.h>
#include "ble_config/sdk_config.h"
#include <legacy/nrf_drv_uart.h>
#include <nrf_systick.h>

extern nrf_drv_uart_t m_uart;
uint8_t rxx[10];

void uart_init();

#define TASK_NUMBER_MAX   (2)
uint32_t __uCurrentTaskIdx = 0;
uint32_t __puTasksPSP[TASK_NUMBER_MAX] = {0};

uint32_t get_current_psp() {
  return __puTasksPSP[__uCurrentTaskIdx];
}

// save PSP value to the slot at __uCurrentTaskIdx index
void save_current_psp(uint32_t psp) {
  __puTasksPSP[__uCurrentTaskIdx] = psp;
}

uint8_t b1[2048];

#define TASK_STACK_SIZE   (1024u)

void init_task(void (*handler)) {
  int i=0;

  // find an empty slot
  for(; i<TASK_NUMBER_MAX; i++) {
    if (__puTasksPSP[i] == 0) break;
  }

  if(i >= TASK_NUMBER_MAX) {
    return;
  } else {
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "Register\r\n", 10);
  }

  // calculate new PSP
  uint32_t* psp = (uint32_t*)&b1[(i + 1) * TASK_STACK_SIZE];

  // fill stack frame
    // fill dummy stack frame
    *(--psp) = 0x01000000u; // Dummy xPSR, just enable Thumb State bit;
    *(--psp) = (uint32_t) handler; // PC
    *(--psp) = 0xFFFFFFFDu; // LR with EXC_RETURN to return to Thread using PSP
    *(--psp) = 0x12121212u; // Dummy R12
    *(--psp) = 0x03030303u; // Dummy R3
    *(--psp) = 0x02020202u; // Dummy R2
    *(--psp) = 0x01010101u; // Dummy R1
    *(--psp) = 0x00000000u; // Dummy R0
    *(--psp) = 0x11111111u; // Dummy R11
    *(--psp) = 0x10101010u; // Dummy R10
    *(--psp) = 0x09090909u; // Dummy R9
    *(--psp) = 0x08080808u; // Dummy R8
    *(--psp) = 0x07070707u; // Dummy R7
    *(--psp) = 0x06060606u; // Dummy R6
    *(--psp) = 0x05050505u; // Dummy R5
    *(--psp) = 0x04040404u; // Dummy R4

    // save PSP
    __puTasksPSP[i] = (uint32_t)psp;
}

void start_scheduler() {
  // start with the first task
  __uCurrentTaskIdx = 0;

  // prepare PSP of the first task
  __asm volatile("BL get_current_psp"); // return PSP in R0
  __asm volatile("MSR PSP, R0");  // set PSP

  // change to use PSP
  __asm volatile("MRS R0, CONTROL");
  __asm volatile("ORR R0, R0, #2"); // set bit[1] SPSEL
  __asm volatile("MSR CONTROL, R0");


  nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "hndl01\r\n", 8);
  nrf_systick_load_set(64000ul);
  nrf_systick_csr_set(
      NRF_SYSTICK_CSR_CLKSOURCE_CPU |
      NRF_SYSTICK_CSR_TICKINT_ENABLE |
      NRF_SYSTICK_CSR_ENABLE);

    // get the handler of the first task by tracing back from PSP which is at R4 slot
  void (*handler)() = (void (*))((uint32_t*)__puTasksPSP[__uCurrentTaskIdx])[14];

  // execute the handler
  handler();
}

void select_next_task() {
    /* Round-Robin scheduler */
    __uCurrentTaskIdx++;
    // check if a task is register at current slot
    if (__uCurrentTaskIdx >= TASK_NUMBER_MAX || __puTasksPSP[__uCurrentTaskIdx] == 0) {
        __uCurrentTaskIdx=0;
    }
}

__attribute__ ((naked)) void SysTick_Handler() {
  // save LR back to main, must do this firstly
  __asm volatile("PUSH {LR}");

  //nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "hndl00\r\n", 8);
  /* Save the context of current task */

  // get current PSP
  __asm volatile("MRS R0, PSP");
  // save R4 to R11 to PSP Frame Stack
  __asm volatile("STMDB R0!, {R4-R11}"); // R0 is updated after decrement
  // save current value of PSP
  __asm volatile("BL save_current_psp"); // R0 is first argument

  /* Do scheduling */

  // select next task
  __asm volatile("BL select_next_task");

  /* Retrieve the context of next task */

  // get its past PSP value
  __asm volatile("BL get_current_psp"); // return PSP is in R0
  // retrieve R4-R11 from PSP Fram Stack
  __asm volatile("LDMIA R0!, {R4-R11}"); // R0 is updated after increment
  // update PSP
  __asm volatile("MSR PSP, R0");

  // exit
  __asm volatile("POP {LR}");
  __asm volatile("BX LR");
}

void delay(uint32_t time) {
  for (uint32_t i = 0; i < time; i++)
    for (uint32_t j = 0; j < 255; j++)
      __asm volatile("NOP");
}

void task1(void) {
  while (true) {
    NRF_P0->OUTSET = 1 << 17;
    NRF_P0->OUTCLR = 1 << 18;
    delay(10000);

    NRF_P0->OUTSET = 1 << 18;
    NRF_P0->OUTCLR = 1 << 17;
    delay(10000);
  }
}

void task2(void) {
  while (true) {
    NRF_P0->OUTSET = 1 << 20;
    NRF_P0->OUTCLR = 1 << 19;
    delay(1000);

    NRF_P0->OUTSET = 1 << 19;
    NRF_P0->OUTCLR = 1 << 20;
    delay(1000);
  }
}

int main() {
  NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
  uart_init();
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "zzzz01\r\n", 8);
  init_task(task1);
  init_task(task2);
  // start
  start_scheduler();

  while (1) {
  }
}

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void uart_init()
{
  nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
  config.pseltxd  = 6;
  config.pselrxd  = 8;
  config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
  config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
  config.baudrate = (nrf_uart_baudrate_t)NRFX_UART_DEFAULT_CONFIG_BAUDRATE;
  nrfx_uart_init(&m_uart.uart,
                 (nrfx_uart_config_t const *)&config,
                 NULL);
  nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "init00\r\n", 8);
}

