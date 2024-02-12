package no.nordicsemi.android.uit_gpio.ui.scanner

import androidx.hilt.navigation.compose.hiltViewModel
import no.nordicsemi.android.uit_gpio.ui.control.Blinky
import no.nordicsemi.android.uit_gpio.ui.control.BlinkyDevice
import no.nordicsemi.android.uit_gpio.ui.scanner.view.BlinkyScanner
import no.nordicsemi.android.common.navigation.createSimpleDestination
import no.nordicsemi.android.common.navigation.defineDestination
import no.nordicsemi.android.common.navigation.viewmodel.SimpleNavigationViewModel

val Scanner = createSimpleDestination("scanner")

val ScannerDestination = defineDestination(Scanner) {
    val viewModel: SimpleNavigationViewModel = hiltViewModel()

    BlinkyScanner(
        onDeviceSelected = { device, name ->
            viewModel.navigateTo(Blinky, BlinkyDevice(device, name))
        }
    )
}