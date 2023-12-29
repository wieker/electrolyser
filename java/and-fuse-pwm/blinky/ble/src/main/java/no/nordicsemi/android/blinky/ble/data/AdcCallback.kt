package no.nordicsemi.android.blinky.ble.data

import android.bluetooth.BluetoothDevice
import no.nordicsemi.android.ble.callback.profile.ProfileReadResponse
import no.nordicsemi.android.ble.data.Data

abstract class AdcCallback: ProfileReadResponse() {

    override fun onDataReceived(device: BluetoothDevice, data: Data) {
        var state = IntArray(6)
        if (data.size() == 2 * 6) {
            var i = 0
            while (i < 6) {
                val buttonState = data.getIntValue(Data.FORMAT_SINT16, i * 2)
                buttonState?.let { state[i] = it }
                i ++
            }
            onButtonStateChanged(device, state)
        } else {
            onInvalidDataReceived(device, data)
        }
    }

    abstract fun onButtonStateChanged(device: BluetoothDevice, state: IntArray)
}