package org.example.current

import io.ktor.server.application.*
import org.koin.dsl.module
import org.koin.ktor.plugin.Koin
import org.koin.logger.slf4jLogger
import javax.sql.DataSource

fun Application.koin() {
    install(Koin) {
        slf4jLogger()
        modules(module {
            single<DataSource> { dataSource() }
        })
    }
}