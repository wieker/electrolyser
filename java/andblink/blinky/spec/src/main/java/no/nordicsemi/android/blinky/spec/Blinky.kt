package no.nordicsemi.android.blinky.spec

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
     * The current state of the LED.
     */
    val ledState: StateFlow<Boolean>

    /**
     * The current state of the button.
     */
    val buttonState: StateFlow<Boolean>
    val adcState: StateFlow<Int>
    val dump: StateFlow<String>
    val sliderPos: StateFlow<Float>

    /**
     * Controls the LED state.
     *
     * @param state the new state of the LED.
     */
    suspend fun turnLed(state: Boolean)
    suspend fun sendCmd(cmd: String)
    suspend fun turnThrottle(v: Float)
}