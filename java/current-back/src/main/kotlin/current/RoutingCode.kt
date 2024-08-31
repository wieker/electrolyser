package org.example.current

import io.ktor.http.*
import io.ktor.server.application.*
import io.ktor.server.request.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import io.ktor.server.thymeleaf.*
import org.example.dto.Measure
import java.awt.image.BufferedImage
import java.awt.image.BufferedImage.TYPE_INT_RGB
import java.time.LocalDate
import javax.imageio.ImageIO

fun Application.configureRouting() {


    routing {
        route("/check") {
            get {
                call.respond("Test")
            }
        }

        route("/png") {
            get {
                call.response.header("Content-Type", "image/png")
                val image = BufferedImage(100, 100, TYPE_INT_RGB)

                val graphics = image.graphics
                graphics.drawArc(10, 20, 30, 40, 50, 60)

                call.respondOutputStream(ContentType.Application.OctetStream) {
                    ImageIO.write(image, "png", this)
                }
            }
        }

        route("/measures") {
            get {
                call.respond(
                    ThymeleafContent("measures", mapOf("measures" to listOf(Measure("t,", LocalDate.now(), 0.1, 0.2))))
                )
            }
        }

        route("/measure/add") {
            post {
                val someValue = call.receiveParameters()["priority"]
                call.respond(
                    ThymeleafContent("measures",
                        mapOf(
                                "measures" to listOf(Measure("t,", LocalDate.now(), 0.1, 0.2)),
                                "someValue" to someValue!!
                            ))
                )
            }
        }
    }
}