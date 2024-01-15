package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.RadioButtonChecked
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedCard
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ColorFilter
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import no.nordicsemi.android.blinky.ui.R
import no.nordicsemi.android.common.theme.NordicTheme

@Composable
internal fun ADCControlView(
    state: IntArray,
    modifier: Modifier = Modifier,
) {
    OutlinedCard(
        modifier = modifier
    ) {
        Column(
            modifier = Modifier
                .padding(16.dp)
        ) {
            var i = 0;
            while (i < 6) {
                Row(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(vertical = 8.dp),
                    verticalAlignment = Alignment.CenterVertically,
                ) {
                    Text(
                        text = "ADC: #" + i,
                        modifier = Modifier.weight(1f)
                    )
                    Text(
                        text = "I="  + (state[i].toFloat() * 0.6f * 10 / 1024).toString() + " A",
                    )
                }

                i ++
            }
        }
    }
}

@Composable
@Preview
private fun ButtonControlViewPreview() {
    NordicTheme {
        ButtonControlView(
            state = true,
            modifier = Modifier.padding(16.dp),
        )
    }
}