#include <stdint.h>
#include <legacy/nrf_drv_uart.h>
#include "ble_lbs.h"
#include "nrf_ble_qwr.h"
#include "adc_logic.h"
#include "cross.h"
#include "nrf_drv_gpiote.h"

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

void send_timer_value(uint32_t cdata0, uint32_t cdata1)
{
    ret_code_t err_code;
    err_code = ble_lbs_update_tmrv(m_conn_handle, &m_lbs, cdata0, cdata1);
    if (err_code != NRF_SUCCESS &&
        err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
        err_code != NRF_ERROR_INVALID_STATE &&
        err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
    {
        APP_ERROR_CHECK(err_code);
    }
}

int pdata0 = 2;
int cdata0 = 1;
int pdata1 = 2;
int cdata1 = 1;

void send_gpio_toggle()
{
    ret_code_t err_code;
    if (pdata0 != cdata0 || pdata1 != cdata1) {
        cdata0 = pdata0;
        cdata1 = pdata1;
        err_code = ble_lbs_update_gpio(m_conn_handle, &m_lbs, cdata0, cdata1);
        if (err_code != NRF_SUCCESS &&
            err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
            err_code != NRF_ERROR_INVALID_STATE &&
            err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        {
            APP_ERROR_CHECK(err_code);
        }
    }
}

void mark_gpio_toggle()
{
    pdata0 = nrf_gpio_pin_read(4);
    pdata1 = nrf_gpio_pin_read(5);
}
