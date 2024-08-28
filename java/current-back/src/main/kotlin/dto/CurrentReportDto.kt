package org.example.dto

import java.time.LocalDate

data class CurrentReportDto(
    val androidDate: LocalDate,
    val currentSecond: Double,
    val currentMinute: Double,
    val currentMinute10: Double,
    val currentHour: Double,
    val currentHour6: Double,
    val currentHour24: Double,
    val current1: Double,
)
