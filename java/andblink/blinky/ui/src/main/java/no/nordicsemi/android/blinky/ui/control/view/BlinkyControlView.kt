package no.nordicsemi.android.blinky.ui.control.view

import Joystick.JoystickMovedListener
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.requiredHeight
import androidx.compose.foundation.layout.size
import androidx.compose.material3.Button
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.AbsoluteAlignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.MobileAnarchy.Android.Widgets.Joystick.JoystickView
import com.longdo.mjpegviewer.MjpegView
import com.longdo.mjpegviewer.MjpegViewUDP
import com.manalkaff.jetstick.JoyStick
import no.nordicsemi.android.blinky.ui.R

@Composable
internal fun BlinkyControlView(
    sliderPosition: Float,
    adcState: Int,
    turnADC: (Int) -> Unit,
    modifier: Modifier = Modifier,
    dump: String,
    throttleValue: (Float) -> Unit,
    commandValue: (String) -> Unit,
    sliderProcess: Boolean,
    joy: JoystickMovedListener,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {
        ModCtlControlView(
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
            Button(onClick = { commandValue("0") }) {
                Text(text = "Stop")
            }
        }
        Row {
            Button(onClick = { commandValue("a") }) {
                Text(text = "X-")
            }
            Button(onClick = { commandValue("s") }) {
                Text(text = "Y-")
            }
            Button(onClick = { commandValue("d") }) {
                Text(text = "X+")
            }
            Button(onClick = { commandValue("w") }) {
                Text(text = "Y+")
            }
        }

        ADCControlView(state = adcState)

        Box (Modifier.requiredHeight(Dp(500f))) {
            JoyStick(backgroundImage = R.drawable.joystick_background_1,
                dotImage = R.drawable.joystick_dot_1,
                size = 300.dp,
                modifier = Modifier.alpha(0.5f),
                moved = {x, y -> joy.OnMoved(x, y)})

            Slider(value = sliderPosition, onValueChange = throttleValue, modifier = Modifier
                .rotate(270f).offset(Dp(-200f), Dp(150f)),
                enabled = sliderProcess,
                steps = 33,
            )

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
        }
    }
}
