package no.nordicsemi.android.blinky.ble.data

import android.bluetooth.BluetoothDevice

class ADCState: AdcCallback() {
    var state: Int = 10

    override fun onButtonStateChanged(device: BluetoothDevice, state: Int) {
        this.state = state
    }
}