package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.material3.Button
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.AbsoluteAlignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.longdo.mjpegviewer.MjpegView
import com.longdo.mjpegviewer.MjpegViewUDP

@Composable
internal fun BlinkyControlView(
    ledState: Boolean,
    sliderPosition: Float,
    adcState: Int,
    onStateChanged: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
    dump: String,
    throttleValue: (Float) -> Unit,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {
        LedControlView(
            state = ledState,
            onStateChanged = onStateChanged,
        )

        Row {
            Button(onClick = { /*TODO*/ }) {
                Text(text = "Calib")
            }
            Button(onClick = { /*TODO*/ }) {
                Text(text = "1")
            }
            Button(onClick = { /*TODO*/ }) {
                Text(text = "2")
            }
            Button(onClick = { /*TODO*/ }) {
                Text(text = "3")
            }
            Button(onClick = { /*TODO*/ }) {
                Text(text = "4")
            }
            Button(onClick = { /*TODO*/ }) {
                Text(text = "Run")
            }
        }
        Row {
            Button(onClick = { /*TODO*/ }) {
                Text(text = "Trim")
            }
        }

        ADCControlView(state = adcState)

        Box {
            AndroidView(factory = {context ->
                MjpegViewUDP(context).apply {
                    setUrl("http://192.168.43.1:8080")
                    setAdjustHeight(true)
                    setMode(MjpegView.MODE_FIT_WIDTH)
                    setRecycleBitmap(true)
                    startStream()
                }
            },
                modifier = Modifier.alpha(0.5f))

            Text(text = dump)

            Slider(value = sliderPosition, onValueChange = throttleValue, modifier = Modifier
                .rotate(270f)
                .align(
                    AbsoluteAlignment.TopRight
                ))
        }
    }
}
