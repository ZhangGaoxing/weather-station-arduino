/*==============================================================*/
/* DBMS name:      PostgreSQL 9.x                               */
/* Created on:     2019/7/27 10:21:36                           */
/*==============================================================*/


drop table weather;

/*==============================================================*/
/* Table: weather                                               */
/*==============================================================*/
create table weather (
   weather_id           SERIAL not null,
   date_time            TIMESTAMP            null,
   weather_name         VARCHAR(10)          null,
   temperature          NUMERIC(4,2)         null,
   humidity             NUMERIC(4,2)         null,
   pressure             NUMERIC(10,2)        null,
   dust                 NUMERIC(4)           null,
   uv                   DECIMAL(4,2)         null,
   constraint PK_WEATHER primary key (weather_id)
);

