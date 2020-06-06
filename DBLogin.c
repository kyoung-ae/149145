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

int login() { // DB 로그인 : id & pwd로 확인
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;
    char input_sql_ec[SQLlen] = { 0, };
    const char *NotUsed = "Callback Function Called";

    char str[MAX] = { 0, };
    char str_login[ACCESSlen] = { 0, };
    int strsize = 0;

    char id[IDlen] = { 0, };

    while(1) { // 접속 메뉴 선택
        str[0] = '\0';
        print_login(); // DBPrintModule.c
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수 선택 정보는 null 불가
            continue;
        if(str[0] == '0')  // 0을 선택하면 종료됨
            exit(0);

        //removeSpace(str_col); // DBManage.c : 공백이 입력된 경우는 제거
        strsize = strlen(str)+1;

        if(strsize > ACCESSlen) { // 길이 제한을 초과한 경우는 반복
            printBOF_gets(str, strsize, ACCESSlen); // DBPrintModule.c
            continue;
        }

        if(str[0] == '1' || str[0] == '2' || str[0] == '3')
            break;
        printf("잘못 입력했습니다.\n");
    }
    strncpy(str_login, str, ACCESSlen-1);

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 0;
    }
   	else {
        //fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    if(str_login[0] == '1') {
        while(1) { // id 검색
            id[0] = '\0';
            str[0] = '\0';
            puts("\nLogin id (9bytes 보다 길면 다시 입력함):");
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

        if(checkID(id) == 0) { // DBManage.c : 사용자가 입력한 id가 존재하지 않으면 종료
            sqlite3_close(db);
            return 0;
        }
        if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
            sqlite3_close(db);
            return 0;
        }
        return 1; // 1이면 createDB()로 이동함.
    }

    if(str_login[0] == '2') {
        inAD_INFO(); // DBInsertModule.c
    }

    if(str_login[0] == '3') {
        findIP();  // DBBackupR.c // case 56 : id, pwd 분실 시 지원 기능
    }
    sqlite3_close(db);

    return 1;
}
