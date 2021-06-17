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

void dma_init(void );

void dma_start();

void dma_complete_cb();

void dma_descrs();

#endif //SAMD11_COM_DMA_H
