#include <math.h>
#include <stdint.h>
#include "defs.h"

#include "stm32f4xx.h"

#include "../../../../lpc/ctl/icm20689/accgyro_icm20689.h"
#include "../../../../lpc/ctl/icm20689/accgyro_mpu.h"

int16_t gyroADC[3];
uint16_t calibratingG = 0;
int16_t gyroZero[3] = { 0, 0, 0 };
int16_t accADC[3];
int16_t accZero[3] = { 0, 0, 0 };
int16_t magADC[3];
int16_t accSmooth[3];
int head;

#define LPC_SSP           LPC_SSP1
#define SSP_DATA_BITS                       (SSP_BITS_8)

uint8_t Rx_Buf[256];
uint8_t Tx_Buf[256];
static const uint8_t WHOAMI = 0x98;
static const uint8_t WHO_AM_I = 0x75;
static const uint8_t PWR_MGMT_1 = 0x6B;
static const uint8_t DIR_READ = 0x80;

int Mag_getADC(void);

void spi_txrx(SPI_TypeDef *spi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    int TxXferCount = Size, RxXferCount = Size, txallowed = 1;
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

    while((TxXferCount > 0U) || (RxXferCount > 0U))
    {
      /* check TXE flag */
      if(txallowed && (TxXferCount > 0U) && SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE))
      {
        *(__IO uint8_t *)&spi->DR = (*pTxData++);
        TxXferCount--;
        /* Next Data is a reception (Rx). Tx not allowed */
        txallowed = 0U;
      }

      /* Wait until RXNE flag is reset */
      if((RxXferCount > 0U) && (SPI_I2S_GetFlagStatus(spi, SPI_FLAG_RXNE)))
      {
        (*(uint8_t *)pRxData++) = spi->DR;
        RxXferCount--;
        /* Next Data is a Transmission (Tx). Tx is allowed */
        txallowed = 1U;
      }
    }

    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}

uint8_t txrx[16];

uint8_t spi_xfer(uint8_t reg, uint8_t dt) {
    txrx[0] = reg;
    txrx[1] = dt;
    spi_txrx(SPI2, txrx, txrx, 2);
    return txrx[1];
}

uint8_t* spi_xfer15(uint8_t reg) {
    txrx[0] = reg;
    spi_txrx(SPI2, txrx, txrx, 15);
    return &txrx[1];
}


void imuInit(void)
{
	/* SSP initialization */
    SPI_InitTypeDef spi;
	GPIO_InitTypeDef            GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_Pin_0, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOA, GPIO_Pin_1, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOA, GPIO_Pin_2, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOA, GPIO_Pin_3, GPIO_AF_SPI2);

    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_Init(SPI2, &spi);
    SPI_Cmd(SPI2, ENABLE);


    spi_xfer(PWR_MGMT_1, 0x80);
    delay(20000000);
    spi_xfer(WHO_AM_I | DIR_READ, 0x80);
    delay(20000000);
    printf("PWM write res=%02x expected=%02x\r\n", Rx_Buf[1], WHOAMI);
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

static void mpuGyroRead(int16_t *gyroData)
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
    uint32_t ctime = millis();
    //printf("test %d\r\n", ctime);
    tmp[0] = (int16_t)((val[0] << 8) | val[1]) / 4;
    tmp[1] = (int16_t)((val[2] << 8) | val[3]) / 4;
    tmp[2] = (int16_t)((val[4] << 8) | val[5]) / 4;
    accADC[0] = tmp[0];
    accADC[1] = tmp[2];
    accADC[2] = - tmp[1];
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    //printf("test %f %f %f\r\n", accADC[0] * 4.0f / 2048.0f, accADC[1] * 4.0f / 2048.0f, accADC[2] * 4.0f / 2048.0f);
}

typedef struct stdev_t {
    float m_oldM, m_newM, m_oldS, m_newS;
    int m_n;
} stdev_t;

static void devClear(stdev_t *dev)
{
    dev->m_n = 0;
}

static void devPush(stdev_t *dev, float x)
{
    dev->m_n++;
    if (dev->m_n == 1) {
        dev->m_oldM = dev->m_newM = x;
        dev->m_oldS = 0.0f;
    } else {
        dev->m_newM = dev->m_oldM + (x - dev->m_oldM) / dev->m_n;
        dev->m_newS = dev->m_oldS + (x - dev->m_oldM) * (x - dev->m_newM);
        dev->m_oldM = dev->m_newM;
        dev->m_oldS = dev->m_newS;
    }
}

static float devVariance(stdev_t *dev)
{
    return ((dev->m_n > 1) ? dev->m_newS / (dev->m_n - 1) : 0.0f);
}

static float devStandardDeviation(stdev_t *dev)
{
    return sqrtf(devVariance(dev));
}

static void GYRO_Common(void)
{
    int axis;
    static int32_t g[3];
    static int32_t v[3];
    static stdev_t var[3];

    if (calibratingG > 0) {
        for (axis = 0; axis < 3; axis++) {
            // Reset g[axis] at start of calibration
            if (calibratingG == CALIBRATING_GYRO_CYCLES) {
                g[axis] = 0;
                v[axis] = 0;
                devClear(&var[axis]);
            }
            // Sum up 1000 readings
            g[axis] += gyroADC[axis];
            v[axis] += accADC[axis];
            devPush(&var[axis], gyroADC[axis]);
            // Clear global variables for next reading
            gyroADC[axis] = 0;
            gyroZero[axis] = 0;
            accADC[axis] = 0;
            accZero[axis] = 0;
            if (calibratingG == 1) {
                float dev = devStandardDeviation(&var[axis]);
                gyroZero[axis] = (g[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
                accZero[axis] = (v[axis] + (CALIBRATING_GYRO_CYCLES / 2)) / CALIBRATING_GYRO_CYCLES;
            }
        }
        calibratingG--;
    }
    for (axis = 0; axis < 3; axis++)
        gyroADC[axis] -= gyroZero[axis];
    accADC[2] -= 6;
    accADC[1] += 7;
    accADC[0] -= 9;
}

void Gyro_getADC(void)
{
    // range: +/- 8192; +/- 2000 deg/sec
    mpuGyroRead(gyroADC);
    GYRO_Common();
}

void computeIMU(void)
{
    Gyro_getADC();

    getEstimatedAttitude();

}
