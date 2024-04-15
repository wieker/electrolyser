package no.nordicsemi.android.uit_gpio.ble

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
import no.nordicsemi.android.uit_gpio.spec.Blinky
import no.nordicsemi.android.uit_gpio.spec.BlinkySpec
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
    private var adcCharacteristic: BluetoothGattCharacteristic? = null
    private var timerCharacteristic: BluetoothGattCharacteristic? = null

    var dumpV = IntArray(1000)
    val dumpMagicVar = MutableStateFlow(IntArray(1000))
    override val dump = dumpMagicVar.asStateFlow()
    private val _timer = MutableStateFlow(8)
    override val timer = _timer.asStateFlow()
    var pos = 0
    val lock = Semaphore(1)

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
        if (lock.tryAcquire()) {
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
            // Get the Button characteristic.
            adcCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_ADC_CHARACTERISTIC_UUID,
                BluetoothGattCharacteristic.PROPERTY_NOTIFY
            )
            // Get the Timer characteristic.
            timerCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_TIMER_CHARACTERISTIC_UUID,
                BluetoothGattCharacteristic.PROPERTY_NOTIFY
            )

            // Return true if all required characteristics are supported.
            return pwmCharacteristic != null &&
                    adcCharacteristic != null && timerCharacteristic != null
        }
        return false
    }

    fun here(data: Data)
    {
        if (data.size() == 2 * 6) {
            var i = 0
            while (i < 6) {
                val buttonState = data.getIntValue(Data.FORMAT_SINT16, i * 2)
                if (pos < 1000) {
                    buttonState?.let { dumpV[pos++] = it }
                }
                i ++
            }
        }
        if (pos >= 1000) {
            dumpMagicVar.tryEmit(dumpV)
            lock.release()
        }
        Timber.log(10, "here");

    }

    override fun initialize() {
        // Enable notifications for the button characteristic.
        setNotificationCallback(this.adcCharacteristic)
            .with { device, data -> here(data)
            }

        enableNotifications(adcCharacteristic)
            .enqueue()

        setNotificationCallback(this.timerCharacteristic).with { device, data -> data.getIntValue(Data.FORMAT_UINT32, 0)?.let { _timer.tryEmit(it) } }
        enableNotifications(timerCharacteristic)
            .enqueue()
    }

    override fun onServicesInvalidated() {
        pwmCharacteristic = null
        adcCharacteristic = null
        timerCharacteristic = null
    }
}