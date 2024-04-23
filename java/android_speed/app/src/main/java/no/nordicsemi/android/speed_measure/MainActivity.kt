package no.nordicsemi.android.speed_measure

import android.os.Bundle
import androidx.activity.compose.setContent
import dagger.hilt.android.AndroidEntryPoint
import no.nordicsemi.android.speed_measure.ui.control.BlinkyDestination
import no.nordicsemi.android.speed_measure.ui.scanner.ScannerDestination
import no.nordicsemi.android.common.navigation.NavigationView
import no.nordicsemi.android.common.theme.NordicActivity
import no.nordicsemi.android.common.theme.NordicTheme

@AndroidEntryPoint
class MainActivity: NordicActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            NordicTheme {
                NavigationView(ScannerDestination + BlinkyDestination)
            }
        }
    }
}