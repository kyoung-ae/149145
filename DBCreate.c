#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "DB.h"
#include "BaseDefine.h"

int createDB() {
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char *sql;
    //char *sql_fk; // Foreign Key 활성화 변수

    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
    	fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "=CPS.db open=\n");
    }

    sqlite3_busy_timeout(db, 1000); //db open시 timeout 500ms로 설정

/*
    sql_fk = "PRAGMA foreign_keys = 1"; // Foreign Key 활성화(1) 시킴
    rc = sqlite3_exec(db, sql_fk, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "참조키 활성화 에러 : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else
        return 0;
*/

    //admin table create
   	sql = "CREATE TABLE ADMIN ("\
        "id TEXT(10) PRIMARY KEY NOT NULL,"\
        "access INT(1),"\
        "pwd TEXT(514));";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create ADMIN table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created ADMIN table successfully\n");
   	}

    //mac table create
    sql = "CREATE TABLE MAC ("\
        "id TEXT(10) PRIMARY KEY NOT NULL,"\
        "public_key TEXT(271),"\
        "mac0 TEXT(18) NOT NULL,"\
        "mac1 TEXT(18),"\
        "mac2 TEXT(18));";
        /*
        "mac2 TEXT(18),"\
        "CONSTRAINT id FOREIGN KEY(id) REFERENCES ADMIN(id));";
        */
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create MAC table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created MAC table successfully\n");
    }

   	//info table create
   	sql = "CREATE TABLE INFO ("\
        "id TEXT(10) PRIMARY KEY NOT NULL,"\
        "name TEXT(31),"\
        "birth TEXT(7),"\
        "email TEXT(51),"\
        "phone TEXT(21),"\
        "date TEXT(21));";
        /*
        "date TEXT(21),"\
        "CONSTRAINT id FOREIGN KEY(id) REFERENCES ADMIN(id));";
        */
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create INFO table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created INFO table successfully\n");
    }

    //whitelist table create
    sql = "CREATE TABLE WHITELIST ("\
        "whitelist TEXT(31) PRIMARY KEY NOT NULL,"\
        "id TEXT(10) NOT NULL,"\
        "date TEXT(21));";
        /*
        "date TEXT(20),"\
        "CONSTRAINT id FOREIGN KEY(id) REFERENCES ADMIN(id));";
        */
   	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create WHITELIST table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created WHITELIST table successfully\n");
    }

    sqlite3_close(db);

    return 0;
}
