package org.example.current.enterprise

import org.apache.ibatis.annotations.Insert
import org.apache.ibatis.annotations.Param
import org.apache.ibatis.annotations.Results
import org.apache.ibatis.annotations.Result
import org.apache.ibatis.annotations.Select
import org.example.dto.Measure

interface MyBatisEnterpriseXMLMapper {
    @Insert("        INSERT INTO measure (type, date, value, duration)\n" +
            "        VALUES\n" +
            "        (#{model.type},\n" +
            "        #{model.date},\n" +
            "        #{model.value},\n" +
            "        #{model.duration});")
    fun insertMeasure(@Param("model") model: Measure)

    @Select("SELECT * FROM measure")
    @Results(id ="measure", value = [
        Result(id = true, property = "id", column = "id"),
        Result(property = "type", column = "type"),
        Result(property = "date", column = "date"),
        Result(property = "value", column = "value"),
        Result(property = "duration", column = "duration")
    ])
    fun getMeasures(): List<Measure>

}