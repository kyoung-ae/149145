#define _CTR_SECURE_NO_WARNINGS

#pragma foreign_keys = 1 // 참조키 활성화

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio_ext.h>

#include "sqlite3.h"
#include "DBCreate.h"
#include "DBPrintModule.h"
#include "DBSearchModule.h"
#include "DBManage.h"
#include "DBManagement.h"
#include "DBInsertModule.h"
#include "DBUpdateModule.h"
#include "DBDeleteModule.h"
#include "DBlen.h"
#include "DB.h"
#include "DBLogin.h"
#include "DBBackupR.h"
#include "BaseDefine.h"
#include "DBProgram.h"
#include "DBd.h"
#include "DBins.h"
#include "DBs.h"
#include "DBup.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int sw() { // int selWhitelist(const char sel_wl[WLlen], OUT struct WhiteListTable sel_wt); // 화이트리스트 검색 // case 36
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;

    char sql_like[SQLlen] = { 0, }; // 입력한 whitelist의 like 검색

    char id[IDlen] = { 0, };
    char _whitelist[WLlen] = { 0, }; // 임시 whitelist
    char whitelist[WLlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열 (버퍼오버플로 방지를 위해 1024로 지정)

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    while(1) { // whitelist를 기준으로 먼저 부분 검색
        _whitelist[0] = '\0';
        str[0] = '\0';
        puts("Whitelist 검색은 먼저, 부분 문자열 검색으로 확인합니다.");
        puts("\nSearch whitelist (30bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize >=4 && strsize <= WLlen)
            break;
        else if(strsize < 4) {
            printf("3 btyes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, WLlen); // DBPrintModule.c
    }
    strncpy(_whitelist, str, WLlen-1);
    printf("입력한 whitelist : %s에 대한 부분 검색 결과입니다.\n", _whitelist);

    __fpurge(stdin);
    strncpy(sql_like, "SELECT whitelist FROM WHITELIST WHERE WHITELIST like '%", 55);
    strncat(sql_like, _whitelist, WLlen-1);
    strncat(sql_like, "%';", 3);
    rc = sqlite3_exec(db, sql_like, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't Search : %s (whitelist error)\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "위의 결과에서 whitelist 명령어를 찾아 정확하게 입력하세요.\n");
    }
    sql_like[0] = '\0';

    while(1) { // whitelist를 정확하게 입력 받음
        whitelist[0] = '\0';
        str[0] = '\0';
        puts("\nWhitelist 검색은 정확히 일치해야 검색됩니다.");
        puts("\nSearch whitelist (30bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize >=4 && strsize <= WLlen)
            break;
        else if(strsize < 4) {
            printf("3 btyes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, WLlen); // DBPrintModule.c
    }
    strncpy(whitelist, str, WLlen-1);

    if(checkWL(whitelist) == 0) { // DBManage.c : 사용자가 입력한 whitelist가 존재하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);

    selWhitelist(whitelist/*, sel_wt*/);

    return 0;
}

int sa() { // int selAdminInfo(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT struct InfoTable sel_it); // 관리자 정보 검색 // case 37
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;

    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열 (버퍼오버플로 방지를 위해 1024로 지정)

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    while(1) { // id 검색
        id[0] = '\0';
        str[0] = '\0';
        puts("\nSearch id (9bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize >= 6 && strsize <= IDlen) // id는 5~9bytes 길이 제한
            break;
        else if(strsize < 6) {
            printf("5 btyes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
    }
    strncpy(id, str, IDlen-1);

    while(1) { // 확인하려는 데이터 id의 pwd
        puts("\n확인하려는 id의 PWD 입력(PWD 틀리면 종료):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수 확인 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen); // DBPrintModule.c
    }
    strncpy(pwd, str, PWDlen-1);

    selAdminInfo(id, pwd/*, OUT struct InfoTable sel_it*/);

    sqlite3_close(db);

    return 0;
}

int sp() { // int searchPWD(const char search_id[IDlen], const char seearch_pwd[PWDlen]); // 비번 검색 // case 38

}

int sk() { // int selPublicKey(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT char sel_pk[PKlen]); // 공개키 검색 // case 39

}
