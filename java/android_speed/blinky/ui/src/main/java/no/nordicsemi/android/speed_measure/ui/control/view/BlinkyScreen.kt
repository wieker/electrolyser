package no.nordicsemi.android.speed_measure.ui.control.view

import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.widthIn
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import no.nordicsemi.android.blinky.ui.R
import no.nordicsemi.android.speed_measure.ui.control.viewmodel.BlinkyViewModel
import no.nordicsemi.android.speed_measure.spec.Blinky
import no.nordicsemi.android.scanner.view.DeviceConnectingView
import no.nordicsemi.android.scanner.view.DeviceDisconnectedView
import no.nordicsemi.android.scanner.view.Reason
import no.nordicsemi.android.common.logger.view.LoggerAppBarIcon
import no.nordicsemi.android.common.permissions.ble.RequireBluetooth
import no.nordicsemi.android.common.theme.view.NordicAppBar

@OptIn(ExperimentalMaterial3Api::class)
@Composable
internal fun BlinkyScreen(
    onNavigateUp: () -> Unit,
) {
    val viewModel: BlinkyViewModel = hiltViewModel()
    val state by viewModel.state.collectAsStateWithLifecycle()

    Column(
        horizontalAlignment = Alignment.CenterHorizontally,
    ) {
        NordicAppBar(
            text = viewModel.deviceName,
            onNavigationButtonClick = onNavigateUp,
            actions = {
                LoggerAppBarIcon(onClick = { viewModel.openLogger() })
            }
        )
        RequireBluetooth {
            when (state) {
                Blinky.State.LOADING -> {
                    DeviceConnectingView(
                        modifier = Modifier.padding(16.dp),
                    ) { padding ->
                        Button(
                            onClick = onNavigateUp,
                            modifier = Modifier.padding(padding),
                        ) {
                            Text(text = stringResource(id = R.string.action_cancel))
                        }
                    }
                }
                Blinky.State.READY -> {
                    val timerState by viewModel.timerState.collectAsStateWithLifecycle()
                    val timer0State by viewModel.timer0State.collectAsStateWithLifecycle()
                    val gpio0State by viewModel.gpio0State.collectAsStateWithLifecycle()
                    val gpio1State by viewModel.gpio1State.collectAsStateWithLifecycle()

                    BlinkyControlView(
                        modifier = Modifier
                            .widthIn(max = 460.dp)
                            .verticalScroll(rememberScrollState())
                            .padding(16.dp),
                        gpioValue = {v -> viewModel.turnGPIO(v)},
                        timerState,
                        timer0State,
                        gpio0State,
                        gpio1State,
                    )
                }
                Blinky.State.NOT_AVAILABLE -> {
                    DeviceDisconnectedView(
                        reason = Reason.LINK_LOSS,
                        modifier = Modifier.padding(16.dp),
                    ) { padding ->
                        Button(
                            onClick = { viewModel.connect() },
                            modifier = Modifier.padding(padding),
                        ) {
                            Text(text = stringResource(id = R.string.action_retry))
                        }
                    }
                }
            }
        }
    }
}