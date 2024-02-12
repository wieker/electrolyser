package no.nordicsemi.android.uit_gpio.ui.control.view

import Joystick.JoystickMovedListener
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.offset
import androidx.compose.foundation.layout.requiredHeight
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp

@Composable
internal fun BlinkyControlView(
    sliderPosition: Float,
    adcState: IntArray,
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

        ADCControlView(state = adcState)

        Box (Modifier.requiredHeight(Dp(500f))) {

            Slider(value = sliderPosition, onValueChange = throttleValue, modifier = Modifier
                .rotate(270f).offset(Dp(-200f), Dp(150f)),
                enabled = sliderProcess,
                steps = 33,
            )

            Text(text = dump)
        }
    }
}
