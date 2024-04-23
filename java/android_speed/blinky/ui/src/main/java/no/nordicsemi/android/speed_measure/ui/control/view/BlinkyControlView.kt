package no.nordicsemi.android.speed_measure.ui.control.view

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

@Composable
internal fun BlinkyControlView(
    modifier: Modifier = Modifier,
    gpioValue: (Int) -> Unit,
    timerVALUE: Int,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
        horizontalAlignment = Alignment.Start
    ) {

        Button(onClick = { gpioValue(1) }) {
            Text(text = "ON")
        }
        Button(onClick = { gpioValue(0) }) {
            Text(text = "OFF")
        }
        Text(text = timerVALUE.toString())
    }
}
