package org.example

import com.zaxxer.hikari.HikariConfig
import com.zaxxer.hikari.HikariDataSource
import io.ktor.serialization.kotlinx.json.*
import io.ktor.server.application.*
import io.ktor.server.engine.*
import io.ktor.server.netty.*
import io.ktor.server.plugins.contentnegotiation.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import liquibase.Liquibase
import liquibase.database.jvm.JdbcConnection
import liquibase.resource.ClassLoaderResourceAccessor
import org.apache.ibatis.mapping.Environment
import org.apache.ibatis.session.Configuration
import org.apache.ibatis.session.SqlSession
import org.apache.ibatis.session.SqlSessionFactoryBuilder
import org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory
import org.koin.core.context.loadKoinModules
import org.koin.dsl.module
import org.koin.ktor.ext.inject
import org.koin.ktor.plugin.Koin
import org.koin.logger.slf4jLogger
import javax.sql.DataSource

fun main() {
    println("Hello World!")
    embeddedServer(Netty, port = 8082, module = Application::prodModules).start(true)
}

@Suppress("UNUSED")
fun Application.prodModules() {
    install(Koin) {
        slf4jLogger()
        modules(module {
            single<DataSource> { dataSource() }
        })
    }
    install(ContentNegotiation) {
        json()
    }
    updateDbSchema()


    routing {
        route("/check") {
            get {
                call.respond("Test")
            }
        }
    }

}

fun Application.updateDbSchema() {
    val database     by inject<DataSource>()
    database.let {
        Liquibase("changelog.sql",
            ClassLoaderResourceAccessor(),
            JdbcConnection(it.connection)
        )
            .update("dev")
    }
    loadKoinModules(module { DbModule(database) })
}

class DbModule(ds: DataSource) {
    val config: Configuration

    init {
        val environment = Environment(
            "todo",
            JdbcTransactionFactory(),
            ds
        )
        config = Configuration(environment)
    }

    fun session(): SqlSession {
        return SqlSessionFactoryBuilder().build(config).openSession()
    }
}

fun dataSource(): DataSource {
    val ds = HikariDataSource(hikariConfig())
    return ds
}

private fun hikariConfig() = HikariConfig().apply {
    isAutoCommit = false
    jdbcUrl = "jdbc:h2:mem:testdb"
    poolName = "name"
    driverClassName = "org.h2.Driver"
    connectionTestQuery = "select 1"
    username = "sa"
    password = ""
    validate()
}