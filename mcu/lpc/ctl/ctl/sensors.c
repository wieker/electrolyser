#include "board.h"
#include <math.h>
#include "defs.h"

#include "../icm20689/accgyro_icm20689.h"
#include "../icm20689/accgyro_mpu.h"

int16_t gyroData[3] = { 0, 0, 0 };
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

uint8_t spi_xfer(uint8_t reg, uint8_t dt) {
	static Chip_SSP_DATA_SETUP_T xf_setup;
	xf_setup.length = 2;
	xf_setup.tx_data = Tx_Buf;
	Tx_Buf[0] = reg;
	Tx_Buf[1] = dt;
	Tx_Buf[2] = 0x04;
	Tx_Buf[2] = 0x08;
	xf_setup.rx_data = Rx_Buf;
	xf_setup.rx_cnt = xf_setup.tx_cnt = 0;
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) false);
	Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) true);
    return Rx_Buf[1];
}

uint8_t* spi_xfer6(uint8_t reg) {
	static Chip_SSP_DATA_SETUP_T xf_setup;
	xf_setup.length = 7;
	xf_setup.tx_data = Tx_Buf;
	Tx_Buf[0] = reg;
	Tx_Buf[1] = 0x00;
	Tx_Buf[2] = 0x00;
	Tx_Buf[2] = 0x00;
	xf_setup.rx_data = Rx_Buf;
	xf_setup.rx_cnt = xf_setup.tx_cnt = 0;
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) false);
	Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 8, (bool) true);
    return &Rx_Buf[1];
}


void imuInit(void)
{
	/* SSP initialization */
	Board_SSP_Init(LPC_SSP);

	Chip_SSP_Init(LPC_SSP);

	Chip_SSP_SetFormat(LPC_SSP, SSP_DATA_BITS, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_MODE0);

	Chip_SSP_Enable(LPC_SSP);
	Chip_SSP_SetMaster(LPC_SSP, 1);


    spi_xfer(PWR_MGMT_1, 0x80);
    delay(20000000);
    spi_xfer(WHO_AM_I | DIR_READ, 0x80);
    delay(20000000);
    DEBUGOUT("PWM write res=%02x expected=%02x\r\n", Rx_Buf[1], WHOAMI);
    spi_xfer(MPU_RA_SIGNAL_PATH_RESET, 0x03);
    delay(20000000);
    spi_xfer(MPU_RA_PWR_MGMT_1, INV_CLK_PLL);
    delay(20000000);
    spi_xfer(MPU_RA_GYRO_CONFIG, INV_FSR_2000DPS << 3);
    delay(200);
    spi_xfer(MPU_RA_ACCEL_CONFIG, INV_FSR_16G << 3);
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
    //printf("test %02x\r\n", spi_xfer(WHO_AM_I | DIR_READ, 0x00));
    uint8_t* val = spi_xfer6(MPU_RA_GYRO_XOUT_H | DIR_READ);
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    gyroData[0] = (int16_t)((val[0] << 8) | val[1]) / 8;
    gyroData[1] = (int16_t)((val[2] << 8) | val[3]) / 8;
    gyroData[2] = (int16_t)((val[4] << 8) | val[5]) / 8;
    uint32_t ctime = millis();
    //printf("test %d\r\n", ctime);
    val = spi_xfer6(MPU_RA_ACCEL_XOUT_H | DIR_READ);
    accADC[0] = (int16_t)((val[0] << 8) | val[1]) / 4;
    accADC[1] = (int16_t)((val[2] << 8) | val[3]) / 4;
    accADC[2] = (int16_t)((val[4] << 8) | val[5]) / 4;
    //printf("test %02x%02x %02x%02x %02x%02x\r\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    //printf("test %f %f %f\r\n", accADC[0] * 16.0f / 32767.0f, accADC[1] * 16.0f / 32767.0f, accADC[2] * 16.0f / 32767.0f);
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
    for (axis = 0; axis < 3; axis++)
        accADC[axis] -= accZero[axis];
    accADC[1] -= acc_1G;
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

    gyroData[YAW] = gyroADC[YAW];
    gyroData[ROLL] = gyroADC[ROLL];
    gyroData[PITCH] = gyroADC[PITCH];

    getEstimatedAttitude();

}

int init_i2c();
uint8_t *read_data(int slaveAddr, unsigned int addr);

#define QMC5883L_MAG_I2C_ADDRESS     0x0D

// Registers
#define QMC5883L_REG_CONF1 0x09
#define QMC5883L_REG_CONF2 0x0A

// data output rates for 5883L
#define QMC5883L_ODR_10HZ (0x00 << 2)
#define QMC5883L_ODR_50HZ  (0x01 << 2)
#define QMC5883L_ODR_100HZ (0x02 << 2)
#define QMC5883L_ODR_200HZ (0x03 << 2)

// Sensor operation modes
#define QMC5883L_MODE_STANDBY 0x00
#define QMC5883L_MODE_CONTINUOUS 0x01

#define QMC5883L_RNG_2G (0x00 << 4)
#define QMC5883L_RNG_8G (0x01 << 4)

#define QMC5883L_OSR_512 (0x00 << 6)
#define QMC5883L_OSR_256 (0x01 << 6)
#define QMC5883L_OSR_128	(0x10 << 6)
#define QMC5883L_OSR_64	(0x11	<< 6)

#define QMC5883L_RST 0x80

#define QMC5883L_REG_DATA_OUTPUT_X 0x00
#define QMC5883L_REG_STATUS 0x06

#define QMC5883L_REG_ID 0x0D
#define QMC5883_ID_VAL 0xFF

void xxx() {
    uint8_t txd[] = {QMC5883L_REG_DATA_OUTPUT_X};
    uint8_t rxd[6];

    static I2C_XFER_T xfer;
    (xfer).slaveAddr = 0x0d;
    (xfer).rxBuff = rxd;
    (xfer).txBuff = txd;
    (xfer).txSz = 1;
    (xfer).rxSz = 6;
    Chip_I2C_MasterTransfer(I2C0, &xfer);
    // DEBUGOUT("Master transfer : %s\r\n",
    //          (xfer).status == I2C_STATUS_DONE ? "SUCCESS" : "FAILURE");
    // DEBUGOUT("Received %d bytes from slave 0x%02X\r\n", 6 - (xfer).rxSz, (xfer).slaveAddr);

    magADC[0] = (int16_t)(rxd[1] << 8 | rxd[0]);
    magADC[1] = (int16_t)(rxd[3] << 8 | rxd[2]);
    magADC[2] = (int16_t)(rxd[5] << 8 | rxd[4]);

    // printf("gyr %d %d %d\r\n", magADC[YAW], magADC[ROLL], magADC[PITCH]);
}

int magInit = 1;
int magcal = 1;

int Mag_getADC(void)
{
    static uint32_t t, tCal = 0;
    static int16_t magZeroTempMin[3];
    static int16_t magZeroTempMax[3];
    static int16_t magZero[3] = {371 - 180, -967 + 700, -730};
    uint32_t axis;

    if ((int32_t)(currentTime - t) < 0)
        return 0;                 //each read is spaced by 100ms
    t = currentTime + 100000;

    // Read mag sensor
    xxx();

    if (!magcal) {
        tCal = t;
        for (axis = 0; axis < 3; axis++) {
            magZero[axis] = 0;
            magZeroTempMin[axis] = magADC[axis];
            magZeroTempMax[axis] = magADC[axis];
        }
        magcal = 1;
    }

    if (magInit) {              // we apply offset only once mag calibration is done
        magADC[0] -= magZero[0];
        magADC[1] -= magZero[1];
        magADC[2] -= magZero[2];
        //printf("zero %d %d %d\r\n", magZero[0], magZero[1], magZero[2]);
    }

    if (tCal != 0) {
        if ((t - tCal) < 60000000) {    // 30s: you have 30s to turn the multi in all directions
            for (axis = 0; axis < 3; axis++) {
                if (magADC[axis] < magZeroTempMin[axis])
                    magZeroTempMin[axis] = magADC[axis];
                if (magADC[axis] > magZeroTempMax[axis])
                    magZeroTempMax[axis] = magADC[axis];
            }
        } else {
            tCal = 0;
            for (axis = 0; axis < 3; axis++)
                magZero[axis] = (magZeroTempMin[axis] + magZeroTempMax[axis]) / 2; // Calculate offsets
            magInit = 1;
        }
    }
    float hd = (atan2f(magADC[0], magADC[1]) * 1800.0f / M_PI) / 10.0f;
    head = lrintf(hd);
    //printf("head: %d\r\n", head);

    return 1;
}
