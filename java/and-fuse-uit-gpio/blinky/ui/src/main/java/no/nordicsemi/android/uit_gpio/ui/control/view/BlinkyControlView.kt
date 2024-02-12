package no.nordicsemi.android.uit_gpio.ui.control.view

import Joystick.JoystickMovedListener
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.requiredHeight
import androidx.compose.material3.Button
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp

@Composable
internal fun BlinkyControlView(
    adcState: IntArray,
    modifier: Modifier = Modifier,
    gpioValue: (Int) -> Unit,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {

        ADCControlView(state = adcState)
        Button(onClick = { gpioValue(1) }) {
            Text(text = "ON")
        }
        Button(onClick = { gpioValue(0) }) {
            Text(text = "OFF")
        }
    }
}
