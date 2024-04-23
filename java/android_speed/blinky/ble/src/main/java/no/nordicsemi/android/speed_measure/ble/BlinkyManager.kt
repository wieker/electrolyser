package no.nordicsemi.android.speed_measure.ble

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCharacteristic
import android.content.Context
import android.util.Log
import kotlinx.coroutines.*
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.sync.Semaphore
import no.nordicsemi.android.ble.BleManager
import no.nordicsemi.android.ble.data.Data
import no.nordicsemi.android.ble.ktx.getCharacteristic
import no.nordicsemi.android.ble.ktx.state.ConnectionState
import no.nordicsemi.android.ble.ktx.stateAsFlow
import no.nordicsemi.android.ble.ktx.suspend
import no.nordicsemi.android.speed_measure.spec.Blinky
import no.nordicsemi.android.speed_measure.spec.BlinkySpec
import timber.log.Timber

class BlinkyManager(
    context: Context,
    device: BluetoothDevice
): Blinky by BlinkyManagerImpl(context, device)

private class BlinkyManagerImpl(
    context: Context,
    private val device: BluetoothDevice,
): BleManager(context), Blinky {
    private val scope = CoroutineScope(Dispatchers.IO)

    private var pwmCharacteristic: BluetoothGattCharacteristic? = null
    private var timerCharacteristic: BluetoothGattCharacteristic? = null

    private val _timer = MutableStateFlow(8)
    override val timer = _timer.asStateFlow()
    private val _timer0 = MutableStateFlow(8)
    override val timer0 = _timer0.asStateFlow()
    var pos = 0

    override val state = stateAsFlow()
        .map {
            when (it) {
                is ConnectionState.Connecting,
                is ConnectionState.Initializing -> Blinky.State.LOADING
                is ConnectionState.Ready -> Blinky.State.READY
                is ConnectionState.Disconnecting,
                is ConnectionState.Disconnected -> Blinky.State.NOT_AVAILABLE
            }
        }
        .stateIn(scope, SharingStarted.Lazily, Blinky.State.NOT_AVAILABLE)

    override suspend fun connect() = connect(device)
        .retry(3, 300)
        .useAutoConnect(false)
        .timeout(3000)
        .suspend()

    override fun release() {
        // Cancel all coroutines.
        scope.cancel()

        val wasConnected = isReady
        // If the device wasn't connected, it means that ConnectRequest was still pending.
        // Cancelling queue will initiate disconnecting automatically.
        cancelQueue()

        // If the device was connected, we have to disconnect manually.
        if (wasConnected) {
            disconnect().enqueue()
        }
    }

    override suspend fun turnADC(cmdCode: Int) {
        pos = 0
        val arr = ByteArray(1)
        arr[0] = cmdCode.toByte()
        // Write the value to the characteristic.
        writeCharacteristic(
            pwmCharacteristic,
            Data(arr),
            BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
        ).suspend()
    }

    override fun log(priority: Int, message: String) {
        Timber.log(priority, message)
    }

    override fun getMinLogPriority(): Int {
        // By default, the library logs only INFO or
        // higher priority messages. You may change it here.
        return Log.VERBOSE
    }

    override fun isRequiredServiceSupported(gatt: BluetoothGatt): Boolean {
        // Get the LBS Service from the gatt object.
        gatt.getService(BlinkySpec.BLINKY_SERVICE_UUID)?.apply {
            // Get the LED characteristic.
            pwmCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_PWM_CHARACTERISTIC_UUID,
                // Mind, that below we pass required properties.
                // If your implementation supports only WRITE_NO_RESPONSE,
                // change the property to BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE.
                BluetoothGattCharacteristic.PROPERTY_WRITE
            )
            // Get the Timer characteristic.
            timerCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_TIMER_CHARACTERISTIC_UUID,
                BluetoothGattCharacteristic.PROPERTY_NOTIFY
            )

            // Return true if all required characteristics are supported.
            return pwmCharacteristic != null && timerCharacteristic != null
        }
        return false
    }

    override fun initialize() {
        setNotificationCallback(this.timerCharacteristic)
            .with { device, data ->
                data.getIntValue(Data.FORMAT_UINT32, 0)
                    ?.let { _timer.tryEmit(it) }
                data.getIntValue(Data.FORMAT_UINT32, 1)
                    ?.let { _timer0.tryEmit(it) }
            }
        enableNotifications(timerCharacteristic)
            .enqueue()
    }

    override fun onServicesInvalidated() {
        pwmCharacteristic = null
        timerCharacteristic = null
    }
}