-- liquibase formatted sql

--changeset author:ch1
create table measure (
type text,
date timestamp,
value numeric,
duration numeric
);