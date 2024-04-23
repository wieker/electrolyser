package no.nordicsemi.android.speed_measure

import android.app.Application
import dagger.hilt.android.HiltAndroidApp
import timber.log.Timber

@HiltAndroidApp
class HiltApplication : Application() {

    override fun onCreate() {
        super.onCreate()

        // Initialize Timber. By default, the library will log to the Android logcat.
        Timber.plant(Timber.DebugTree())
    }
}