#include <stdint.h>
#include <legacy/nrf_drv_uart.h>
#include "ble_lbs.h"
#include "nrf_ble_qwr.h"
#include "adc_logic.h"
#include "cross.h"

BLE_LBS_DEF(m_lbs);                                                             /**< LED Button Service instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                         /**< Context for the Queued Write module.*/

uint8_t tx_buf[100];
nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

/**@brief Function for handling write events to the LED characteristic.
 *
 * @param[in] p_lbs     Instance of LED Button Service to which the write applies.
 * @param[in] led_state Written/desired state of the LED.
 */
static void uart_tx_handler(uint16_t conn_handle, ble_lbs_t * p_lbs, int len, uint8_t * data)
{
    int i = 0;
    while (i < len) {
        tx_buf[i] = data[i];
        i ++;
    }
    nrfx_uart_tx(&m_uart.uart, tx_buf, len);
}



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
    init.uart_tx_handler = uart_tx_handler;

    err_code = ble_lbs_init(&m_lbs, &init);
    APP_ERROR_CHECK(err_code);
}

uint8_t rx_data[100];

uint8_t rx_buf[20][20];
uint16_t rx_len[20];
int buf_index = 0;
int buf_read_index = 0;

static void evh(nrfx_uart_event_t const * p_event,
         void *                    p_context)
{
    if (p_event->type == 1) {
        int i = 0;
        rx_len[buf_index] = p_event->data.rxtx.bytes;
        while (i < p_event->data.rxtx.bytes) {
            rx_buf[buf_index][i] = p_event->data.rxtx.p_data[i];
            i ++;
        }
        buf_index = (buf_index + 1) % 20;
    }
    nrfx_uart_rx(&m_uart.uart, p_event->data.rxtx.p_data, 20);
}

void uart_init()
{
  nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
  config.pseltxd  = 0;
  config.pselrxd  = 1;
  config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
  config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
  config.baudrate = (nrf_uart_baudrate_t)NRFX_UART_DEFAULT_CONFIG_BAUDRATE;
  nrfx_uart_init(&m_uart.uart,
                 (nrfx_uart_config_t const *)&config,
                 evh);
  nrfx_uart_rx_enable(&m_uart.uart);
  nrfx_uart_rx(&m_uart.uart, rx_data, 20);
  nrfx_uart_rx(&m_uart.uart, rx_data + 20, 20);
}

void send_adc(nrf_saadc_value_t *vls)
{
    //ble_lbs_on_uart_rx(m_conn_handle, &m_lbs, 4, "uuu\n");
    //return;
    ret_code_t err_code;
    err_code = ble_lbs_on_adc_timer(m_conn_handle, &m_lbs, vls[0]);
    if (err_code != NRF_SUCCESS &&
        err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
        err_code != NRF_ERROR_INVALID_STATE &&
        err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
    {
        APP_ERROR_CHECK(err_code);
    }
}


#include "nrf_drv_timer.h"
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
    static uint32_t i;

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE1:
            if (rx_len[buf_read_index] > 0) {
                ble_lbs_on_uart_rx(m_conn_handle, &m_lbs, rx_len[buf_read_index], rx_buf[buf_read_index]);
                rx_len[buf_read_index] = 0;
                buf_read_index = (buf_read_index + 1) % 20;
            }
            break;

        default:
            //Do nothing.
            break;
    }
}
