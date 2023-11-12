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
    turnADC: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
    dump: String,
    throttleValue: (Float) -> Unit,
    commandValue: (String) -> Unit,
    sliderProcess: Boolean,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {
        LedControlView(
            state = ledState,
            turnADC = turnADC,
        )

        Row {
            Button(onClick = { commandValue("g") }) {
                Text(text = "g")
            }
            Button(onClick = { commandValue("1") }) {
                Text(text = "1")
            }
            Button(onClick = { commandValue("2") }) {
                Text(text = "2")
            }
            Button(onClick = { commandValue("3") }) {
                Text(text = "3")
            }
            Button(onClick = { commandValue("4") }) {
                Text(text = "4")
            }
        }
        Row {
            Button(onClick = { commandValue("m") }) {
                Text(text = "Seq")
            }
            Button(onClick = { commandValue("v") }) {
                Text(text = "Zero")
            }
            Button(onClick = { commandValue("0") }) {
                Text(text = "Stop")
            }
            Button(onClick = { commandValue("-") }) {
                Text(text = "-10")
            }
            Button(onClick = { commandValue("=") }) {
                Text(text = "+50")
            }
        }
        Row {
            Button(onClick = { commandValue("a") }) {
                Text(text = "a")
            }
            Button(onClick = { commandValue("s") }) {
                Text(text = "s")
            }
            Button(onClick = { commandValue("d") }) {
                Text(text = "d")
            }
            Button(onClick = { commandValue("w") }) {
                Text(text = "w")
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
                ),
                enabled = sliderProcess,
                steps = 33,
            )
        }
    }
}
