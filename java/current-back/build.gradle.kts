plugins {
    kotlin("jvm") version "1.9.23"
    id("io.ktor.plugin") version "2.3.10"
    kotlin("plugin.serialization") version "1.9.22"
    id("io.kotest.multiplatform") version "5.8.1"
}

group = "org.example"
version = "1.0-SNAPSHOT"
val ktor_version: String="2.3.10"
val koin_version="3.5.6"

repositories {
    mavenCentral()
}

dependencies {

    // ktor
    implementation("io.ktor:ktor-server-core-jvm")
    implementation("io.ktor:ktor-serialization-kotlinx-json-jvm")
    implementation("io.ktor:ktor-server-content-negotiation-jvm")
    implementation("io.ktor:ktor-server-status-pages:$ktor_version")
    implementation("io.ktor:ktor-server-swagger-jvm")
    implementation("io.ktor:ktor-server-openapi")
    implementation("io.ktor:ktor-server-netty-jvm")
    implementation("io.ktor:ktor-server-metrics-micrometer-jvm")
    implementation("io.ktor:ktor-client-cio:$ktor_version")
    implementation("io.ktor:ktor-client-encoding:$ktor_version")
    implementation("io.ktor:ktor-client-logging:$ktor_version")
    implementation("io.ktor:ktor-server-auth:$ktor_version")
    implementation("io.ktor:ktor-server-cors:$ktor_version")

    // koin
    implementation("io.insert-koin:koin-ktor:$koin_version")
    implementation("io.insert-koin:koin-logger-slf4j:$koin_version")

    implementation("org.postgresql:postgresql:42.7.3")
    implementation("com.h2database:h2:2.1.214")
    implementation("org.liquibase:liquibase-core:4.27.0")
    implementation("com.zaxxer:HikariCP:5.1.0")
    implementation ("org.mybatis:mybatis:3.5.16")
    implementation ("org.mybatis.dynamic-sql:mybatis-dynamic-sql:1.5.1")

    testImplementation(kotlin("test"))
}

tasks.test {
    useJUnitPlatform()
}
kotlin {
    jvmToolchain(17)
}