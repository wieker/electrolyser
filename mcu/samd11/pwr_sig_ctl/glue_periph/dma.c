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

#define DMAC_CHANNELS_NUMBER        1

static uint8_t busyStatus;

/* Initial write back memory section for DMAC */
static  dmac_descriptor_registers_t _write_back_section[DMAC_CHANNELS_NUMBER]    __attribute__ ((aligned (16)));

/* Descriptor section for DMAC */
static  dmac_descriptor_registers_t  descriptor_section[DMAC_CHANNELS_NUMBER]    __attribute__ ((aligned (16)));

/* Descriptor section for DMAC */
static  dmac_descriptor_registers_t  descriptor_chain_3   __attribute__ ((aligned (16)));
static int fake = 0;

void dma_init()
{
  busyStatus = false;

  /* Update the Base address and Write Back address register */
  DMAC->BASEADDR.reg = (uint32_t) descriptor_section;
  DMAC->WRBADDR.reg  = (uint32_t)_write_back_section;

  /* Update the Priority Control register */
  DMAC->PRICTRL0.reg = DMAC_PRICTRL0_LVLPRI0(1) | DMAC_PRICTRL0_RRLVLEN0 | DMAC_PRICTRL0_LVLPRI1(1) | DMAC_PRICTRL0_RRLVLEN1 | DMAC_PRICTRL0_LVLPRI2(1) | DMAC_PRICTRL0_RRLVLEN2 | DMAC_PRICTRL0_LVLPRI3(1) | DMAC_PRICTRL0_RRLVLEN3;

  /***************** Configure DMA channel 0 ********************/

  DMAC->CHID.reg = 0;

  DMAC->CHCTRLB.reg = DMAC_CHCTRLB_TRIGACT(2) | DMAC_CHCTRLB_TRIGSRC(ADC_DMAC_ID_RESRDY) | DMAC_CHCTRLB_LVL(0) |
          DMAC_CHCTRLB_EVOE;

  descriptor_section[0].DMAC_BTCTRL = DMAC_BTCTRL_BLOCKACT_NOACT | DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_VALID |
          DMAC_BTCTRL_DSTINC | DMAC_BTCTRL_EVOSEL_BEAT;
  descriptor_chain_3.DMAC_BTCTRL = DMAC_BTCTRL_BLOCKACT_INT | DMAC_BTCTRL_BEATSIZE_BYTE | DMAC_BTCTRL_VALID;

  DMAC->CHINTENSET.reg = (DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL);

  /* Enable the DMAC module & Priority Level x Enable */
  DMAC->CTRL.reg = DMAC_CTRL_DMAENABLE | DMAC_CTRL_LVLEN0 | DMAC_CTRL_LVLEN1 | DMAC_CTRL_LVLEN2 | DMAC_CTRL_LVLEN3;


  NVIC_SetPriority(DMAC_IRQn, 3);
  NVIC_EnableIRQ(DMAC_IRQn);

  dma_descrs();

}

/*******************************************************************************
    This function schedules a DMA transfer on the specified DMA channel.
********************************************************************************/

void dma_start()
{
  if (busyStatus == false) {
    busyStatus = true;

    DMAC->CHID.reg = 0;
    DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
  }
}

void dma_descrs() {
  descriptor_section[0].DMAC_DESCADDR = (uint32_t) &descriptor_chain_3;
  descriptor_section[0].DMAC_DSTADDR = (uint32_t) ((intptr_t) (app_response_buffer + 32));
  descriptor_section[0].DMAC_SRCADDR = (uint32_t) ((const void *) &ADC->RESULT.reg);
  descriptor_section[0].DMAC_BTCNT = 32;

  descriptor_chain_3.DMAC_DESCADDR = (uint32_t) descriptor_section;
  descriptor_chain_3.DMAC_DSTADDR = ((intptr_t) &fake);
  descriptor_chain_3.DMAC_SRCADDR = (uint32_t) ((const void *) &ADC->RESULT.reg);
  descriptor_chain_3.DMAC_BTCNT = 1;
}

void irq_handler_dmac( void )
{
    uint8_t channel = 0;
    uint8_t channelId = 0;
    volatile uint32_t chanIntFlagStatus = 0;

  /* Get active channel number */
    channel = DMAC->INTPEND.reg & DMAC_INTPEND_ID_Msk;

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

      busyStatus = false;
    }

    /* Verify if DMAC Channel Error flag is set */
    if (chanIntFlagStatus & DMAC_CHINTENCLR_TERR)
    {
        /* Clear transfer error flag */
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TERR;

      busyStatus = false;
    }

    /* Restore channel ID */
    DMAC->CHID.reg = channelId;

    dma_complete_cb();
}

