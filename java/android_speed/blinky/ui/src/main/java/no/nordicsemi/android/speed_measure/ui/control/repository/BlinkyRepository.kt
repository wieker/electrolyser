package no.nordicsemi.android.speed_measure.ui.control.repository

import android.content.Context
import android.net.Uri
import dagger.hilt.android.qualifiers.ApplicationContext
import no.nordicsemi.android.speed_measure.spec.Blinky
import no.nordicsemi.android.log.timber.nRFLoggerTree
import timber.log.Timber
import javax.inject.Inject
import javax.inject.Named

/**
 *
 * @param context The application context.
 * @param deviceId The device ID.
 * @param deviceName The name of the Blinky device, as advertised.
 * @property blinky The Blinky implementation.
 */
class BlinkyRepository @Inject constructor(
    @ApplicationContext context: Context,
    @Named("deviceId") deviceId: String,
    @Named("deviceName") deviceName: String,
    private val blinky: Blinky,
): Blinky by blinky {
    /** Timber tree that logs to nRF Logger. */
    private val tree: Timber.Tree

    /** If the nRF Logger is installed, this URI will allow to open the session. */
    internal val sessionUri: Uri?

    init {
        // Plant a new Tree that logs to nRF Logger.
        tree = nRFLoggerTree(context, null, deviceId, deviceName)
            .also { Timber.plant(it) }
            .also { sessionUri = it.session?.sessionUri }
    }

    override fun release() {
        Timber.uproot(tree)
        blinky.release()
    }
}