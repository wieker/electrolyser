#include <stdint.h>
#include <legacy/nrf_drv_uart.h>
#include "ble_lbs.h"
#include "nrf_ble_qwr.h"
#include "adc_logic.h"
#include "cross.h"

BLE_LBS_DEF(m_lbs);                                                             /**< LED Button Service instance. */
NRF_BLE_QWR_DEF(m_qwr);


/**@brief Function for initializing services that will be used by the application.
 */
void services_init(void)
{
    ret_code_t         err_code;
    ble_lbs_init_t     init     = {0};
    nrf_ble_qwr_init_t qwr_init = {0};

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    m_qwr_void = &m_qwr;
    APP_ERROR_CHECK(err_code);

    // Initialize LBS.

    err_code = ble_lbs_init(&m_lbs, &init);
    APP_ERROR_CHECK(err_code);
}

void send_adc(nrf_saadc_value_t *vls, int size)
{
    //ble_lbs_on_uart_rx(m_conn_handle, &m_lbs, 4, "uuu\n");
    //return;
    ret_code_t err_code;
    err_code = ble_lbs_on_adc_timer(m_conn_handle, &m_lbs, vls);
    if (err_code != NRF_SUCCESS &&
        err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
        err_code != NRF_ERROR_INVALID_STATE &&
        err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
    {
        APP_ERROR_CHECK(err_code);
    }
}

uint8_t rx_buf[40][20];
uint16_t rx_len[40];
int buf_index = 0;

static void copy_data(int len, uint8_t* dt) {
    int i = 0;
    rx_len[buf_index] = len;
    while (i < len) {
        rx_buf[buf_index][i] = dt[i];
        i ++;
    }
    buf_index = (buf_index + 1) % 40;
}


void dump_adc(uint8_t* value, int len) {
    copy_data(len, value);
}
