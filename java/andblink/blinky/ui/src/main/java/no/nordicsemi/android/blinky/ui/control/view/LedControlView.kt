package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.OutlinedCard
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import no.nordicsemi.android.blinky.ui.R
import no.nordicsemi.android.common.theme.NordicTheme

@Composable
internal fun LedControlView(
    state: Boolean,
    turnADC: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
) {
    OutlinedCard(
        modifier = modifier
    ) {
        Row {
            Column(
            ) {
                Button(onClick = { turnADC(false) }) {
                    Text(text = "Current ADC")
                }
            }
            Column(
            ) {
                Button(onClick = { turnADC(false) }) {
                    Text(text = "Pulse ADC")
                }
            }
        }
    }
}

@Composable
@Preview
private fun LecControlViewPreview() {
    NordicTheme {
        LedControlView(
            state = true,
            turnADC = {},
            modifier = Modifier.padding(16.dp),
        )
    }
}