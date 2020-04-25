#define _CTR_SECURE_NO_WARNINGS // prevent compilation errors due to scanf security warnings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "BaseDefine.h"
#include "DBlen.h"

//void printMenu();

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

// DBProgram.c 로 파일명을 바꿔야 함 (DB 확인을 위해 main.c 로 작성한 상태임)
int main(int argc, char *argv[]) { // int selMenu(int menu) 로 함수명을 바꿔야 함 (DB 확인을 위해 main 함수로 작성한 상태임)

    //int menu;
    //printMenu(menu);

    sqlite3 *db;
   	char *errmsg;
    sqlite3_stmt *res;
    int rc;
    char *sql;
    char input_sql[512];
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    char date[DATElen];
    char src_white[WLlen] = { 0, }; // 수정할 화이트리스트 ->다른 화이트리스트로 변경됨.

    rc = sqlite3_open("WHITELIST.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open WHITELIST DB : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    /* table create
    sql = "CREATE TABLE WHITELIST ("\
        "whitelist TEXT(30) PRIMARY KEY NOT NULL,"\
        "id TEXT(9) NOT NULL,"\
        "date TEXT(12));";
   	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
       	fprintf(stderr, "Can't create WHITELIST table : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
    else {
       	fprintf(stderr, "Created table successfully\n");
    }
    */

    system("clear");

    /* records insert
    puts("WHITELIST TABLE's insert\n");
    puts("input whitelist:");
    gets(whitelist);
    puts("input id:");
    gets(id);
    puts("input date:");
    gets(date);

    fflush(stdin);
    strcpy(input_sql, "insert into WHITELIST values('");
    strcat(input_sql, whitelist); //whitelist
    strcat(input_sql, "','");
    strcat(input_sql, id); //id
    strcat(input_sql, "','");
    //sql = "select datetimes(strftime('%s', 'now'), 'unixepoch', 'localtime'))"; //time, date
    strcat(input_sql, date); //date
    strcat(input_sql, "');");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print input successfully\n");
    }
    */

    // 화이트리스트 테이블 확인
    sql = "select * from WHITELIST";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }


    /*records delete
    res = "Callback Function Called";

    puts("WHITELIST TABLE's records delete.\n");
    puts("input whitelist:");
    gets(whitelist);
    puts("input id:");
    gets(id);

    fflush(stdin);
    strcpy(input_sql, "DELETE from WHITELIST where whitelist = '");
    strcat(input_sql, whitelist); //whitelist
    strcat(input_sql, "';");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, res, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't delete : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Delete successfully\n");
    }
    */

    // records update
    res = "Callback Function Called";

    puts("WHITELIST TABLE's Update\n");
    puts("update source whitelist:");
    gets(src_white);
    puts("update whitelist:");
    gets(whitelist);

    fflush(stdin);
    strcpy(input_sql, "UPDATE WHITELIST SET whitelist = '");
    strcat(input_sql, whitelist); //whitelist
    strcat(input_sql, "'");
    strcat(input_sql, "where whitelist = '");
    strcat(input_sql, src_white);
    strcat(input_sql, "';");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, res, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't Update : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Update successfully\n");
    }

    // 화이트리스트 테이블 확인
    sql = "select * from WHITELIST";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }

    sqlite3_close(db);

    return 0;
}


/*	if(menu == 2) {
    	rc = sqlite3_open("ADMINISTRATOR.db", &db);
    	if(rc != SQLITE_OK) {
      		fprintf(stderr, "Can't open ADMINISTRATOR DB : %s\n", sqlite3_errmsg(db));
        	return 1;
        }
   		else {
       		fprintf(stderr, "Opened database successfully\n");
    	}
    	sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    	sql = "CREATE TABLE MAC ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "public_key TEXT(270),"\
        "mac0 TEXT(17) NOT NULL,"\
        "mac1 TEXT(17),"\
        "mac2 TEXT(17));";
    	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    	if(rc != SQLITE_OK) {
        	fprintf(stderr, "Can't create MAC table : %s\n", sqlite3_errmsg(db));
        	return 1;
    	}
    	else {
        	fprintf(stderr, "Created table successfully\n");
    	}

    	sql = "CREATE TABLE ADMIN ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "access INT(1),"\
        "pwd TEXT(513));";
    	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    	if(rc != SQLITE_OK) {
        	fprintf(stderr, "Can't create ADMIN table : %s\n", sqlite3_errmsg(db));
        	return 1;
    	}
    	else {
        	fprintf(stderr, "Created table successfully\n");
    	}

    	sql = "CREATE TABLE INFO ("\
        "id TEXT(9) PRIMARY KEY NOT NULL,"\
        "access INT(1),"\
        "name TEXT(30),"\
        "birth TEXT(6),"\
        "email TEXT(50),"\
        "phone TEXT(20),"\
        "date TEXT(12));";
    	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    	if(rc != SQLITE_OK) {
        	fprintf(stderr, "Can't create INFO table : %s\n", sqlite3_errmsg(db));
        	return 1;
    	}
    	else {
        	fprintf(stderr, "Created table successfully\n");
    	}
	}
*/

/*
    sqlite3_close(db);

	return 0;
} */
