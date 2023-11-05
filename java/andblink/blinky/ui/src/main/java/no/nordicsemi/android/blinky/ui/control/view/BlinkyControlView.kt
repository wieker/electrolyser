package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.longdo.mjpegviewer.MjpegView
import com.longdo.mjpegviewer.MjpegViewUDP

@Composable
internal fun BlinkyControlView(
    ledState: Boolean,
    buttonState: Boolean,
    adcState: Int,
    onStateChanged: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
    dump: String,
) {
    Box {
        Slider(value = 0.5f, onValueChange = {})
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

            AndroidView(factory = {context ->
                MjpegViewUDP(context).apply {
                    setUrl("http://192.168.43.1:8080")
                    setAdjustHeight(true)
                    setMode(MjpegView.MODE_FIT_WIDTH)
                    setRecycleBitmap(true)
                    startStream()
                }
            })

            Text(text = dump)
        }
    }
}
