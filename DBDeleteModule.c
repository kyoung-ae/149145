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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) { // callback
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int delCol() { // case 46
    sqlite3 *db;
    char *errmsg;
    int rc;

    char id[IDlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char I_date[DATElen] = { 0, };

    int del_c = 0; // 삭제 분류 번호

    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t); // DBManage.c

    const char *NotUsed = "Callback Function Called";

    char sql_n[SQLlen] = { 0, }; // INFO name 출력 sql // 1번
    char sql_b[SQLlen] = { 0, }; // INFO birth 출력 sql // 2번
    char sql_p[SQLlen] = { 0, }; // INFO phone 출력 sql // 3번
    char sql_nb[SQLlen] = { 0, }; // INFO name 과 birth 출력 sql // 4번
    char sql_np[SQLlen] = { 0, }; // INFO name 과 phone 출력 sql // 5번
    char sql_bp[SQLlen] = { 0, }; // INFO birth 와 phone 출력 sql // 6번
    char sql_nbp[SQLlen] = { 0, }; // INFO name과 birth와 phone 출력 sql // 7번

    char sql_de_n[SQLlen] = { 0, }; // INFO name 삭제 sql // 1번
    char sql_de_b[SQLlen] = { 0, }; // INFO birth 삭제 sql // 2번
    char sql_de_p[SQLlen] = { 0, }; // INFO phone 삭제 sql // 3번
    char sql_de_nb[SQLlen] = { 0, }; // INFO name 과 birth 삭제 sql // 4번
    char sql_de_np[SQLlen] = { 0, }; // INFO name 과 phone삭제 sql // 5번
    char sql_de_bp[SQLlen] = { 0, }; // INFO birth 와 phone 삭제 sql // 6번
    char sql_de_nbp[SQLlen] = { 0, }; // INFO name과 birth와 phone 삭제 sql // 7번

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

    printf("Delete col!!!\n");

    while(1) { // 삭제 항목 입력받는 무한루프
        del_c = 0;
        print_del_col(); // DBPrintModule.c
        puts("input :");
        scanf("%d", &del_c);
        removeEnter(); // DBManage.c

        if(del_c == 0)
            exit(0);
        if(del_c >= 1 && del_c < 8)
            break;
        else
            continue;
    }

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

    switch(del_c) {
        case 1:
            printf("삭제하려는 ");

            __fpurge(stdin);
            strncpy(sql_n, "SELECT name FROM info where id = '", 34);
            strncat(sql_n, id, IDlen-1);
            strncat(sql_n, "';", 2);
            //printf("%s\n", sql_n);
            rc = sqlite3_exec(db, sql_n, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_n[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_n, "UPDATE INFO SET (name, I_date) = ('', '", 39);
            strncat(sql_de_n, I_date, DATElen-1);
            strncat(sql_de_n, "') WHERE id = '", 15);
            strncat(sql_de_n, id, IDlen-1);
            strncat(sql_de_n, "';", 2);
            //printf("%s\n", sql_de_n);
            rc = sqlite3_exec(db, sql_de_n, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (name) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (name)\n");
            }
            sql_de_n[0] = '\0';
            break;

        case 2:
            printf("삭제하려는 ");

            __fpurge(stdin);
            strncpy(sql_b, "SELECT birth FROM info where id = '", 35);
            strncat(sql_b, id, IDlen-1);
            strncat(sql_b, "';", 2);
            //printf("%s\n", sql_b);
            rc = sqlite3_exec(db, sql_b, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_b[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_b, "UPDATE INFO SET (birth, I_date) = ('', '", 40);
            strncat(sql_de_b, I_date, DATElen-1);
            strncat(sql_de_b, "') WHERE id = '", 15);
            strncat(sql_de_b, id, IDlen-1);
            strncat(sql_de_b, "';", 2);
            //printf("%s\n", sql_de_b);
            rc = sqlite3_exec(db, sql_de_b, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (birth) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (birth)\n");
            }
            sql_de_b[0] = '\0';
            break;

        case 3:
            printf("삭제하려는 ");

            __fpurge(stdin);
            strncpy(sql_p, "SELECT phone FROM info where id = '", 35);
            strncat(sql_p, id, IDlen-1);
            strncat(sql_p, "';", 2);
            //printf("%s\n", sql_p);
            rc = sqlite3_exec(db, sql_p, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_p[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_p, "UPDATE INFO SET (phone, I_date) = ('', '", 40);
            strncat(sql_de_p, I_date, DATElen-1);
            strncat(sql_de_p, "') WHERE id = '", 15);
            strncat(sql_de_p, id, IDlen-1);
            strncat(sql_de_p, "';", 2);
            //printf("%s\n", sql_de_p);
            rc = sqlite3_exec(db, sql_de_p, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (phone) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (phone)\n");
            }
            sql_de_p[0] = '\0';
            break;

        case 4:
            printf("삭제하려는 name과 birth 값\n");

            __fpurge(stdin);
            strncpy(sql_nb, "SELECT name, birth FROM info where id = '", 41);
            strncat(sql_nb, id, IDlen-1);
            strncat(sql_nb, "';", 2);
            //printf("%s\n", sql_nb);
            rc = sqlite3_exec(db, sql_nb, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_nb[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_nb, "UPDATE INFO SET (name, birth, I_date) = ('','', '", 49);
            strncat(sql_de_nb, I_date, DATElen-1);
            strncat(sql_de_nb, "') WHERE id = '", 15);
            strncat(sql_de_nb, id, IDlen-1);
            strncat(sql_de_nb, "';", 2);
            //printf("%s\n", sql_de_nb);
            rc = sqlite3_exec(db, sql_de_nb, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (name, birth) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (name, birth)\n");
            }
            sql_de_nb[0] = '\0';
            break;

        case 5:
            printf("삭제하려는 name과 phone 값\n");

            __fpurge(stdin);
            strncpy(sql_np, "SELECT name, phone FROM info where id = '", 41);
            strncat(sql_np, id, IDlen-1);
            strncat(sql_np, "';", 2);
            //printf("%s\n", sql_np);
            rc = sqlite3_exec(db, sql_np, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_np[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_np, "UPDATE INFO SET (name, phone, I_date) = ('','', '", 49);
            strncat(sql_de_np, I_date, DATElen-1);
            strncat(sql_de_np, "') WHERE id = '", 15);
            strncat(sql_de_np, id, IDlen-1);
            strncat(sql_de_np, "';", 2);
            //printf("%s\n", sql_de_np);
            rc = sqlite3_exec(db, sql_de_np, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (name, phone) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (name, phone)\n");
            }
            sql_de_np[0] = '\0';
            break;

        case 6:
            printf("삭제하려는 birth와 phone 값\n");

            __fpurge(stdin);
            strncpy(sql_bp, "SELECT birth, phone FROM info where id = '", 42);
            strncat(sql_bp, id, IDlen-1);
            strncat(sql_bp, "';", 2);
            //printf("%s\n", sql_bp);
            rc = sqlite3_exec(db, sql_bp, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_bp[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_bp, "UPDATE INFO SET (birth, phone, I_date) = ('','', '", 50);
            strncat(sql_de_bp, I_date, DATElen-1);
            strncat(sql_de_bp, "') WHERE id = '", 15);
            strncat(sql_de_bp, id, IDlen-1);
            strncat(sql_de_bp, "';", 2);
            //printf("%s\n", sql_de_bp);
            rc = sqlite3_exec(db, sql_de_bp, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (birth, phone) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (birth, phone)\n");
            }
            sql_de_bp[0] = '\0';
            break;

        case 7:
            printf("삭제하려는 name과 birth와 phone 값\n");

            __fpurge(stdin);
            strncpy(sql_nbp, "SELECT name, birth, phone FROM info where id = '", 48);
            strncat(sql_nbp, id, IDlen-1);
            strncat(sql_nbp, "';", 2);
            //printf("%s\n", sql_nbp);
            rc = sqlite3_exec(db, sql_nbp, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_nbp[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(I_date, str_now, DATElen);
            strncpy(sql_de_nbp, "UPDATE INFO SET (name, birth, phone, I_date) = ('','','', '", 59);
            strncat(sql_de_nbp, I_date, DATElen-1);
            strncat(sql_de_nbp, "') WHERE id = '", 15);
            strncat(sql_de_nbp, id, IDlen-1);
            strncat(sql_de_nbp, "';", 2);
            //printf("%s\n", sql_de_nbp);
            rc = sqlite3_exec(db, sql_de_nbp, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (name, birth, phone) : %s\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Delete success (name, birth, phone)\n");
            }
            sql_de_nbp[0] = '\0';
            break;

        default:
            break;
    }
    sqlite3_close(db);

    return 0;
}

int delRow() { // case 47
    sqlite3 *db;
    char *errmsg;
    int rc;

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char id[IDlen] = { 0, };
    char _whitelist[WLlen] = { 0, };
    char whitelist[WLlen] = { 0, };

    int del_r = 0; // 삭제 분류 번호

    const char *NotUsed = "Callback Function Called";

    char sql_iw[SQLlen] = { 0, }; // id 기준으로 화이트리스트 출력 sql // 1번
    char sql_ii[SQLlen] = { 0, }; // id 기준으로 관리자 정보 출력 sql // 1번
    char sql_wi[SQLlen] = { 0, }; // whitelist 기준으로 관리자 정보 출력 sql // 2번
    char sql_ww[SQLlen] = { 0, }; // whitelist 기준으로 화이트리스트 출력 sql // 2번

    char sql_like[SQLlen] = { 0, }; // 입력한 whitelist의 like 검색

    char sql_de_iw[SQLlen] = { 0, }; // id 기준으로 화이트리스트 삭제 sql // 1번
    char sql_de_ia[SQLlen] = { 0, }; // id 기준으로 관리자 삭제 sql // 1번
    char sql_de_ii[SQLlen] = { 0, }; // id 기준으로 info 삭제 sql // 1번
    char sql_de_ww[SQLlen] = { 0, }; // whitelist 기준으로 화이트리스트 삭제 sql // 2번
    char sql_de_wa[SQLlen] = { 0, }; // whitelist 기준으로 관리자 정보 삭제 sql // 2번
    char sql_de_wi[SQLlen] = { 0, }; // whitelist 기준으로 info 삭제 sql // 2번

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

    while(1) { // 삭제 항목 입력받는 무한루프
        del_r = 0;
        print_del_row(); // DBPrintModule.c
        puts("input :");
        scanf("%d", &del_r);
        removeEnter(); // DBManage.c

        if(del_r == 0)
            exit(0);
        if(del_r == 1 || del_r == 2)
            break;
        else
            continue;
    }

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

    switch(del_r) {
        case 1: // id 기준으로 삭제
            printf("\n삭제하려는 데이터\n");

            __fpurge(stdin);
            strncpy(sql_iw, "SELECT id, whitelist, W_date from whitelist where id = '", 56);
            strncat(sql_iw, id, IDlen-1);
            strncat(sql_iw, "';", 2);
            //printf("%s\n", sql_iw);
            rc = sqlite3_exec(db, sql_iw, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : (whitelist) %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_iw[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_ii, "SELECT admin.access, info.name, info.birth, info.email, info.phone, info.I_date", 79);
            strncat(sql_ii, " from admin, info where admin.id = '", 36);
            strncat(sql_ii, id, IDlen);
            strncat(sql_ii, "' and info.id = '", 19);
            strncat(sql_ii, id, IDlen);
            strncat(sql_ii, "';", 2);
            //printf("%s\n", sql_ii);
            rc = sqlite3_exec(db, sql_ii, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : (admin info) %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_ii[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(sql_de_iw, "DELETE from whitelist where id = '", 34);
            strncat(sql_de_iw, id, IDlen-1);
            strncat(sql_de_iw, "';", 2);
            //printf("%s\n", sql_de_iw);
            rc = sqlite3_exec(db, sql_de_iw, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (whitelist.id:%s) : %s\n", id, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (whitelist.id:%s)\n", id);
            sql_de_iw[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_de_ia, "DELETE from admin where id = '", 30);
            strncat(sql_de_ia, id, IDlen-1);
            strncat(sql_de_ia, "';", 2);
            //printf("%s\n", sql_de_ia);
            rc = sqlite3_exec(db, sql_de_ia, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (admin.id:%s) : %s\n", id, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (admin.id:%s)\n", id);
            sql_de_ia[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_de_ii, "DELETE from info where id = '", 29);
            strncat(sql_de_ii, id, IDlen-1);
            strncat(sql_de_ii, "';", 2);
            //printf("%s\n", sql_de_ii);
            rc = sqlite3_exec(db, sql_de_ii, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (info.id:%s) : %s\n", id, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (info.id:%s)\n", id);
            sql_de_ii[0] = '\0';
            break;

        case 2: // whitelist 기준으로 삭제
            while(1) { // whitelist를 기준으로 먼저 부분 검색
                _whitelist[0] = '\0';
                str[0] = '\0';
                puts("삭제를 원하는 Whitelist를 입력하세요. 먼저 부분적으로 확인합니다.");
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
                fprintf(stderr, "Search success : whitelist like\n");
                printf("위의 결과에서 whitelist 명령어를 찾아 정확하게 입력하세요.\n");
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

            printf("\n삭제하려는 데이터\n");

            __fpurge(stdin);
            strncpy(sql_ww, "SELECT id, whitelist, W_date from whitelist where whitelist = '", 63);
            strncat(sql_ww, whitelist, WLlen-1);
            strncat(sql_ww, "';", 2);
            //printf("%s\n", sql_ww);
            rc = sqlite3_exec(db, sql_ww, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : (whitelist) %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_ww[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_wi, "SELECT admin.id, admin.access, info.name, info.birth, info.email, info.phone, info.I_date", 89);
            strncat(sql_wi, " from admin, info where admin.id = (select id from whitelist where whitelist = '", 80);
            strncat(sql_wi, whitelist, WLlen-1);
            strncat(sql_wi, "') and info.id = (select id from whitelist where whitelist = '", 62);
            strncat(sql_wi, whitelist, WLlen-1);
            strncat(sql_wi, "');", 3);
            //printf("%s\n", sql_wi);
            rc = sqlite3_exec(db, sql_wi, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't print : (admin info) %s\n", sqlite3_errmsg(db));
                return 1;
            }
            sql_wi[0] = '\0';

            while(1) { // 삭제 진행 여부 확인 루프
                str[0] = '\0';
                puts("0 : 취소(종료)");
                puts("1 : 삭제");
                printf("input : ");
                gets(str);

                if(str[0] == '1')
                    break;
                if(str[0] == '0') {
                    sqlite3_close(db);
                    printf("취소-> 종료됩니다.\n");
                    return 0;
                }
                if(str[0] == '\n' || str[0] == '\0') // 필수 입력 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, IDlen); // DBPrintModule.c
                    continue;
                }
            }

            printf("삭제 진행을 위해 pwd를 재확인합니다.\n");
            if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
                sqlite3_close(db);
                return 0;
            }

            __fpurge(stdin);
            strncpy(sql_de_ww, "DELETE from whitelist where whitelist = '", 41);
            strncat(sql_de_ww, whitelist, WLlen-1);
            strncat(sql_de_ww, "';", 2);
            //printf("%s\n", sql_de_ww);
            rc = sqlite3_exec(db, sql_de_ww, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (whitelist.whitelist:%s) : %s\n", whitelist, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (whitelist.whitelist:%s)\n", whitelist);
            sql_de_ww[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_de_wa, "DELETE from admin where id = (select id from whitelist where whitelist = '", 74);
            strncat(sql_de_wa, whitelist, WLlen-1);
            strncat(sql_de_wa, "');", 3);
            //printf("%s\n", sql_de_wa);
            rc = sqlite3_exec(db, sql_de_wa, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (whitelist-id-admin:%s) : %s\n", whitelist, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (whitelist-id-admin:%s)\n", whitelist);
            sql_de_wa[0] = '\0';

            __fpurge(stdin);
            strncpy(sql_de_wi, "DELETE from info where id = (select id from whitelist where whitelist = '", 73);
            strncat(sql_de_wi, whitelist, WLlen-1);
            strncat(sql_de_wi,  "');", 3);
            //printf("%s\n", sql_de_wi);
            rc = sqlite3_exec(db, sql_de_wi, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete (whitelist-id-info:%s) : %s\n", whitelist, sqlite3_errmsg(db));
                return 1;
            }
            else
                fprintf(stderr, "Delete success (whitelist-id-info:%s)\n", whitelist);
            sql_de_wi[0] = '\0';
            break;

        default:
            break;
    }
    sqlite3_close(db);

    return 0;
}
