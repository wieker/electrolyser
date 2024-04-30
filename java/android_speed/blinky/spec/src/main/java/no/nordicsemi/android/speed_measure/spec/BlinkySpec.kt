package no.nordicsemi.android.speed_measure.spec

import java.util.UUID

class BlinkySpec {

    companion object {
        val BLINKY_SERVICE_UUID: UUID = UUID.fromString("00001623-1212-efde-1523-785feabcd123")
        val BLINKY_RX_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001528-1212-efde-1523-785feabcd123")
        val BLINKY_PWM_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001525-1212-efde-1523-785feabcd123")
        val BLINKY_ADC_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001526-1212-efde-1523-785feabcd123")
        val BLINKY_TX_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001527-1212-efde-1523-785feabcd123")
        val BLINKY_TIMER_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001529-1212-efde-1523-785feabcd123")
        val BLINKY_GPIO_CHARACTERISTIC_UUID: UUID = UUID.fromString("00001530-1212-efde-1523-785feabcd123")
    }

}