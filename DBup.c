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

int ua() { // int updateAdminInfo(struct InsInfo up_a); // 관리자 정보 수정 // case 26
    struct InsInfo a;

    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, 0};
    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char I_date[DATElen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int b_date = 0; // birth 문자열을 정수형으로 받을 변수
    int i = 0;
    int pos_at = 0; // email 문자열의 @ 위치
    int pos_dot = 0; // email 문자열의 . 위치
    int p_number = 0; // phone 문자열이 숫자가 아닐 때마다 1씩 증가되는 변수

    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t); // DBManage.c

    sqlite3 *db;
    char *errmsg;
    int rc;

    char input_sql[SQLlen] = { 0, };
    char sql_tmp[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 access sql
    char sql_tn[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 name sql
    char sql_tb[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 sql
    char sql_te[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 email sql
    char sql_tp[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 phone sql

    while(1) { // 수정하려는 데이터의 id
        id[0] = '\0';
        str[0] = '\0';
        puts("\n수정하려는 데이터의 id(기본키) 입력:");
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
    strncpy(a.id, str, IDlen-1);

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

    if(checkID(a.id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(a.id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    printf("\nADMIN INFO Update\n");
    printf("\n=수정이 가능한 데이터=\n");
    printf("pwd =  수정이 가능합니다.\n");

    __fpurge(stdin);
    strncpy(id, a.id, IDlen-1);

    strncpy(input_sql, "SELECT admin.access, info.name, info.birth, info.email, info.phone ", 67);
    strncat(input_sql, "FROM ADMIN, INFO WHERE admin.id = '", 35);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "' and info.id = '", 17);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "';", 2);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    input_sql[0] = '\0';

    strncpy(sql_tmp, "select access from admin where id = '", 37);
    strncat(sql_tmp, id, IDlen-1);
    strncat(sql_tmp, "';", 2);
    //printf("%s\n", sql_tmp);
    rc = sqlite3_exec(db, sql_tmp, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sql_tmp[0] = '\0';

    while(1) { // 수정하려는 id의 access 수정
        access[0] = 'N';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("ACCESS(권한) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize <= ACCESSlen)
            break;

        printBOF_gets(str, strsize, ACCESSlen); // DBPrintModule.c
    }
    strncpy(a.access, str, ACCESSlen-1);
    //update admin set access = 'access' where access is not null and id = 'id'; // null이 아닌 access 값 수정

    while(1) { // 수정하려는 id의 pwd 수정
        pwd[0] = '\0';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("PWD(비밀번호) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen); // DBPrintModule.c
    }
    strncpy(a.pwd, str, PWDlen-1);

    printf("admin 정보 수정을 위해 id와 pwd를 재확인합니다.\n");
    if(checkID(a.id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(a.id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    printf("\n지금부터 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
    printf("기존 정보에서 수정을 원하지 않는 항목은 EnterKey만 누르세요.\n");

    strncpy(sql_tn, "select name from info where id = '", 34);
    strncat(sql_tn, id, IDlen-1);
    strncat(sql_tn, "';", 2);
    //printf("%s\n", sql_tn);
    rc = sqlite3_exec(db, sql_tn, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sql_tn[0] = '\0';

    while(1) { // 수정하려는 id의 name 수정
        name[0] = '\0';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("NAME(이름) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize >= 3 && strsize <= NAMElen)
            break;
        else if(strsize < 3) {
            printf("2bytes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, NAMElen); // DBPrintModule.c
    }
    strncpy(a.name, str, NAMElen-1);

    strncpy(a.I_date, str_now, DATElen);

    strncpy(sql_tb, "select birth from info where id = '", 35);
    strncat(sql_tb, id, IDlen-1);
    strncat(sql_tb, "';", 2);
    //printf("%s\n", sql_tb);
    rc = sqlite3_exec(db, sql_tb, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sql_tb[0] = '\0';

    while(1) { // 수정하려는 id의 생년월일(yyyymmdd 8개의 유효 날짜) 수정
        birth[0] = '\0';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("birth(생년월일 yyyymmdd 8개 날짜) 변경 값 입력:");
        gets(str);

        str[0] == '\0'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize == BIRTHlen) {
            b_date = atoi(str);
            if(checkDate(b_date) == 1) // DBManage.c : 1이면 참(유효날짜)
                 break;
            else {
                printf("유효 날짜가 아닙니다!!!\n");
                continue;
            }
        }
        else
            printf("yyyymmdd 8개의 유효 날짜(숫자)로 입력해야 합니다!\n");
    }
    strncpy(a.birth, str, BIRTHlen-1);

    strncpy(a.I_date, str_now, DATElen);

    printf("info 정보 수정을 위해 id와 pwd를 재확인합니다.\n"); // email은 고유 값이어야해서 한번 더 확인
    if(checkID(a.id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(a.id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    strncpy(sql_te, "select email from info where id = '", 35);
    strncat(sql_te, id, IDlen-1);
    strncat(sql_te, "';", 2);
    //printf("%s\n", sql_te);
    rc = sqlite3_exec(db, sql_te, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sql_te[0] = '\0';

    while(1) { // 이메일 수정 : @는 [1] ~ [끝-2] && .은 [@위치+2] ~ [끝]
        email[0] = '\0';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("email 변경 값 입력:");
        gets(str);

        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > EMAILlen) {
            printBOF_gets(str, strsize, EMAILlen); // DBPrintModule.c
            continue;
        }
        for(i = 0; i < strsize; i++) { // 입력받은 이메일의 한 글자씩 확인하는 루프
            if(i > 0 && str[i] == '@') // @는 맨 앞에 올 수 없음
                pos_at = i+1;
            if(i > pos_at && str[i] =='.') // .은 @+1 보다 뒤에 있음
                pos_dot = i+1;
        }

        if(pos_at > 0 && pos_dot > 3 && pos_dot < i) // @는 맨 앞이면 안됨 && .은 4번째~끝(i) 사이
            break;
        else {
            printf("이메일 형식 -@-.- 에 맞지 않습니다!\n");
            continue;
        }
    }
    strncpy(a.email, str, EMAILlen-1);

    strncpy(a.I_date, str_now, DATElen);

    strncpy(sql_tp, "select phone from info where id = '", 35);
    strncat(sql_tp, id, IDlen-1);
    strncat(sql_tp, "';", 2);
    //printf("%s\n", sql_tp);
    rc = sqlite3_exec(db, sql_tp, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    sql_tp[0] = '\0';

    while(1) { // 전화번호 수정
        phone[0] = '\0';
        str[0] = '\0';
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        printf("수정을 원하면 2 ~ 20개의 수를 -표시 없이 입력하세요.\n");
        puts("phone(전화번호) 변경 값 입력:");
        gets(str);

        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > PHONElen) {
            printBOF_gets(str, strsize, PHONElen); // DBPrintModule.c
            continue;
        }
        if(strsize < 3) { // 숫자 1개만 입력하면 반복
            printf("최소 2개의 숫자를 입력하세요!!!\n");
            continue;
        }

        for(p_number = 0, i = 0; i < strsize; i++) { // 입력받은 이메일의 한 글자씩 확인하는 루프
            if((str[i] < '0') || (str[i] > '9')) // 숫자가 아닌 문자가 입력된 경우 반복
                p_number++; // 숫자가 아니면 p_number 변수 값에 1을 더함
            else
                p_number = 0;
        }

        if(p_number == 1) // 전화번호가 길이제한을 넘기지 않고, 모두 숫자로만 입력된 경우는 무한루프 탈출
            break;
    }
    strncpy(a.phone, str, PHONElen-1);

    strncpy(a.I_date, str_now, DATElen);

    sqlite3_close(db);

    updateAdminInfo(a);

    return 0;
}

int um() { // int updateMac(const char upmac_id[IDlen], const char upmac_mac[MAClen]); // 맥 수정 // case 27

}
