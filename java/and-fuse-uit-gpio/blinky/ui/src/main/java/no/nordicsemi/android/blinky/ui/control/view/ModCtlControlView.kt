package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.OutlinedCard
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier

@Composable
internal fun ModCtlControlView(
    turnADC: (Int) -> Unit,
    modifier: Modifier = Modifier,
) {
    OutlinedCard(
        modifier = modifier
    ) {
        Row {
            Column(
            ) {
                Button(onClick = { turnADC(0) }) {
                    Text(text = "Dump ADC")
                }
            }
            Column(
            ) {
                Button(onClick = { turnADC(1) }) {
                    Text(text = "Flash Seq")
                }
            }
        }
    }
}
