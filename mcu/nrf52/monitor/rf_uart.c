//
// Created by wieker on 1/17/20.
//


#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>
#include <proprietary_rf/esb/nrf_esb.h>
#include <sdk_macros.h>
#include <string.h>
#include "i2cfunc.h"


void clocks_start( void )
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

static nrf_esb_payload_t        rx_payload;
int count = 0;
int pos = 0;
int init = 1;
int wait = 0;

void advertise();

void wait_and_blink();

int rcvd[256];

static nrf_esb_config_t nrf_esb_config = NRF_ESB_DEFAULT_CONFIG;
static nrf_esb_payload_t tx_payload, rx_payload;
static volatile bool radio_tx_busy = false, radio_started = false;

void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
{
    switch (p_event->evt_id)
    {
        case NRF_ESB_EVENT_TX_SUCCESS:
            radio_tx_busy = false;
            nrf_esb_disable();
            nrf_esb_config.mode = NRF_ESB_MODE_PRX;
            nrf_esb_init(&nrf_esb_config);
            nrf_esb_start_rx();
            break;
        case NRF_ESB_EVENT_TX_FAILED:
            radio_tx_busy = false;
            break;
        case NRF_ESB_EVENT_RX_RECEIVED:
            if (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS)
            {
                nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "rx", 2);
                nrfx_uart_tx(&m_uart.uart, (uint8_t  *) rx_payload.data, rx_payload.length);
                int rcvr = rx_payload.data[0] - '0';
                int new_c = rx_payload.data[2] - '0';
                count = new_c > count ? new_c : count;
                if (init) {
                    count ++;
                    pos = count;
                    init = 0;
                } else if (wait && (rcvr == pos - 1)) {
                    wait = 0;
                }
            }
            break;
    }
}

uint32_t radio_init(uint16_t network_id)
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xC5, 0xE7, 0xE7, 0x3A};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

    nrf_esb_config.payload_length           = 250;
    nrf_esb_config.tx_mode                  = NRF_ESB_TXMODE_AUTO;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.selective_auto_ack       = true;

    base_addr_0[1] ^= (network_id & 0xFF);
    base_addr_0[2] ^= (network_id >> 8);

    err_code = nrf_esb_init(&nrf_esb_config);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_rf_channel(network_id % 64 + 4);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_start_rx();
    VERIFY_SUCCESS(err_code);

    radio_started = true;

    return err_code;
}

bool radio_packet_send(uint8_t *packet, uint32_t packet_length)
{
    if(radio_started)
    {
        while(radio_tx_busy);
        nrf_esb_stop_rx();
        nrf_esb_disable();
        nrf_esb_config.mode = NRF_ESB_MODE_PTX;
        nrf_esb_init(&nrf_esb_config);
        nrf_esb_start_tx();
        memcpy(tx_payload.data, packet, packet_length);
        tx_payload.length = packet_length;
        tx_payload.pipe = 0;
        tx_payload.noack = true;
        if(nrf_esb_write_payload(&tx_payload) == NRF_SUCCESS) radio_tx_busy = true;
        return true;
    }
    return false;
}

int main() {
    NRF_P0->DIRSET = 1 << 22 | 1 << 18 | 1 << 19 | 1 << 20;
    uart_init();

    clocks_start();

    radio_init(1);
    nrf_esb_config.mode = NRF_ESB_MODE_PRX;
    nrf_esb_init(&nrf_esb_config);
    nrf_esb_start_rx();

    for (int i = 0; i < 20; i ++) {
        wait_and_blink();
    }

    if (!init) {
        wait = 1;
        while (wait) {
            nrf_delay_ms(10);
        }
        wait_and_blink();
    } else {
        pos = 1;
        count = 1;
    }

    while (true)
    {
        advertise();

        for (int i = 0; i < count + 1; i ++) {
            wait_and_blink();
        }
    }
}

void wait_and_blink() {
    NRF_P0->OUTSET = 1 << 20 | 1 << 22;
    NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
    nrf_delay_ms(500);
    NRF_P0->OUTSET = 1 << 18 | 1 << 19;
    NRF_P0->OUTCLR = 1 << 20 | 1 << 22;
    nrf_delay_ms(500);
}

void advertise() {
    char data[5];
    data[0] = '0' + pos;
    data[1] = 'c';
    data[2] = '0' + count;
    data[3] = '\r';
    data[4] = '\n';
    radio_packet_send((uint8_t *) data, sizeof(data));
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "tx", 2);
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) data, sizeof(data));
}

