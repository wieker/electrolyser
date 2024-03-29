package no.nordicsemi.android.blinky.spec

import Joystick.JoyValues
import kotlinx.coroutines.flow.StateFlow

interface Blinky {

    enum class State {
        LOADING,
        READY,
        NOT_AVAILABLE
    }

    /**
     * Connects to the device.
     */
    suspend fun connect()

    /**
     * Disconnects from the device.
     */
    fun release()

    /**
     * The current state of the blinky.
     */
    val state: StateFlow<State>

    /**
     * The current state of the button.
     */
    val buttonState: StateFlow<Boolean>
    val adcState: StateFlow<IntArray>
    val dump: StateFlow<String>
    val sliderPos: StateFlow<Float>
    val sliderProcess: StateFlow<Boolean>
    val joy: JoyValues

    /**
     * Controls the LED state.
     *
     * @param state the new state of the LED.
     */
    suspend fun turnADC(cmdCode: Int)
    suspend fun sendCmd(cmd: String)
    suspend fun sendCmd(cmd: ByteArray)
    suspend fun turnThrottle(v: Float)
}