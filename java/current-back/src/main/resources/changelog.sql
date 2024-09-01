-- liquibase formatted sql

--changeset author:ch1
create table measure (
type text,
date timestamp,
value numeric,
duration numeric
);


--changeset author:ch2
alter table measure add column id bigserial primary key;