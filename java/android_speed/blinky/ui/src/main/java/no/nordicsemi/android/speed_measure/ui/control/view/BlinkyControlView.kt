package no.nordicsemi.android.speed_measure.ui.control.view

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.RadioButtonChecked
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedCard
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ColorFilter
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import no.nordicsemi.android.blinky.ui.R

@Composable
internal fun BlinkyControlView(
    modifier: Modifier = Modifier,
    gpioValue: (Int) -> Unit,
    timerVALUE: Int,
    timer0VALUE: Int,
    gpioValue0: Int,
    gpioValue1: Int,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {

        Button(onClick = { gpioValue(1) }) {
            Text(text = "ON")
        }
        Button(onClick = { gpioValue(0) }) {
            Text(text = "OFF")
        }
        Row {
            Text(text = timerVALUE.toString())
        }
        Row {
            Text(text = timer0VALUE.toString())
        }
        gpioSenseElement(modifier, gpioValue0)
        gpioSenseElement(modifier, gpioValue1)
    }
}

@Composable
private fun gpioSenseElement(modifier: Modifier, gpioValue0: Int) {
        Column(
            modifier = Modifier
                .padding(16.dp)
        ) {
            Row(
                modifier = Modifier
                    .fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
            ) {
                Image(
                    imageVector = Icons.Default.RadioButtonChecked,
                    contentDescription = null,
                    modifier = Modifier.padding(end = 16.dp),
                    colorFilter = ColorFilter.tint(MaterialTheme.colorScheme.onSurface)
                )
                Text(
                    text = stringResource(R.string.blinky_button),
                    style = MaterialTheme.typography.headlineMedium,
                )
            }
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 8.dp),
                verticalAlignment = Alignment.CenterVertically,
            ) {
                Text(
                    text = stringResource(R.string.blinky_button_descr),
                    modifier = Modifier.weight(1f)
                )
                Text(
                    text = if (gpioValue0 == 1) stringResource(R.string.blinky_on) else stringResource(
                        R.string.blinky_off
                    ),
                )
            }
        }
}

