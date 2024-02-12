package no.nordicsemi.android.blinky.ble.data

import android.bluetooth.BluetoothDevice

class ADCState: AdcCallback() {
    var state: IntArray = intArrayOf(1, 2, 3, 4, 5, 6)

    override fun onButtonStateChanged(device: BluetoothDevice, state: IntArray) {
        this.state = state
    }
}