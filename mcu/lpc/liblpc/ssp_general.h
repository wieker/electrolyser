//
// Created by wieker on 9/28/19.
//

#ifndef HACKRF_FIRMWARE_ALL_SSP_GENERAL_H
#define HACKRF_FIRMWARE_ALL_SSP_GENERAL_H

uint8_t *spi_flash_we();

uint8_t *spi_flash_write(uint32_t addr, uint32_t len, uint8_t* data);

uint8_t *spi_flash_read(uint32_t addr, uint32_t len);

uint8_t *spi_flash_erase(uint32_t addr);

uint8_t *spi_flash_wait();

uint8_t *spi_flash_erase_all();

#endif //HACKRF_FIRMWARE_ALL_SSP_GENERAL_H
