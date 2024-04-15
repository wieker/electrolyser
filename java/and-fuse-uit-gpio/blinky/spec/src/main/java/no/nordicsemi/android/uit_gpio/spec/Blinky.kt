package no.nordicsemi.android.uit_gpio.spec

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

    val dump: StateFlow<IntArray>
    val timer: StateFlow<Int>

    /**
     * Controls the LED state.
     *
     * @param state the new state of the LED.
     */
    suspend fun turnADC(cmdCode: Int)
}