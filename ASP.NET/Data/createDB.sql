/*==============================================================*/
/* DBMS name:      PostgreSQL 9.x                               */
/* Created on:     2019/7/26 11:33:58                           */
/*==============================================================*/


drop table weather;

/*==============================================================*/
/* Table: weather                                               */
/*==============================================================*/
create table weather (
   weather_id           SERIAL not null,
   date_time            TIMESTAMP            null,
   temperature          NUMERIC(4,2)         null,
   humidity             NUMERIC(4,2)         null,
   pressure             NUMERIC(10,2)        null,
   dust                 NUMERIC(4)           null,
   uv                   DECIMAL(4,2)         null,
   constraint PK_WEATHER primary key (weather_id)
);

