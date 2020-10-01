//
// Created by wieker on 9/30/20.
//

#include <stddef.h>
#include <stdbool.h>
#include "dma.h"
#include "samd11.h"

typedef struct
{  /* Direct Memory Access Controller */
    __IO  uint16_t                       DMAC_BTCTRL;        /**< Offset: 0x00 (R/W  16) Block Transfer Control */
    __IO  uint16_t                       DMAC_BTCNT;         /**< Offset: 0x02 (R/W  16) Block Transfer Count */
    __IO  uint32_t                       DMAC_SRCADDR;       /**< Offset: 0x04 (R/W  32) Block Transfer Source Address */
    __IO  uint32_t                       DMAC_DSTADDR;       /**< Offset: 0x08 (R/W  32) Block Transfer Destination Address */
    __IO  uint32_t                       DMAC_DESCADDR;      /**< Offset: 0x0C (R/W  32) Next Descriptor Address */
} dmac_descriptor_registers_t
#ifdef __GNUC__
        __attribute__ ((aligned (8)))
#endif
;

typedef enum
{
    /* Data was transferred successfully. */
    DMAC_TRANSFER_EVENT_COMPLETE,

    /* Error while processing the request */
    DMAC_TRANSFER_EVENT_ERROR

} DMAC_TRANSFER_EVENT;

typedef uint32_t DMAC_CHANNEL_CONFIG;

typedef void (*DMAC_CHANNEL_CALLBACK) (DMAC_TRANSFER_EVENT event, uintptr_t contextHandle);

#define DMAC_CHANNELS_NUMBER        1

/* DMAC channels object configuration structure */
typedef struct
{
    uint8_t                inUse;

    DMAC_CHANNEL_CALLBACK  callback;

    uintptr_t              context;

    uint8_t                busyStatus;

} DMAC_CH_OBJECT ;

/* Initial write back memory section for DMAC */
static  dmac_descriptor_registers_t _write_back_section[DMAC_CHANNELS_NUMBER]    __attribute__ ((aligned (16)));

/* Descriptor section for DMAC */
static  dmac_descriptor_registers_t  descriptor_section[DMAC_CHANNELS_NUMBER]    __attribute__ ((aligned (16)));

/* DMAC Channels object information structure */
DMAC_CH_OBJECT dmacChannelObj[DMAC_CHANNELS_NUMBER];

void DMAC_Initialize( void )
{
    DMAC_CH_OBJECT *dmacChObj = (DMAC_CH_OBJECT *)&dmacChannelObj[0];
    uint16_t channel = 0;

    /* Initialize DMAC Channel objects */
    for(channel = 0; channel < DMAC_CHANNELS_NUMBER; channel++)
    {
        dmacChObj->inUse = 0;
        dmacChObj->callback = NULL;
        dmacChObj->context = 0;
        dmacChObj->busyStatus = false;

        /* Point to next channel object */
        dmacChObj += 1;
    }

    /* Update the Base address and Write Back address register */
    DMAC->BASEADDR.reg = (uint32_t) descriptor_section;
    DMAC->WRBADDR.reg  = (uint32_t)_write_back_section;

    /* Update the Priority Control register */
    DMAC->PRICTRL0.reg = DMAC_PRICTRL0_LVLPRI0(1) | DMAC_PRICTRL0_RRLVLEN0 | DMAC_PRICTRL0_LVLPRI1(1) | DMAC_PRICTRL0_RRLVLEN1 | DMAC_PRICTRL0_LVLPRI2(1) | DMAC_PRICTRL0_RRLVLEN2 | DMAC_PRICTRL0_LVLPRI3(1) | DMAC_PRICTRL0_RRLVLEN3;

    /***************** Configure DMA channel 0 ********************/

    DMAC->CHID.reg = 0;

    DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGACT(2) | DMAC_CHCTRLB_TRIGSRC(18) | DMAC_CHCTRLB_LVL(0) ;

    descriptor_section[0].DMAC_BTCTRL = DMAC_BTCTRL_BLOCKACT_INT | DMAC_BTCTRL_BEATSIZE_HWORD | DMAC_BTCTRL_VALID | DMAC_BTCTRL_DSTINC ;

    dmacChannelObj[0].inUse = 1;

    DMAC->CHINTENSET.reg = (DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL);

    /* Enable the DMAC module & Priority Level x Enable */
    DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN0 | DMAC_CTRL_LVLEN1 | DMAC_CTRL_LVLEN2 | DMAC_CTRL_LVLEN3;


    NVIC_SetPriority(DMAC_IRQn, 3);
    NVIC_EnableIRQ(DMAC_IRQn);

}

/*******************************************************************************
    This function schedules a DMA transfer on the specified DMA channel.
********************************************************************************/

bool DMAC_ChannelTransfer( DMAC_CHANNEL channel, const void *srcAddr, const void *destAddr, size_t blockSize )
{
    uint8_t beat_size = 0;
    uint8_t channelId = 0;
    bool returnStatus = false;

    if (dmacChannelObj[channel].busyStatus == false)
    {
        /* Get a pointer to the module hardware instance */
        dmac_descriptor_registers_t *const dmacDescReg = &descriptor_section[channel];

        dmacChannelObj[channel].busyStatus = true;

        /* Set source address */
        if (dmacDescReg->DMAC_BTCTRL & DMAC_BTCTRL_SRCINC)
        {
            dmacDescReg->DMAC_SRCADDR = (uint32_t) ((intptr_t)srcAddr + blockSize);
        }
        else
        {
            dmacDescReg->DMAC_SRCADDR = (uint32_t) (srcAddr);
        }

        /* Set destination address */
        if (dmacDescReg->DMAC_BTCTRL & DMAC_BTCTRL_DSTINC)
        {
            dmacDescReg->DMAC_DSTADDR = (uint32_t) ((intptr_t)destAddr + blockSize);
        }
        else
        {
            dmacDescReg->DMAC_DSTADDR = (uint32_t) (destAddr);
        }

        /* Calculate the beat size and then set the BTCNT value */
        beat_size = (dmacDescReg->DMAC_BTCTRL & DMAC_BTCTRL_BEATSIZE_Msk) >> DMAC_BTCTRL_BEATSIZE_Pos;

        /* Set Block Transfer Count */
        dmacDescReg->DMAC_BTCNT = blockSize / (1 << beat_size);

        /* Save channel ID */
        channelId = (uint8_t)DMAC->CHID.reg;

        /* Set the DMA channel */
        DMAC->CHID.reg = channel;

        /* Enable the channel */
        DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;

        /* Verify if Trigger source is Software Trigger */
        if ((((DMAC->CHCTRLB.reg & DMAC_CHCTRLB_TRIGSRC_Msk) >> DMAC_CHCTRLB_TRIGSRC_Pos) == 0x00)
            && ((DMAC->CHCTRLB.reg & DMAC_CHCTRLB_EVIE) != DMAC_CHCTRLB_EVIE))
        {
            /* Trigger the DMA transfer */
            DMAC->SWTRIGCTRL.reg |= (1 << channel);
        }

        /* Restore channel ID */
        DMAC->CHID.reg = channelId;

        returnStatus = true;
    }

    return returnStatus;
}

void irq_handler_dmac( void )
{
    DMAC_CH_OBJECT  *dmacChObj = NULL;
    uint8_t channel = 0;
    uint8_t channelId = 0;
    volatile uint32_t chanIntFlagStatus = 0;
    DMAC_TRANSFER_EVENT event = DMAC_TRANSFER_EVENT_ERROR;

    /* Get active channel number */
    channel = DMAC->INTPEND.reg & DMAC_INTPEND_ID_Msk;

    dmacChObj = (DMAC_CH_OBJECT *)&dmacChannelObj[channel];

    /* Save channel ID */
    channelId = (uint8_t)DMAC->CHID.reg;

    /* Update the DMAC channel ID */
    DMAC->CHID.reg = channel;

    /* Get the DMAC channel interrupt status */
    chanIntFlagStatus = DMAC->CHINTFLAG.reg;

    /* Verify if DMAC Channel Transfer complete flag is set */
    if (chanIntFlagStatus & DMAC_CHINTENCLR_TCMPL)
    {
        /* Clear the transfer complete flag */
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL;

        event = DMAC_TRANSFER_EVENT_COMPLETE;

        dmacChObj->busyStatus = false;
    }

    /* Verify if DMAC Channel Error flag is set */
    if (chanIntFlagStatus & DMAC_CHINTENCLR_TERR)
    {
        /* Clear transfer error flag */
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TERR;

        event = DMAC_TRANSFER_EVENT_ERROR;

        dmacChObj->busyStatus = false;
    }

    /* Execute the callback function */
    if (dmacChObj->callback != NULL)
    {
        dmacChObj->callback (event, dmacChObj->context);
    }

    /* Restore channel ID */
    DMAC->CHID.reg = channelId;

    dma_complete_cb();
}

