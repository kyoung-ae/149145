#include <stdio.h>
#include <sqlite3.h>
#include "BaseDefine.h"
#include "DB.h"

int createDB() {
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char *sql;
    char *sql_fk; // Foreign Key 활성화 변수

    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
    	fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    sql_fk = "PRAGMA foreign_keys = 1"; // Foreign Key 활성화(1) 시킴
    rc = sqlite3_exec(db, sql_fk, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "참조키 활성화 에러 : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else
        return 0;

    //admin table create
   	sql = "CREATE TABLE ADMIN ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "access INTEGER(1),"\
        "pwd TEXT(513));";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create ADMIN table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created admin table successfully\n");
   	}

    //mac table create
    sql = "CREATE TABLE MAC ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "public_key TEXT(270),"\
        "mac0 TEXT(17) NOT NULL,"\
        "mac1 TEXT(17),"\
        "mac2 TEXT(17),"\
        "CONSTRAINT id_fk FOREIGN KEY(id) REFERENCES ADMIN(id));";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create MAC table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created mac table successfully\n");
    }

   	//info table create
   	sql = "CREATE TABLE INFO ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "name TEXT(30),"\
        "birth TEXT(6),"\
        "email TEXT(50),"\
        "phone TEXT(20),"\
        "date TEXT(12),"\
        "CONSTRAINT id_fk FOREIGN KEY(id) REFERENCES ADMIN(id));";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create INFO table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created info table successfully\n");
    }

    //whitelist table create
    sql = "CREATE TABLE WHITELIST ("\
        "whitelist TEXT(30) PRIMARY KEY NOT NULL,"\
        "id TEXT(9) NOT NULL,"\
        "date TEXT(12),"\
        "CONSTRAINT id_fk FOREIGN KEY(id) REFERENCES ADMIN(id));";
   	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create WHITELIST table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created whitelist table successfully\n");
    }

    sqlite3_close(db);

    return 0;
}
