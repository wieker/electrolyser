package no.nordicsemi.android.blinky.ui.control.viewmodel

import android.app.Application
import android.content.Context
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import dagger.hilt.android.lifecycle.HiltViewModel
import dagger.hilt.android.qualifiers.ApplicationContext
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import no.nordicsemi.android.blinky.ui.control.repository.BlinkyRepository
import no.nordicsemi.android.common.logger.LoggerLauncher
import javax.inject.Inject
import javax.inject.Named

/**
 * The view model for the Blinky screen.
 *
 * @param context The application context.
 * @property repository The repository that will be used to interact with the device.
 * @property deviceName The name of the Blinky device, as advertised.
 */
@HiltViewModel
class BlinkyViewModel @Inject constructor(
    @ApplicationContext context: Context,
    private val repository: BlinkyRepository,
    @Named("deviceName") val deviceName: String,
) : AndroidViewModel(context as Application) {
    /** The connection state of the device. */
    val state = repository.state
    val sliderProcess = repository.sliderProcess
        .stateIn(viewModelScope, SharingStarted.Lazily, true)
    /** The button state. */
    val buttonState = repository.loggedButtonState
        .stateIn(viewModelScope, SharingStarted.Lazily, false)
    val sliderPos = repository.sliderPos
        .stateIn(viewModelScope, SharingStarted.Lazily, 0.0f)
    val adcState = repository.loggedADCState
        .stateIn(viewModelScope, SharingStarted.Lazily, IntArray(6))
    val dump = repository.dump.stateIn(viewModelScope, SharingStarted.Lazily, "v")
    val joy = repository.joy

    init {
        // In this sample we want to connect to the device as soon as the view model is created.
        connect()
    }

    /**
     * Connects to the device.
     */
    fun connect() {
        val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
        viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
            // This method may throw an exception if the connection fails,
            // Bluetooth is disabled, etc.
            // The exception will be caught by the exception handler and will be ignored.
            repository.connect()
        }
    }

    /**
     * Sends a command to the device to toggle the LED state.
     * @param on The new state of the LED.
     */
    fun turnADC(cmdCode: Int) {
        val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
        viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
            // Just like above, when this method throws an exception, it will be caught by the
            // exception handler and ignored.
            repository.turnADC(cmdCode)
        }
    }
    fun turnThrottle(v: Float) {
        val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
        viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
            // Just like above, when this method throws an exception, it will be caught by the
            // exception handler and ignored.
            repository.turnThrottle(v)
        }
    }
    fun cmdSend(str: String) {
        val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
        viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
            // Just like above, when this method throws an exception, it will be caught by the
            // exception handler and ignored.
            repository.sendCmd(str)
        }
    }

    /**
     * Opens nRF Logger app with the log or Google Play if the app is not installed.
     */
    fun openLogger() {
        LoggerLauncher.launch(getApplication(), repository.sessionUri)
    }

    override fun onCleared() {
        super.onCleared()
        repository.release()
    }
}