//
// Created by wieker on 9/30/20.
//

#ifndef SAMD11_COM_DMA_H
#define SAMD11_COM_DMA_H

#include <stdint.h>
#include "stddef.h"

typedef enum
{
    /* DMAC Channel 0 */
    DMAC_CHANNEL_0 = 0,
} DMAC_CHANNEL;

extern uint8_t app_response_buffer[64];

void DMAC_Initialize( void );

bool DMAC_ChannelTransfer( DMAC_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize );

void dma_complete_cb();

#endif //SAMD11_COM_DMA_H
