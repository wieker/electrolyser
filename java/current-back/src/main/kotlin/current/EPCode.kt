package org.example.current

import io.ktor.serialization.kotlinx.json.*
import io.ktor.server.application.*
import io.ktor.server.plugins.contentnegotiation.*

fun Application.configureEP() {


    install(ContentNegotiation) {
        json()
    }
}