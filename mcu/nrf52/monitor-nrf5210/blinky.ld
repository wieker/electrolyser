MEMORY
{
  FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x30000
  RAM (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x06000
}

INCLUDE "nrf_common.ld"

