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

int dw() { // int delWhiteList(char del_wl[WLlen]); // 화이트리스트 삭제 // case 46
    sqlite3 *db;
    char *errmsg;
    int rc;

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char id[IDlen] = { 0, };
    char whitelist[WLlen] = { 0, };

    const char *NotUsed = "Callback Function Called";

    char sql_de_ww[SQLlen] = { 0, }; // whitelist 기준으로 화이트리스트 삭제 sql // 2번

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

    printf("Delete row!!!\n");

    printf("삭제하려는 데이터의 id와 pwd가 일치해야 삭제됩니다.\n");
    while(1) { // 삭제하려는 데이터의 id
        id[0] = '\0';
        str[0] = '\0';
        puts("삭제하려는 항목의 id(기본키) 입력:");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
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

    if(checkID(id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

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
    delWhiteList(whitelist);
    return 0;
}

int da() { // int delAdmin(const char del_id[IDlen], const char del_pwd[PWDlen]); // 관리자 삭제 // case 47
    sqlite3 *db;
    char *errmsg;
    int rc;

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열
    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };

    const char *NotUsed = "Callback Function Called";

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

    printf("Delete row!!!\n");

    printf("삭제하려는 데이터의 id와 pwd가 일치해야 삭제됩니다.\n");
    while(1) { // 삭제하려는 데이터의 id
        id[0] = '\0';
        str[0] = '\0';
        puts("삭제하려는 항목의 id(기본키) 입력:");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
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

    if(checkID(id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    while(1) { // 확인하려는 데이터 id의 pwd
        puts("\n확인하려는 id의 원래 PWD 입력(PWD 틀리면 종료):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수 확인 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen); // DBPrintModule.c
    }
    strncpy(pwd, str, PWDlen-1);

    sqlite3_close(db);

    delAdmin(id, pwd);

    return 0;
}
