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

int insWhiteListTable(struct WhiteListTable ins_wt) { // 화이트리스트 등록 // case 16
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char input_sql_iw[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    char W_date[DATElen] = { 0, };

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 0;
    }
   	else {
        fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    puts("WHITELIST TABLE's insert\n");

    strncpy(whitelist, ins_wt.whitelist, WLlen-1);
    strncpy(id, ins_wt.id, IDlen-1);
    strncpy(W_date, ins_wt.W_date, DATElen-1);

    __fpurge(stdin);
    strncpy(input_sql_iw, "insert into WHITELIST(whitelist, id, W_date) values('", 53);
    strncat(input_sql_iw, whitelist, WLlen-1);
    strncat(input_sql_iw, "','", 3);
    strncat(input_sql_iw, id, IDlen-1);
    strncat(input_sql_iw, "','", 3);
    strncat(input_sql_iw, W_date, DATElen-1);
    strncat(input_sql_iw, "');", 3);
    //printf("%s\n", input_sql_iw);
    rc = sqlite3_exec(db, input_sql_iw, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input : %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else {
        fprintf(stderr, "Print input successfully\n");
    }
    input_sql_iw[0] = '\0';
    sqlite3_close(db);

    return 1;
}

int insAdminInfo(struct InsInfo ins_ai) { // 관라자 정보 등록 // case 17
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql_ia[SQLlen] = { 0, }; // admin 테이블
    char input_sql_ii[SQLlen] = { 0, }; // info 테이블
    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, 0};
    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char I_date[DATElen] = { 0, };

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 0;
    }
   	else {
        fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    puts("\nADMIN INFO insert");

    strncpy(id, ins_ai.id, IDlen-1);
    strncpy(access, ins_ai.access, ACCESSlen-1);
    strncpy(pwd, ins_ai.pwd, PWDlen-1);

    __fpurge(stdin);
    strncpy(input_sql_ia, "insert into ADMIN(id, access, pwd) values('", 43);
    strncat(input_sql_ia, id, IDlen-1);
    strncat(input_sql_ia, "','", 3);
    strncat(input_sql_ia, access, ACCESSlen-1);
    strncat(input_sql_ia, "','", 3);
    strncat(input_sql_ia, pwd, PWDlen-1);
    strncat(input_sql_ia, "');", 3);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql_ia, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input id, access, pwd : %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else {
        fprintf(stderr, "Insert success (id, access, pwd)\n");
    }
    input_sql_ia[0] = '\0';

    strncpy(id, ins_ai.id, IDlen-1);
    strncpy(name, ins_ai.name, NAMElen-1);
    strncpy(birth, ins_ai.birth, BIRTHlen-1);
    strncpy(email, ins_ai.email, EMAILlen-1);
    strncpy(phone, ins_ai.phone, PHONElen-1);
    strncpy(I_date, ins_ai.I_date, DATElen-1);

    __fpurge(stdin);
    strncpy(input_sql_ii, "insert into INFO(id, name, birth, email, phone, I_date) values('", 64);
    strncat(input_sql_ii, id, IDlen-1);
    strncat(input_sql_ii, "','", 3);
    strncat(input_sql_ii, name, NAMElen-1);
    strncat(input_sql_ii, "','", 3);
    strncat(input_sql_ii, birth, BIRTHlen-1);
    strncat(input_sql_ii, "','", 3);
    strncat(input_sql_ii, email, EMAILlen-1);
    strncat(input_sql_ii, "','", 3);
    strncat(input_sql_ii, phone, PHONElen-1);
    strncat(input_sql_ii, "','", 3);
    strncat(input_sql_ii, I_date, DATElen-1);
    strncat(input_sql_ii, "');", 3);
    //printf("%s\n", input_sql_ii);
    rc = sqlite3_exec(db, input_sql_ii, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input id, name, birth, email, phone, I_date: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else {
        fprintf(stderr, "Insert success (id, name, birth, email, phone, I_date)\n");
    }
    input_sql_ii[0] = '\0';
    sqlite3_close(db);

    return 1;
}

int insMAC(const char insmac_id[IDlen], const char isnmac_mac[MAClen]) { // 맥 등록 // case 18
    char id[IDlen];
    char mac[MAClen];
}

int updateAdminInfo(struct InsInfo up_a) { // 관리자 정보 수정 // case 26
    sqlite3 *db;
    char *errmsg;
    int rc;

    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, 0};
    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char I_date[DATElen] = { 0, };

    const char *NotUsed = "Callback Function Called";

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

    strncpy(access, up_a.access, ACCESSlen-1);
    strncpy(pwd, up_a.pwd, PWDlen-1);
    strncpy(name, up_a.name, NAMElen-1);
    strncpy(birth, up_a.birth, BIRTHlen-1);
    strncpy(email, up_a.email, EMAILlen-1);
    strncpy(phone, up_a.phone, PHONElen-1);
    strncpy(I_date, up_a.I_date, DATElen-1);

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

    return 1;
}

int updateMac(const char upmac_id[IDlen], const char upmac_mac[MAClen]) { // 맥 수정 // case 27
    char id[IDlen];
    char mac[MAClen];
}

int selWhitelist(/*const*/ char whitelist[]/*, OUT struct WhiteListTable sel_wt*/) { // 화이트리스트 검색 // case 36
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;

    char sql_s_w0[SQLlen] = { 0, }; // 입력한 whitelist를 등록한 id의 모든 항목이 나오게 검색
    char sql_s_w1[SQLlen] = { 0, }; // admin.access 검색
    char sql_s_w2[SQLlen] = { 0, }; // info.name&birth&email&phone&I_date 검색
    char sql_s_w3[SQLlen] = { 0, }; // whitelist&id&W_date 검색

    char id[IDlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열 (버퍼오버플로 방지를 위해 1024로 지정)

    char str_col[ACCESSlen] = { 0, }; // 사용자가 입력한 문자열(항목 번호)

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

    printf("\n입력한 whitelist : %s를 기준으로 검색결과를 출력합니다.\n", whitelist);
    printf("\n검색 결과 항목은 pwd를 제외하고 모든 항목이 출력됩니다.\n");
    printf("모든 항목 출력이 아닌, 원하는 항목을 지정하려면 해당 번호를 누르세요.\n\n");

    while(1) { // 검색 결과 항목 선택
        str[0] = '\0';
        print_search_w(); // DBPrintModule.c
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // null 이면 모든 항목을 출력함
            break;
        if(str[0] == '0')  // 0을 선택하면 종료됨
            exit(0);

        //removeSpace(str_col); // DBManage.c : 공백이 입력된 경우는 제거
        strsize = strlen(str)+1;

        if(strsize > ACCESSlen) { // 길이 제한을 초과한 경우는 반복
            printBOF_gets(str, strsize, ACCESSlen); // DBPrintModule.c
            continue;
        }

        if(str[0] == '1' || str[0] == '2')
            break;
        printf("잘못 입력했습니다.\n");
    }
    strncpy(str_col, str, ACCESSlen-1);
    printf("\n");
    printf("<whitelist : %s 를 기준으로 검색한 결과>\n\n", whitelist);

    if(str_col[0] == '\0') { // null(EnterKey) 인 경우는 전부 출력되야 하므로 0으로 지정
        __fpurge(stdin);
        strncpy(sql_s_w0, "select whitelist.id, whitelist.W_date, admin.access, ", 53);
        strncat(sql_s_w0, "info.name, info.birth, info.email, info.phone, info.I_date", 58);
        strncat(sql_s_w0, " from admin, info, whitelist where whitelist.whitelist = '", 58);
        strncat(sql_s_w0, whitelist, WLlen-1);
        strncat(sql_s_w0, "' and (select id from whitelist where whitelist = '", 51);
        strncat(sql_s_w0, whitelist, WLlen-1);
        strncat(sql_s_w0, "') = whitelist.id and (select id from whitelist where whitelist = '", 87);
        strncat(sql_s_w0, whitelist, WLlen-1);
        strncat(sql_s_w0, "') = admin.id and (select id from whitelist where whitelist = '", 63);
        strncat(sql_s_w0, whitelist, WLlen-1);
        strncat(sql_s_w0, "') = info.id;", 13);
        //printf("%s\n", sql_s_w0);
        rc = sqlite3_exec(db, sql_s_w0, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
            sql_s_w0[0] = '\0';
            return 1;
        }
        else {
            fprintf(stderr, "\nSearch success : all col\n");
            printf("원하는 결과가 안 나왔다면\n");
            printf("Admin 테이블에 방금 입력한 whitelist의 id로 등록되지 않았기 때문입니다.\n");
            printf("Admin 테이블에 방금 whitelist의 id로 등록을 하거나,\n");
            printf("검색 항목을 선택할 때, 1 로 선택하세요.\n");
        }
        sql_s_w0[0] = '\0';
    }

    if(str_col[0] == '1') { // 관리자 정보 검색
        __fpurge(stdin); // admin.access
        strncpy(sql_s_w1, "SELECT id, access FROM admin WHERE id = ", 40);
        strncat(sql_s_w1, "(select id from whitelist where whitelist = '", 45);
        strncat(sql_s_w1, whitelist, WLlen-1);
        strncat(sql_s_w1, "');", 3);
        rc = sqlite3_exec(db, sql_s_w1, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (1.admin.access)\n", sqlite3_errmsg(db));
            return 1;
        }
        sql_s_w1[0] = '\0';

        __fpurge(stdin); // info.name & birth & email & phone & I_date
        strncpy(sql_s_w2, "SELECT name, birth, email, phone, I_date FROM info WHERE id = ", 72);
        strncat(sql_s_w2, "(select id from whitelist where whitelist = '", 45);
        strncat(sql_s_w2, whitelist, WLlen-1);
        strncat(sql_s_w2, "');", 3);
        rc = sqlite3_exec(db, sql_s_w2, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (1.info.name,birth,email,phone,I_date)\n", sqlite3_errmsg(db));
            return 1;
        }
        sql_s_w2[0] = '\0';
    }

    if(str_col[0] == '2') { // 화이트리스트 테이블 검색
        __fpurge(stdin);  // whitelist.id & whitelist & W_date
        strncpy(sql_s_w3, "SELECT whitelist, id, W_date FROM whitelist WHERE whitelist = '", 63);
        strncat(sql_s_w3, whitelist, WLlen-1);
        strncat(sql_s_w3, "';", 2);
        rc = sqlite3_exec(db, sql_s_w3, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (2.whitelist.whitelist & id & W_date)\n", sqlite3_errmsg(db));
            return 1;
        }
        sql_s_w3[0] = '\0';
    }
    sqlite3_close(db);

    return 1;
}

int selAdminInfo(const char id[], const char pwd[]/*, OUT struct InfoTable sel_it*/) { // 관리자 정보 검색 // case 37
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열 (버퍼오버플로 방지를 위해 1024로 지정)

    char str_col[ACCESSlen] = { 0, }; // 사용자가 입력한 문자열(항목 번호)

    int sort = 0; // whitelist 정렬 기준 값
    char sql_s_i0[SQLlen] = { 0, }; // 모든 항목이 나오게 검색
    char sql_s_i1[SQLlen] = { 0, }; // whitelist&W_date 검색
    char sql_s_i2[SQLlen] = { 0, }; // admin.access 검색
    char sql_s_i3[SQLlen] = { 0, }; // info.name&birth&email&phone&I_date 검색
    
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

    printf("\n입력한 id : %s를 기준으로 검색결과를 출력합니다.\n", id);
    printf("\n검색 결과 항목은 pwd를 제외하고 모든 항목이 출력됩니다.\n");
    printf("모든 항목 출력이 아닌, 원하는 항목을 지정하려면 해당 번호를 누르세요.\n");

    printf("정보 확인을 위해 id와 pwd를 재확인합니다.\n");

    while(1) { // 검색 결과 항목 선택
        str[0] = '\0';
        print_search_i(); // DBPrintModule.c
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // null 이면 모든 항목을 출력함
            break;
        if(str[0] == '0')  // 0을 선택하면 종료됨
            exit(0);

        //removeSpace(str_col); // DBManage.c : 공백이 입력된 경우는 제거
        strsize = strlen(str)+1;

        if(strsize > ACCESSlen) { // 길이 제한을 초과한 경우는 반복
            printBOF_gets(str, strsize, ACCESSlen); // DBPrintModule.c
            continue;
        }

        if(str[0] == '1' || str[0] == '2')
            break; //break 탈출이 안 됨 (세그 폴트 ㅠㅠ 단일 소스로는 이상없이 돌아감...이건..무슨 경우?)
            // 소스 수정없이 업데이트와 업그레이드 진행 후에 해결됨...(세그 폴트 발생 이유도, 해결된 이유도 여전히 모르겠음)
            // 소스 안 건드렸는데 또 세그폴트 됨...ㅠㅠ
            // break로 탈출이 안 되는 줄 알았는데, 탈출 후에 입력값을 받는 부분에서 세그폴트가 발생함을 확인함.
            //  break 부분의 세그폴트 원인 (break로 탈출 후에 다음 실행 소스가 세그폴트였음)파악되어 해결됨.
        printf("잘못 입력했습니다.\n");
    }
    strncpy(str_col, str, ACCESSlen-1);

    if(str_col[0] == '\0') { // null(EnterKey) 인 경우는 전부 출력되야 하므로 0으로 지정
        while(1) {
            sort = 0;
            print_search_sort();
            puts("select number:");
            scanf("%d", &sort);
            removeEnter();
            if(sort >= 1 && sort <=4)
                break;
            printf("1~4까지의 숫자를 입력하세요!\n");
        }

        printf("\n<id : %s 를 기준으로 검색한 결과>\n\n", id);

        switch(sort) { // whitelist 정렬 기준
            case 1: // whitelist 오름차순 검색
                __fpurge(stdin);
                strncpy(sql_s_i0, "select whitelist.whitelist, whitelist.W_date, admin.id, admin.access,", 69);
                strncat(sql_s_i0, " info.name, info.birth, info.email, info.phone, info.I_date ", 60);
                strncat(sql_s_i0, "from admin, info, whitelist where admin.id = '", 46);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and info.id = '", 17);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and whitelist.id = '", 22);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' order by whitelist asc;", 25);
                rc = sqlite3_exec(db, sql_s_i0, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
                    return 1;
                }
                else {
                    fprintf(stderr, "whitelist 오름차순 검색 : all col\n");
                    printf("원하는 결과가 안 나왔다면\n");
                    printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
                    printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
                    printf("검색 항목을 선택할 때, 2 로 선택하세요.\n");
                }
                sql_s_i0[0] = '\0';
                break;

            case 2: // whitelist 내림차순 검색
                __fpurge(stdin);
                strncpy(sql_s_i0, "select whitelist.whitelist, whitelist.W_date, admin.id, admin.access,", 69);
                strncat(sql_s_i0, " info.name, info.birth, info.email, info.phone, info.I_date ", 60);
                strncat(sql_s_i0, "from admin, info, whitelist where admin.id = '", 46);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and info.id = '", 17);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and whitelist.id = '", 22);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' order by whitelist desc;", 26);
                rc = sqlite3_exec(db, sql_s_i0, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
                    return 1;
                }
                else {
                    fprintf(stderr, "whitelist 내림차순 검색 : all col\n");
                    printf("원하는 결과가 안 나왔다면\n");
                    printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
                    printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
                    printf("검색 항목을 선택할 때, 2 로 선택하세요.\n");
                }
                sql_s_i0[0] = '\0';
                break;

            case 3: // W_date whitelist를 먼저 등록한 순서로 정렬
                __fpurge(stdin);
                strncpy(sql_s_i0, "select whitelist.W_date, whitelist.whitelist, admin.id, admin.access,", 69);
                strncat(sql_s_i0, " info.name, info.birth, info.email, info.phone, info.I_date ", 60);
                strncat(sql_s_i0, "from admin, info, whitelist where admin.id = '", 46);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and info.id = '", 17);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and whitelist.id = '", 22);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' order by W_date desc;", 23);
                rc = sqlite3_exec(db, sql_s_i0, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
                    return 1;
                }
                else {
                    fprintf(stderr, "W_date 먼저 등록한 순서로 검색 : all col\n");
                    printf("원하는 결과가 안 나왔다면\n");
                    printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
                    printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
                    printf("검색 항목을 선택할 때, 2 로 선택하세요.\n");
                }
                sql_s_i0[0] = '\0';
                break;

            case 4: // W_date whitelist를 최근에 등록한 순서로 정렬
                __fpurge(stdin);
                strncpy(sql_s_i0, "select whitelist.W_date, whitelist.whitelist, admin.id, admin.access,", 69);
                strncat(sql_s_i0, " info.name, info.birth, info.email, info.phone, info.I_date ", 60);
                strncat(sql_s_i0, "from admin, info, whitelist where admin.id = '", 46);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and info.id = '", 17);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' and whitelist.id = '", 22);
                strncat(sql_s_i0, id, IDlen-1);
                strncat(sql_s_i0, "' order by W_date asc;", 22);
                rc = sqlite3_exec(db, sql_s_i0, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
                    return 1;
                }
                else {
                    fprintf(stderr, "W_date 최근에 등록한 순서로 검색 : all col\n");
                    printf("원하는 결과가 안 나왔다면\n");
                    printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
                    printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
                    printf("검색 항목을 선택할 때, 2 로 선택하세요.\n");
                }
                sql_s_i0[0] = '\0';
                break;

            default:
                break;
        }
    }

    if(str_col[0] == '1') { // 화이트리스트 테이블 검색
        while(1) {
            sort = 0;
            print_search_sort();
            puts("select number:");
            scanf("%d", &sort);
            removeEnter();
            if(sort >= 1 && sort <=4)
                break;
            printf("1~4까지의 숫자를 입력하세요!\n");
        }

        printf("\n<id : %s 를 기준으로 검색한 결과>\n\n", id);

        switch(sort) { // whitelist 정렬 기준
            case 1: // whitelist 오름차순 검색
                __fpurge(stdin);
                strncpy(sql_s_i1, "SELECT whitelist, W_date FROM whitelist WHERE id = '", 62);
                strncat(sql_s_i1, id, IDlen-1);
                strncat(sql_s_i1, "' order by whitelist asc;", 25);
                rc = sqlite3_exec(db, sql_s_i1, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (1.whitelist & W_date)\n", sqlite3_errmsg(db));
                    return 1;
                }
                sql_s_i1[0] = '\0';
                break;
            case 2: // whitelist 내림차순 검색
                __fpurge(stdin);
                strncpy(sql_s_i1, "SELECT whitelist, W_date FROM whitelist WHERE id = '", 62);
                strncat(sql_s_i1, id, IDlen-1);
                strncat(sql_s_i1, "' order by whitelist desc;", 26);
                rc = sqlite3_exec(db, sql_s_i1, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (1.whitelist & W_date)\n", sqlite3_errmsg(db));
                    return 1;
                }
                sql_s_i1[0] = '\0';
                break;

            case 3: // W_date whitelist를 먼저 등록한 순서로 정렬
                __fpurge(stdin);
                strncpy(sql_s_i1, "SELECT  W_date, whitelist FROM whitelist WHERE id = '", 62);
                strncat(sql_s_i1, id, IDlen-1);
                strncat(sql_s_i1, "' order by W_date desc;", 23);
                rc = sqlite3_exec(db, sql_s_i1, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (1.whitelist & W_date)\n", sqlite3_errmsg(db));
                    return 1;
                }
                sql_s_i1[0] = '\0';
                break;

            case 4: // W_date whitelist를 최근에 등록한 순서로 정렬
                __fpurge(stdin);
                strncpy(sql_s_i1, "SELECT W_date, whitelist FROM whitelist WHERE id = '", 62);
                strncat(sql_s_i1, id, IDlen-1);
                strncat(sql_s_i1, "' order by W_date asc;", 22);
                rc = sqlite3_exec(db, sql_s_i1, callback, 0, &errmsg);
                if(rc != SQLITE_OK) {
                    fprintf(stderr, "Can't Search : %s (1.whitelist & W_date)\n", sqlite3_errmsg(db));
                    return 1;
                }
                sql_s_i1[0] = '\0';
                break;

            default:
                break;
        }
    }

    if(str_col[0] == '2') { // 관리자 정보 검색
        __fpurge(stdin); // admin.access
        strncpy(sql_s_i2, "SELECT access FROM admin WHERE id = '", 37);
        strncat(sql_s_i2, id, IDlen-1);
        strncat(sql_s_i2, "';", 2);
        //printf("%s\n", sql_s_i2);
        rc = sqlite3_exec(db, sql_s_i2, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (2.admin.access)\n", sqlite3_errmsg(db));
            return 1;
        }
        sql_s_i2[0] = '\0';

        __fpurge(stdin); // info.name & birth & email & phone & I_date
        strncpy(sql_s_i3, "SELECT name, birth, email, phone, I_date FROM info WHERE id = '", 63);
        strncat(sql_s_i3, id, IDlen-1);
        strncat(sql_s_i3, "';", 2);
        rc = sqlite3_exec(db, sql_s_i3, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (2.info.name,birth,email,phone,I_date)\n", sqlite3_errmsg(db));
            return 1;
        }
        sql_s_i3[0] = '\0';
    }
}

int searchPWD(const char id[], const char pwd[]) { // 비번 검색 // case 38
    sqlite3 *db;
    sqlite3_stmt *res;
    //char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 0;
    }
   	else {
        fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정
    __fpurge(stdin);
    strncpy(input_sql, "SELECT id, pwd FROM ADMIN WHERE id = '", 38);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "' AND pwd = '", 13);
    strncat(input_sql, pwd, PWDlen-1);
    strncat(input_sql, "';", 2);
    /*
    strncpy(input_sql, "SELECT CASE WHEN id=='", 22);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "' AND pwd=='", 12);
    strncat(input_sql, pwd, PWDlen-1);
    strncat(input_sql, "' THEN 'Success' ELSE 'Failure' END FROM ADMIN;", 47);
    /*
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print Admin Table : %s\n", sqlite3_errmsg(db));
        return 0;
    }
    */
    rc = sqlite3_prepare_v2(db, input_sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) { // 등록된 id와 pwd가 일치함
        sqlite3_finalize(res);
        sqlite3_close(db);
        input_sql[0] = '\0';
        printf("등록된 id와 pwd가 모두 일치합니다.\n");
        return 1;
    }
    else { // 등록된 id와 pwd가 일치하지 않음
        printf("등록된 id의 pwd가 아닙니다.종료됩니다.\n");
        sqlite3_finalize(res);
        sqlite3_close(db);
        input_sql[0] = '\0';
        return 0;
    }
}

int selPublicKey(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT char sel_pk[PKlen]) { // 공개키 검색 // case 39
    char id[IDlen];
    char pwd[PWDlen];
    OUT char public_key[PKlen]; // OUT publickey를 반환함
}

int delWhiteList(char whitelist[]) { // 화이트리스트 삭제 // 46
    sqlite3 *db;
    char *errmsg;
    int rc;

    const char *NotUsed = "Callback Function Called";

    char sql_ww[SQLlen] = { 0, };
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

    sqlite3_close(db);

    return 0;
}

int delAdmin(const char id[IDlen], const char pwd[PWDlen]) { // 관리자 삭제 // case 47
    sqlite3 *db;
    char *errmsg;
    int rc;

    char sql_de_ia[SQLlen] = { 0, }; // id 기준으로 관리자 삭제 sql
    char sql_de_ii[SQLlen] = { 0, }; // id 기준으로 info 삭제 sql

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

    if(checkIDPWD(id) == 0) { // DBManage.c : 입력한 id&pwd 일치하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

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

    sqlite3_close(db);

    return 1;
}
