package no.nordicsemi.android.blinky.ui.control.view

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import no.nordicsemi.android.common.theme.NordicTheme

@Composable
internal fun BlinkyControlView(
    ledState: Boolean,
    buttonState: Boolean,
    adcState: Int,
    onStateChanged: (Boolean) -> Unit,
    modifier: Modifier = Modifier,
) {
    Column(
        modifier = modifier,
        verticalArrangement = Arrangement.spacedBy(16.dp),
    ) {
        LedControlView(
            state = ledState,
            onStateChanged = onStateChanged,
        )

        ButtonControlView(
            state = buttonState
        )

        ADCControlView(state = adcState)
    }
}

@Preview
@Composable
private fun BlinkyControlViewPreview() {
    NordicTheme {
        BlinkyControlView(
            ledState = true,
            buttonState = true,
            adcState = 0,
            onStateChanged = {},
            modifier = Modifier.padding(16.dp),
        )
    }
}