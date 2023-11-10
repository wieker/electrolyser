#include "defs.h"
#include <stdio.h>

#include "../../../../lpc/ctl/icm20689/accgyro_icm20689.h"
#include "../../../../lpc/ctl/icm20689/accgyro_mpu.h"

static const uint8_t WHOAMI = 0x98;
static const uint8_t WHO_AM_I = 0x75;
static const uint8_t PWR_MGMT_1 = 0x6B;
static const uint8_t DIR_READ = 0x80;


void icm20689_init(void)
{
    spiInit();

    spi_xfer(PWR_MGMT_1, 0x80);
    printf("point3\r\n");
    delay(20000000);
    uint8_t rxv = spi_xfer(WHO_AM_I | DIR_READ, 0xAB);
    delay(20000000);
    printf("PWM write res=%02x expected=%02x\r\n", rxv, WHOAMI);
    spi_xfer(MPU_RA_SIGNAL_PATH_RESET, 0x03);
    delay(20000000);
    spi_xfer(MPU_RA_PWR_MGMT_1, INV_CLK_PLL);
    delay(20000000);
    spi_xfer(0x6A, 0x10);
    delay(200);
    spi_xfer(MPU_RA_GYRO_CONFIG, INV_FSR_2000DPS << 3);
    delay(200);
    spi_xfer(MPU_RA_ACCEL_CONFIG, INV_FSR_16G << 3);
    delay(200);
    spi_xfer(0x1D, 0x05);
    delay(200);
    spi_xfer(MPU_RA_CONFIG, MPU_DLPF_10HZ);
    delay(200);
    spi_xfer(MPU_RA_SMPLRT_DIV, 0);
    delay(200);
}

void icm20689_read(int16_t *gyroData)
{
    //delay(20000000);
    static uint32_t ptime = 0;
    int16_t tmp[3];
    //printf("test %02x\r\n", spi_xfer(WHO_AM_I | DIR_READ, 0x00));
    uint8_t* val = spi_xfer15(MPU_RA_ACCEL_XOUT_H | DIR_READ);
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[8], val[9], val[10], val[11], val[12], val[13]);
    tmp[0] = (int16_t)((val[8] << 8) | val[9]) / 4;
    tmp[1] = (int16_t)((val[10] << 8) | val[11]) / 4;
    tmp[2] = (int16_t)((val[12] << 8) | val[13]) / 4;
    gyroData[0] = tmp[0];
    gyroData[1] = tmp[2];
    gyroData[2] = - tmp[1];
    tmp[0] = (int16_t)((val[0] << 8) | val[1]) / 4;
    tmp[1] = (int16_t)((val[2] << 8) | val[3]) / 4;
    tmp[2] = (int16_t)((val[4] << 8) | val[5]) / 4;
    accADC[0] = tmp[0];
    accADC[1] = tmp[2];
    accADC[2] = - tmp[1];
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[0], val[1], val[2], val[3], val[4], val[5]);
}
