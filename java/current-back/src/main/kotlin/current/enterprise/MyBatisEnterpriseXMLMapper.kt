package org.example.current.enterprise

import org.apache.ibatis.annotations.Insert
import org.apache.ibatis.annotations.Param
import org.example.dto.Measure

interface MyBatisEnterpriseXMLMapper {
    @Insert("        INSERT INTO measure (type, date, value, duration)\n" +
            "        VALUES\n" +
            "        (#{model.type},\n" +
            "        #{model.date},\n" +
            "        #{model.value},\n" +
            "        #{model.duration});")
    fun insertMeasure(@Param("model") model: Measure)
}