package org.example.current

import com.zaxxer.hikari.HikariConfig
import com.zaxxer.hikari.HikariDataSource
import io.ktor.server.application.*
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
import javax.sql.DataSource


fun Application.initDB() {
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
    jdbcUrl = "jdbc:postgresql://localhost:5432/current_mon"
    poolName = "name"
    driverClassName = "org.postgresql.Driver"
    connectionTestQuery = "select 1"
    username = "current_mon"
    password = "current_mon"
    validate()
}