package no.nordicsemi.android.uit_gpio.ui.control.view

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
    dump: String,
    modifier: Modifier = Modifier,
    gpioValue: (Int) -> Unit,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
        horizontalAlignment = Alignment.Start
    ) {

        Text(text = dump)
        Button(onClick = { gpioValue(1) }) {
            Text(text = "ON")
        }
        Button(onClick = { gpioValue(0) }) {
            Text(text = "OFF")
        }
    }
}
