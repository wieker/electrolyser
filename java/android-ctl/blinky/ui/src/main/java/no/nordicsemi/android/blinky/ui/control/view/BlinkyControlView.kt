package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import no.nordicsemi.android.blinky.ui.control.viewmodel.BlinkyViewModel

@Composable
internal fun BlinkyControlView(
    ledState: Boolean,
    buttonState: Boolean,
    adcState: Int,
    onStateChanged: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
    viewModel: BlinkyViewModel
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {
        LedControlView(
            state = ledState,
            onStateChanged = onStateChanged,
        )

        ButtonControlView(
            state = buttonState
        )

        ADCControlView(state = adcState)


        Row {
            Button(
                onClick = {
                    val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
                    viewModel.viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
                        // Just like above, when this method throws an exception, it will be caught by the
                        // exception handler and ignored.
                        viewModel.getRepo().sendCommand('m'.code.toByte())
                    }
                },
            ) {
                Text(text = "start")
            }
            Button(
                onClick = {
                    val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
                    viewModel.viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
                        // Just like above, when this method throws an exception, it will be caught by the
                        // exception handler and ignored.
                        viewModel.getRepo().sendCommand('0'.code.toByte())
                    }
                },
            ) {
                Text(text = "stop")
            }
        }
        Row {
            Button(
                onClick = {
                    val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
                    viewModel.viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
                        // Just like above, when this method throws an exception, it will be caught by the
                        // exception handler and ignored.
                        viewModel.getRepo().sendCommand('='.code.toByte())
                    }
                },
            ) {
                Text(text = "inc")
            }
            Button(
                onClick = {
                    val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
                    viewModel.viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
                        // Just like above, when this method throws an exception, it will be caught by the
                        // exception handler and ignored.
                        viewModel.getRepo().sendCommand('-'.code.toByte())
                    }
                },
            ) {
                Text(text = "dec")
            }
        }
        Button(
            onClick = {
                val exceptionHandler = CoroutineExceptionHandler { _, _ -> }
                viewModel.viewModelScope.launch(Dispatchers.IO + exceptionHandler) {
                    // Just like above, when this method throws an exception, it will be caught by the
                    // exception handler and ignored.
                    viewModel.getRepo().sendCommand('g'.code.toByte())
                }
            },
        ) {
            Text(text = "cal")
        }
    }
}
