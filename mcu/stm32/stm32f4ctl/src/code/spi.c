#include "../ctl/defs.h"

#include "stm32f4xx.h"


void spi_txrx(SPI_TypeDef *spi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    int i = 0;
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

    while(i < Size)
    {
      /* check TXE flag */
        while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE) == RESET) {}
        SPI_I2S_SendData(spi, pTxData[i]);

      /* Wait until RXNE flag is reset */
        while (SPI_I2S_GetFlagStatus(spi, SPI_FLAG_RXNE) == RESET) {}
        pRxData[i] = SPI_I2S_ReceiveData(spi);

        i ++;
    }

    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
}

uint8_t txrx[16];

uint8_t spi_xfer(uint8_t reg, uint8_t dt) {
    txrx[0] = reg;
    txrx[1] = dt;
    spi_txrx(SPI1, txrx, txrx, 2);
    return txrx[1];
}

uint8_t* spi_xfer15(uint8_t reg) {
    txrx[0] = reg;
    spi_txrx(SPI1, txrx, txrx, 15);
    return &txrx[1];
}

void spiInit(void) {
	/* SSP initialization */
    SPI_InitTypeDef spi;
	GPIO_InitTypeDef            GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_Init(SPI1, &spi);
    SPI_Cmd(SPI1, ENABLE);
}
