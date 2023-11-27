package no.nordicsemi.android.blinky.ble

import Joystick.JoyValues
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCharacteristic
import android.content.Context
import android.os.Build
import android.util.Log
import kotlinx.coroutines.*
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.sync.Semaphore
import no.nordicsemi.android.ble.BleManager
import no.nordicsemi.android.ble.data.Data
import no.nordicsemi.android.ble.ktx.asValidResponseFlow
import no.nordicsemi.android.ble.ktx.getCharacteristic
import no.nordicsemi.android.ble.ktx.state.ConnectionState
import no.nordicsemi.android.ble.ktx.stateAsFlow
import no.nordicsemi.android.ble.ktx.suspend
import no.nordicsemi.android.blinky.ble.data.ADCState
import no.nordicsemi.android.blinky.ble.data.AdcCallback
import no.nordicsemi.android.blinky.ble.data.ButtonCallback
import no.nordicsemi.android.blinky.spec.Blinky
import no.nordicsemi.android.blinky.spec.BlinkySpec
import timber.log.Timber
import kotlin.math.floor

class BlinkyManager(
    context: Context,
    device: BluetoothDevice
): Blinky by BlinkyManagerImpl(context, device)

private class BlinkyManagerImpl(
    context: Context,
    private val device: BluetoothDevice,
): BleManager(context), Blinky {
    private val scope = CoroutineScope(Dispatchers.IO)

    private var ledCharacteristic: BluetoothGattCharacteristic? = null
    private var adcCharacteristic: BluetoothGattCharacteristic? = null
    private var rxCharacteristic: BluetoothGattCharacteristic? = null
    private var txCharacteristic: BluetoothGattCharacteristic? = null

    private val _buttonState = MutableStateFlow(false)
    override val buttonState = _buttonState.asStateFlow()

    private val _adcState = MutableStateFlow(0)
    override val adcState = _adcState.asStateFlow()

    private val _sliderPos = MutableStateFlow(0.0f)
    override val sliderPos = _sliderPos.asStateFlow()
    var oldV = 0.0f

    private val _sliderProcess = MutableStateFlow(true)
    override val sliderProcess = _sliderProcess.asStateFlow()

    var dumpV = MutableStateFlow("s")
    override val dump = dumpV.asStateFlow()
    var cv = ""
    var cs = ""

    override val joy = JoyValues()

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


    private val buttonCallback by lazy {
        object : ButtonCallback() {
            override fun onButtonStateChanged(device: BluetoothDevice, state: Boolean) {
                _buttonState.tryEmit(state)
            }
        }
    }

    private val adcCallback by lazy {
        object : AdcCallback() {
            override fun onButtonStateChanged(device: BluetoothDevice, state: Int) {
                _adcState.tryEmit(state)
            }
        }
    }

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
        val arr = ByteArray(1)
        arr[0] = cmdCode.toByte()
        // Write the value to the characteristic.
        writeCharacteristic(
            ledCharacteristic,
            Data(arr),
            BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
        ).suspend()
    }

    override suspend fun sendCmd(cmd: String) {
        // Write the value to the characteristic.
        cc = cmd
    }

    var cc = "0"

    override suspend fun sendCmd(cmd: ByteArray) {
        // Write the value to the characteristic.
        writeCharacteristic(
            txCharacteristic,
            Data(cmd),
            BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
        ).suspend()
    }

    override suspend fun turnThrottle(v: Float) {
        _sliderPos.value = v
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
            ledCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_LED_CHARACTERISTIC_UUID,
                // Mind, that below we pass required properties.
                // If your implementation supports only WRITE_NO_RESPONSE,
                // change the property to BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE.
                BluetoothGattCharacteristic.PROPERTY_WRITE
            )
            txCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_TX_CHARACTERISTIC_UUID,
                // Mind, that below we pass required properties.
                // If your implementation supports only WRITE_NO_RESPONSE,
                // change the property to BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE.
                BluetoothGattCharacteristic.PROPERTY_WRITE
            )
            // Get the Button characteristic.
            rxCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_RX_CHARACTERISTIC_UUID,
                BluetoothGattCharacteristic.PROPERTY_NOTIFY
            )
            // Get the Button characteristic.
            adcCharacteristic = getCharacteristic(
                BlinkySpec.BLINKY_ADC_CHARACTERISTIC_UUID,
                BluetoothGattCharacteristic.PROPERTY_NOTIFY
            )

            // Return true if all required characteristics are supported.
            return ledCharacteristic != null && txCharacteristic != null &&
                    rxCharacteristic != null &&
                    adcCharacteristic != null
        }
        return false
    }

    fun here(data: Data)
    {
        val receivedValue = if (data.value == null) "nn" else String(data.value!!, Charsets.UTF_8)
        val lines = receivedValue.split("\n")
        cs += lines[0];
        for (i in 1..(lines.size - 1))  {
            cv = cs + '\n' + cv
            cs = lines[i]
        }
        cv = cv.substring(0, Integer.min(200, cv.length))
        dumpV.tryEmit(cs + '\n' + cv)
        Timber.log(10, "here");

    }
    val lock = Semaphore(1)

    @OptIn(ExperimentalCoroutinesApi::class)
    override fun initialize() {
        // Enable notifications for the button characteristic.
        setNotificationCallback(this.rxCharacteristic)
            .with { device, data -> here(data)
            }

        enableNotifications(this.rxCharacteristic)
            .enqueue()

        // Read the initial value of the button characteristic.
        readCharacteristic(this.rxCharacteristic)
            .with(buttonCallback)
            .enqueue()

        // Enable notifications for the ADC characteristic.
        val adcFlow: Flow<ADCState> = setNotificationCallback(adcCharacteristic)
            .asValidResponseFlow()

        // Forward the button state to the buttonState flow.
        scope.launch {
            adcFlow.map { it.state }.collect { _adcState.tryEmit(it) }
        }

        enableNotifications(adcCharacteristic)
            .enqueue()

        // Read the initial value of the button characteristic.
        readCharacteristic(adcCharacteristic)
            .with(adcCallback)
            .enqueue()

        Thread {
            var started = false
            var autoCommand = 0
            var specCmd = "c"
            while (true) {

                try {
                    if (lock.tryAcquire()) {
                        if (!started && _sliderPos.value >= 0.3f) {
                            autoCommand = 10
                            specCmd = "m"
                            started = true
                        }
                        if (started && _sliderPos.value < 0.2f) {
                            autoCommand = 10
                            specCmd = "0"
                            started = false
                        }
                        if (cc.equals("c") && autoCommand > 0) {
                            cc = specCmd
                            autoCommand --
                        }
                        try {
                            val a = ByteArray(10)
                            a[0] = 't'.code.toByte()
                            a[1] = 'q'.code.toByte()
                            a[2] = cc[0].code.toByte()
                            a[3] = floor(_sliderPos.value * 100).toInt().toByte()
                            a[4] = joy.x.toByte()
                            a[5] = joy.y.toByte()
                            a[6] = '0'.code.toByte()
                            a[7] = '0'.code.toByte()
                            a[8] = 'e'.code.toByte()
                            Timber.log(10, "locked " + a[5]);
                            cc = "c";
                            writeCharacteristic(
                                txCharacteristic,
                                Data(a),
                                BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE
                            )
                                .then { lock.release() }
                                .invalid { lock.release() }
                                .enqueue()
                        } finally {

                        }
                        oldV = _sliderPos.value
                    }

                    Thread.sleep(30)
                } finally {
                }
            }
        }.start()
    }

    override fun onServicesInvalidated() {
        ledCharacteristic = null
        rxCharacteristic = null
        txCharacteristic = null
        adcCharacteristic = null
    }
}