package org.example.dto

import java.time.LocalDateTime

class Measure (
    var type: String? = null,
    var date: LocalDateTime? = null,
    var value: Double? = null,
    var duration: Double? = null,
)