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

int upAD_INFO() { // case 26
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };

    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char I_date[DATElen] = { 0, };

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

    const char *NotUsed = "Callback Function Called";

    char sql_tmp[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 access sql
    char sql_tn[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 name sql
    char sql_tb[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 sql
    char sql_te[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 email sql
    char sql_tp[SQLlen] = { 0, }; // 수정 항목의 값을 출력해줄 phone sql
    char sql_ua[SQLlen] = { 0, }; // ADMIN access 업데이트 sql
    char sql_up[SQLlen] = { 0, }; // ADMIN pwd 업데이트 sql
    char sql_uap[SQLlen] = { 0, }; // ADMIN access, pwd 업데이트 sql
    char sql_uin[SQLlen] = { 0, }; // INFO name 업데이트 sql
    char sql_uib[SQLlen] = { 0, }; // INFO birth 업데이트 sql
    char sql_uie[SQLlen] = { 0, }; // INFO email 업데이트 sql
    char sql_uip[SQLlen] = { 0, }; // INFO phone 업데이트 sql

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

    printf("\nADMIN INFO Update\n");

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
    strncpy(id, str, IDlen-1);

    if(checkID(id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    printf("\n=수정이 가능한 데이터=\n");
    printf("pwd =  수정이 가능합니다.\n");
    __fpurge(stdin);
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
    strncpy(access, str, ACCESSlen-1);
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
    strncpy(pwd, str, PWDlen-1);

    printf("admin 정보 수정을 위해 id와 pwd를 재확인합니다.\n");
    if(checkID(id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    if((strlen(access) != 0) && (strlen(pwd) == 0)) { // access만 수정
        __fpurge(stdin);
        strncpy(sql_ua, "UPDATE ADMIN SET access = '", 27);
        strncat(sql_ua, access, ACCESSlen-1);
        strncat(sql_ua, "' WHERE id = '", 14);
        strncat(sql_ua, id, IDlen-1);;
        strncat(sql_ua, "';", 2);
        //printf("%s\n", sql_ua);
        rc = sqlite3_exec(db, sql_ua, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (access) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "access 값을 %s로 수정 성공\n\n", access);
        }
        sql_ua[0] = '\0';
    }

    if((strlen(access) == 0) && (strlen(pwd) != 0)) { // pwd만 수정
        __fpurge(stdin);
        strncpy(sql_up, "UPDATE ADMIN SET pwd = '", 24);
        strncat(sql_up, pwd, PWDlen-1);
        strncat(sql_up, "' WHERE id = '", 14);
        strncat(sql_up, id, IDlen-1);
        strncat(sql_up, "';", 2);
        //printf("%s\n", sql_up);
        rc = sqlite3_exec(db, sql_up, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (pwd) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "pwd 값을 %s로 수정 성공\n", pwd);
            printf("지금부터 pwd 입력란에는 %s로 입력하세요.\n\n", pwd);
        }
        sql_up[0] = '\0';
    }

    if((strlen(access) != 0) && (strlen(pwd) != 0)) { // access & pwd 함께 수정
        __fpurge(stdin);
        strncpy(sql_uap, "UPDATE ADMIN SET (access, pwd) = ('", 35);
        strncat(sql_uap, access, ACCESSlen-1);
        strncat(sql_uap, "', '", 4);
        strncat(sql_uap, pwd, PWDlen-1);
        strncat(sql_uap, "') WHERE id = '", 15);
        strncat(sql_uap, id, IDlen-1);
        strncat(sql_uap, "';", 2);
        //printf("%s\n", sql_uap);
        rc = sqlite3_exec(db, sql_uap, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (access&pwd) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "access 값은 %s로, pwd 값은 %s로 수정 성공\n\n", access, pwd);
        }
        sql_uap[0] = '\0';
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
    strncpy(name, str, NAMElen-1);

    strncpy(I_date, str_now, DATElen);

    if(strlen(name) != 0) { // name 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uin, "UPDATE INFO SET (name, I_date) = ('", 35);
        strncat(sql_uin, name, NAMElen-1);
        strncat(sql_uin, "','", 3);
        strncat(sql_uin, I_date, DATElen-1);
        strncat(sql_uin, "') WHERE id = '", 15);
        strncat(sql_uin, id, IDlen-1);
        strncat(sql_uin, "';", 2);
        //printf("%s\n", sql_uin);
        rc = sqlite3_exec(db, sql_uin, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (name) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "name 값을 %s로 수정 성공\n\n", name);
        }
        sql_uin[0] = '\0';
    }

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
    strncpy(birth, str, BIRTHlen-1);

    strncpy(I_date, str_now, DATElen);

    if(strlen(birth) != 0) { // birth 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uib, "UPDATE INFO SET (birth, I_date) = ('", 36);
        strncat(sql_uib, birth, BIRTHlen-1);
        strncat(sql_uib, "','", 3);
        strncat(sql_uib, I_date, DATElen-1);
        strncat(sql_uib, "') WHERE id = '", 15);
        strncat(sql_uib, id, IDlen-1);
        strncat(sql_uib, "';", 2);
        //printf("%s\n", sql_uib);
        rc = sqlite3_exec(db, sql_uib, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (birth) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "birth 값을 %s로 수정 성공\n\n", birth);
        }
        sql_uib[0] = '\0';
    }

    printf("info 정보 수정을 위해 id와 pwd를 재확인합니다.\n"); // email은 고유 값이어야해서 한번 더 확인
    if(checkID(id) == 0) { // DBManage.c : 등록된 id가 아니면 종료
        sqlite3_close(db);
        return 0;
    }

    if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
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
    strncpy(email, str, EMAILlen-1);

    strncpy(I_date, str_now, DATElen);

    if(strlen(email) != 0) { // email 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uie, "UPDATE INFO SET (email, I_date) = ('", 36);
        strncat(sql_uie, email, EMAILlen-1);
        strncat(sql_uie, "','", 3);
        strncat(sql_uie, I_date, DATElen-1);
        strncat(sql_uie, "') WHERE id = '", 15);
        strncat(sql_uie, id, IDlen-1);
        strncat(sql_uie, "';", 2);
        //printf("%s\n", sql_uie);
        rc = sqlite3_exec(db, sql_uie, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (email) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "email 값을 %s로 수정 성공\n\n", email);
        }
        sql_uie[0] = '\0';
    }

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
    strncpy(phone, str, PHONElen-1);

    strncpy(I_date, str_now, DATElen);

    if(strlen(phone) != 0) { // phone 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uip, "UPDATE INFO SET (phone, I_date) = ('", 36);
        strncat(sql_uip, phone, PHONElen-1);
        strncat(sql_uip, "','", 3);
        strncat(sql_uip, I_date, DATElen-1);
        strncat(sql_uip, "') WHERE id = '", 15);
        strncat(sql_uip, id, IDlen-1);
        strncat(sql_uip, "';", 2);
        //printf("%s\n", sql_uip);
        rc = sqlite3_exec(db, sql_uip, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (phone) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "phone 값을 %s로 수정 성공\n", phone);
        }
        sql_uip[0] = '\0';
    }
    sqlite3_close(db);

    return 0;
}
