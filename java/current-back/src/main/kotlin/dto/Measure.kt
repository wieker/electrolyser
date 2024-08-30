package org.example.dto

import java.time.LocalDate

class Measure (
    var type: String? = null,
    var date: LocalDate? = null,
    var value: Double? = null,
    var duration: Double? = null,
)