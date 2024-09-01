package org.example.current

import io.ktor.http.*
import io.ktor.server.application.*
import io.ktor.server.request.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import io.ktor.server.thymeleaf.*
import org.example.current.enterprise.MyBatisEnterpriseXMLMapper
import org.example.dto.Measure
import org.koin.ktor.ext.inject
import java.awt.image.BufferedImage
import java.awt.image.BufferedImage.TYPE_INT_RGB
import java.time.LocalDateTime
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
            val dbModule: DbModule by inject<DbModule>()
            get {
                call.respond(
                    ThymeleafContent("measures",
                        mapOf(
                            "measures" to dbModule.session().getMapper(MyBatisEnterpriseXMLMapper::class.java).getMeasures(),
                            "someValue" to "none",
                            "curDate" to LocalDateTime.now()
                        ))
                )
            }
        }

        route("/measure/add") {
            val dbModule: DbModule by inject<DbModule>()
            post {
                val receiveParameters = call.receiveParameters()
                val type = receiveParameters["type"]
                val measure = Measure(null, type,
                    LocalDateTime.now(),
                    receiveParameters["value"]!!.toDouble(),
                    receiveParameters["duration"]!!.toDouble())
                dbModule.session().let { s ->
                    s.getMapper(MyBatisEnterpriseXMLMapper::class.java).insertMeasure(measure)
                    s.commit()
                }
                call.respond(
                    ThymeleafContent("measures",
                        mapOf(
                                "measures" to dbModule.session().getMapper(MyBatisEnterpriseXMLMapper::class.java).getMeasures(),
                                "someValue" to type!!,
                                "curDate" to LocalDateTime.now()
                            ))
                )
            }
        }
    }
}