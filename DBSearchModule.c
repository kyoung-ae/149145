#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "BaseDefine.h"
#include "DB.h"
#include "DBPrintModule.h"
#include "DBManage.h"
#include <ctype.h>

#pragma foreign_keys = 1 // 참조키 활성화

static int callback(void *NotUsed, int argc, char **argv, char **azColName) { // callback
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int search_id() { // case 36 : id를 기준으로 검색
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char id[IDlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    while(1) { // id 검색
        puts("search id (9bytes 보다 길면 다시 입력함):");
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
            printBOF_gets(str, strsize, IDlen);
    }
    strncpy(id, str, IDlen-1);

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened CPS database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    /*
    // 정렬 값이 없는 경우 : 테이블에 저장된 순서대로 출력해준다.
    select admin.id, admin.access, info.name, info.birth, info.email, info.phone, info.date,
    whitelist.whitelist, whitelist.date from admin, info, whitelist
    where admin.id = info.id and info.id=whitelist.id;

    또는 정렬하기 위해 위 문장의 맨 마지막 where 부분에 내용을 추가해서 sql문을 실행시킴
    asc는 오름차순(기본값이라서 지정하지 않으면 오름차순<으로 정렬됨), desc는 내림차순> 정렬이다.
    첫 번째 지정한 정렬을 수행하고, 첫 정렬 값이 동일한 경우에만 두 번째 지정한 정렬을 수행한다.
    ,로 몇 개든지 정렬을 추가할 수 있지만, 첫 번째 지정한 정렬을 기준으로 정렬되는 것은 동일하다.

    // 정렬 값이 있는 경우
    select admin.id, admin.access, info.name, info.birth, info.email, info.phone, info.date,
    whitelist.whitelist, whitelist.date  from admin, info, whitelist
    where admin.id = info.id and info.id=whitelist.id
    order by admin.access asc, whitelist.whitelist desc;
    */

    return 0;
}

int search_wl() { // case 37 : whitelist를 기준으로 검색
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

   while(1) { // whitelist를 기준으로 검색
        puts("search whitelis (30bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= WLlen)
            break;

        printBOF_gets(str, strsize, WLlen);
    }
    strncpy(whitelist, str, WLlen-1);

}
/*
int selAD_INFO() {

    //ADMIN Table 출력
    sql = "select id, access from ADMIN";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }

    int sel_at_id() { // case 1
        puts("search id:");
        fgets(id, IDlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT id, access FROM ADMIN WHERE id = '", SQLlen-1);
        strncat(input_sql, id, SQLlen-1);
        strcat(input_sql, "';");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_at_access() { // case 2
        puts("search access:");
        scanf("%d", &access);
        while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
            putchar(tmp);
        }
        sprintf(buf_access, "%d", access); // access를 문자로 변환

        __fpurge(stdin);
        strncpy(input_sql, "SELECT id, access FROM ADMIN WHERE access = ", SQLlen-1);
        strncat(input_sql, buf_access, SQLlen-1);
        strcat(input_sql, ";");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_at_pwd() { // case 3 // int searchPWD(const char search_id[IDlen], const char seearch_pwd[PWDlen]) pwd 인증
        puts("seach pwd's ID:");
        fgets(id, IDlen, stdin);
        puts("search pwd:");
        fgets(cmp_pwd, PWDlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT id, access FROM ADMIN WHERE id = '", SQLlen-1);
        strncat(input_sql, id, SQLlen-1);
        strncat(input_sql, "' AND pwd = '", SQLlen-1);
        strncat(input_sql, cmp_pwd, SQLlen-1);
        strcat(input_sql, "';");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    void Itmenu() { // ADMIN table menu
        printf("1. ID\n");
        printf("2. ACCESS\n");
        printf("3. PWD\n");
        printf("0. EXIT\n");
        printf("검색하려는 항목의 번호를 고르세요:");
        scanf("%d", &menu);
        while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
            putchar(tmp);
        }

        switch(menu) {
            case 1:
                sel_at_id();
                break;

            case 2:
                sel_at_access();
                break;

            case 3:
                sel_at_pwd();
                break;

            default:
                break;
        }
    }
    Itmenu();

    sqlite3_close(db);

    return 0;
}

int selINFO() {
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
   	char *sql; // table schema sql
    char id[IDlen] = { 0, };
    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char date[DATElen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int menu; // search menu
    char tmp; // Enter Key remove

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened CPS database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    //INFO Table 출력
    sql = "select * from INFO";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }

    int sel_it_id() { // case 1
        puts("search id:");
        fgets(id, IDlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM INFO WHERE id = '", SQLlen-1);
        strncat(input_sql, id, SQLlen-1);
        strcat(input_sql, "';");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_it_name() { // case 2
        puts("search name:");
        fgets(name, NAMElen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM INFO WHERE name like '%", SQLlen-1);
        strncat(input_sql, name, SQLlen-1);
        strcat(input_sql, "%';");
        printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_it_birth() { // case 3
        puts("search birth:");
        fgets(birth, BIRTHlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM INFO WHERE birth like '%", SQLlen-1);
        strncat(input_sql, birth, SQLlen-1);
        strcat(input_sql, "%';");
        printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_it_email() { // case 4
        puts("search email:");
        fgets(email, EMAILlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM INFO WHERE email like '%", SQLlen-1);
        strncat(input_sql, email, SQLlen-1);
        strcat(input_sql, "%';");
        printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    int sel_it_phone() { // case 5
        puts("search phone:");
        fgets(phone, PHONElen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM INFO WHERE phone like '%", SQLlen-1);
        strncat(input_sql, phone, SQLlen-1);
        strcat(input_sql, "%';");
        printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    void Itmenu() { // INFO table menu
        printf("1. ID\n");
        printf("2. NAME\n");
        printf("3. BIRTH\n");
        printf("4. EMAIL\n");
        printf("5. PHONE\n");
        printf("0. EXIT\n");
        printf("검색하려는 항목의 번호를 고르세요:");
        scanf("%d", &menu);
        while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
            putchar(tmp);
        }

        switch(menu) {
            case 1:
                sel_it_id();
                break;

            case 2:
                sel_it_name();
                break;

            case 3:
                sel_it_birth();
                break;

            case 4:
                sel_it_email();
                break;

            case 5:
                sel_it_phone();
                break;

            default:
                break;
        }
    }
    Itmenu();

    sqlite3_close(db);

    return 0;
}

int selWL() {
sqlite3 *db;
   	char *errmsg;
   	char *sql; // table schema sql
    int rc;
    char input_sql[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int menu; // search menu
    char tmp; // Enter Key remove

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        fprintf(stderr, "Opened CPS database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    // WHITELIST Table 출력
    sql = "select * from WHITELIST";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }

    int sel_wl_wl() { // case 1
        puts("search whitelist:");
        fgets(whitelist, WLlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM WHITELIST WHERE WHITELIST like '%", SQLlen-1);
        strncat(input_sql, whitelist, SQLlen-1);
        strcat(input_sql, "%';");
        printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }


    int sel_wl_id() { // case 2
        puts("search id:");
        fgets(id, IDlen, stdin);

        __fpurge(stdin);
        strncpy(input_sql, "SELECT * FROM WHITELIST WHERE ID = '", SQLlen-1);
        strncat(input_sql, id, SQLlen-1);
        strcat(input_sql, "';");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't search : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Print search successfully\n");
        }
    }

    void Itmenu() { // WHITELIST table menu
        printf("1. WHITELIST\n");
        printf("2. ID\n");
        printf("0. EXIT\n");
        printf("검색하려는 항목의 번호를 고르세요:\n");
        scanf("%d", &menu);
        while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
            putchar(tmp);
        }
        switch(menu) {
            case 1:
                sel_wl_wl();
                break;

            case 2:
                sel_wl_id();
                break;

            default:
                break;
        }
    }
    sqlite3_close(db);

    return 0;
}
*/
