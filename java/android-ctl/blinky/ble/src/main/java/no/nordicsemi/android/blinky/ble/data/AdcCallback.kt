package no.nordicsemi.android.blinky.ble.data

import android.bluetooth.BluetoothDevice
import no.nordicsemi.android.ble.callback.profile.ProfileReadResponse
import no.nordicsemi.android.ble.data.Data

abstract class AdcCallback: ProfileReadResponse() {

    override fun onDataReceived(device: BluetoothDevice, data: Data) {
        if (data.size() == 2) {
            val buttonState = data.getIntValue(Data.FORMAT_SINT16, 0)
            buttonState?.let { onButtonStateChanged(device, it) }
        } else {
            onInvalidDataReceived(device, data)
        }
    }

    abstract fun onButtonStateChanged(device: BluetoothDevice, state: Int)
}