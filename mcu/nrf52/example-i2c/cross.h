#pragma once

extern uint16_t m_conn_handle;
extern ble_lbs_t m_lbs;
extern void *m_qwr_void;;

void uart_init();
void services_init(void);
