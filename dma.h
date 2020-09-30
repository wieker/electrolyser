//
// Created by wieker on 9/30/20.
//

#ifndef SAMD11_COM_DMA_H
#define SAMD11_COM_DMA_H

#include "stddef.h"

typedef enum
{
    /* DMAC Channel 0 */
    DMAC_CHANNEL_0 = 0,
} DMAC_CHANNEL;

void DMAC_Initialize( void );

bool DMAC_ChannelTransfer( DMAC_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize );

#endif //SAMD11_COM_DMA_H
