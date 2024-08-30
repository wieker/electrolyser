package org.example

import io.ktor.server.application.*
import io.ktor.server.engine.*
import io.ktor.server.netty.*
import org.example.current.*

fun main() {
    System.setProperty("java.awt.headless", "true")
    println("Hello World!")
    embeddedServer(Netty, port = 8082, module = Application::prodModules).start(true)
}

@Suppress("UNUSED")
fun Application.prodModules() {
    koin()
    configureRouting()
    configureTemplating()
    configureEP()
    initDB()
}