package no.nordicsemi.android.blinky.ble.data

import no.nordicsemi.android.ble.data.Data

class CtlData private constructor() {

    companion object {
        fun from(value: Byte): Data {
            return Data.opCode(value)
        }
    }

}