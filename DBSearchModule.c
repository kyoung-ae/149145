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
    sqlite3_stmt *res;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char sql_s_i0[SQLlen] = { 0, };
    char sql_s_i1[SQLlen] = { 0, };
    char sql_s_i2[SQLlen] = { 0, };
    char sql_s_i3[SQLlen] = { 0, };
    char sql_s_i4[SQLlen] = { 0, };
    char sql_s_i5[SQLlen] = { 0, };
    char sql_s_i6[SQLlen] = { 0, };
    char sql_s_i7[SQLlen] = { 0, };
    char sql_s_i8[SQLlen] = { 0, };

    char id[IDlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int p_number = 0; // str_col 문자열이 1~8의 숫자가 아닐 때마다 1씩 증가되는 변수
    int col_number[COLlen] = { 0, }; // int str_col 을 정수로 변환
    char str_col[COLlen] = { 0, }; // 사용자가 입력한 문자열(항목 번호)
    char col[COLNAMElen] = { 0, };
    int i = 0; // str_col 숫자 확인 반복문

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
        puts("\nsearch id (9bytes 보다 길면 다시 입력함):");
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
            printBOF_gets(str, strsize, IDlen);
    }
    strncpy(id, str, IDlen-1);

    if(checkID(id) == 0) { // 사용자가 입력한 id가 존재하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        //fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    printf("\n입력한 id : %s를 기준으로 검색결과를 출력합니다.\n", id);
    printf("\n검색 결과 항목은 pwd를 제외하고 모든 항목이 출력됩니다.\n");
    printf("모든 항목 출력이 아닌, 원하는 항목을 지정하려면 해당 번호를 누르세요.\n");

    while(1) { // 검색 결과 항목 선택
        print_search_i();
        gets(str_col);

        if(str_col[0] == '\n' || str_col[0] == '\0') // null 이면 모든 항목을 출력함
            break;
        if(str_col[0] == '0')  // 0을 선택하면 종료됨
            exit(0);

        removeSpace(str_col); // 공백이 입력된 경우는 제거
        strsize = strlen(str_col)+1;

        if(strsize > COLlen) { // 길이 제한을 초과한 경우는 반복
            printBOF_gets(str, strsize, COLlen);
            continue;
        }

        for(p_number = 0, i = 0; i < strsize; i++) { // 입력받은 숫자를 하나씩 확인하는 루프
            if((str_col[i] < '0') || (str_col[i] > '8')) // 숫자가 아닌 문자가 입력된 경우 반복
                p_number++; // 숫자가 아니면 p_number 변수 값에 1을 더함
        }
        if(p_number == 1) // 모두 9가 아닌 숫자로만 입력된 경우는 무한루프 탈출, 맨 끝의 null문자 때문에 p_number가 1이면 참이 됨
            break; //break 탈출이 안 됨 (세그 폴트 ㅠㅠ 단일 소스로는 이상없이 돌아감...이건..무슨 경우?)
            // 소스 수정없이 업데이트와 업그레이드 진행 후에 해결됨...(세그 폴트 발생 이유도, 해결된 이유도 여전히 모르겠음)
            // 소스 안 건드렸는데 또 세그폴트 됨...망할...이거 뭐야 진짜...
    }
    if(str_col[0] == '\0') { // null(EnterKey) 인 경우는 전부 출력되야 하므로 0으로 지정
        __fpurge(stdin);
        strncpy(sql_s_i0, "select admin.id, admin.access, info.name, info.birth, info.email, info.phone, info.date,", 88);
        strncat(sql_s_i0, " whitelist.whitelist, whitelist.date ", 37);
        strncat(sql_s_i0, "from admin, info, whitelist where admin.id = '", 46);
        strncat(sql_s_i0, id, IDlen-1);
        strncat(sql_s_i0, "' and info.id = '", 17);
        strncat(sql_s_i0, id, IDlen-1);
        strncat(sql_s_i0, "' and whitelist.id = '", 22);
        strncat(sql_s_i0, id, IDlen-1);
        strncat(sql_s_i0, "';", 2);
        rc = sqlite3_exec(db, sql_s_i0, callback, 0, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't Search : %s (All col)\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Search success : all col\n");
            printf("이 메시지가 보이는데, 원하는 결과가 안 나왔다면\n");
            printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
            printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
            printf("검색 항목을 선택할 때, 2 ~ 7 항목을 선택하세요.\n");
        }
    }
    else { // 값이 있으면 정수형으로 변환
        for(i = 0; i < strlen(str_col); i++) {
            col_number[i] = atoi(str_col[i]);
            printf("col_number[%d]는 %d입니다.", i, col_number[i]);
        }
    }

    switch(col_number[i]) { // 사용자가 검색 항목을 지정한 경우
        case 1: // whitelist.whitelist
            __fpurge(stdin);
            strncpy(sql_s_i1, "SELECT whitelist FROM whitelist WHERE id = '", 44);
            strncat(sql_s_i1, id, IDlen-1);
            strncat(sql_s_i1, "';", 2);
            rc = sqlite3_exec(db, sql_s_i1, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (1.whitelist.whitelist)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 1.whitelist.whitelist\n");
                printf("이 메시지가 보이는데, 원하는 결과가 안 나왔다면\n");
                printf("Whitelist 테이블에 방금 입력한 id로 등록된 게 없기 때문입니다.\n");
                printf("whitelist 테이블에 방금의 id로 등록을 하거나,\n");
                printf("검색 항목을 선택할 때, 2 ~ 7 항목을 선택하세요.\n");
            }
            break;

        case 2: // admin.access
            __fpurge(stdin);
            strncpy(sql_s_i2, "SELECT access FROM admin WHERE id = '", 37);
            strncat(sql_s_i2, id, IDlen-1);
            strncat(sql_s_i2, "';", 2);
            rc = sqlite3_exec(db, sql_s_i2, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (2.admin.access)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 2.admin.access\n");
                printf("N은 권한 지정 안함, E는 로봇 관리자, C는 CPS관리자, R은 Root.\n");
                printf("access 수정을 원하면 DB 시작메뉴에서 수정 기능을 선택하세요.\n");
            }
            break;

        case 3: // info.name
            __fpurge(stdin);
            strncpy(sql_s_i3, "SELECT name FROM info WHERE id = '", 34);
            strncat(sql_s_i3, id, IDlen-1);
            strncat(sql_s_i3, "';", 2);
            rc = sqlite3_exec(db, sql_s_i3, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (3.info.name)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 3.info.name\n");
                printf("이 메시지가 보이는데, 원하는 결과가 안 나왔다면\n");
                printf("방금 입력한 id로 등록된 name 정보가 없기 때문입니다.\n");
                printf("등록을 원하면 DB 시작 메뉴에서 수정 기능을 이용하세요.\n");
            }
            break;

        case 4: // info.birth
            __fpurge(stdin);
            strncpy(sql_s_i4, "SELECT birth FROM info WHERE id = '", 35);
            strncat(sql_s_i4, id, IDlen-1);
            strncat(sql_s_i4, "';", 2);
            rc = sqlite3_exec(db, sql_s_i4, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (4.info.birth)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 4.info.birth\n");
                printf("이 메시지가 보이는데, 원하는 결과가 안 나왔다면\n");
                printf("방금 입력한 id로 등록된 birth 정보가 없기 때문입니다.\n");
                printf("등록을 원하면 DB 시작 메뉴에서 수정 기능을 이용하세요.\n");
            }
            break;

        case 5: // info.email
            __fpurge(stdin);
            strncpy(sql_s_i5, "SELECT email FROM info WHERE id = '", 35);
            strncat(sql_s_i5, id, IDlen-1);
            strncat(sql_s_i5, "';", 2);
            rc = sqlite3_exec(db, sql_s_i5, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (5.info.email)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 5.info.email\n");
                printf("email 수정을 원하면 DB 시작 메뉴에서 수정 기능을 이용하세요.\n");
            }
            break;

        case 6: // info.phone
            __fpurge(stdin);
            strncpy(sql_s_i6, "SELECT phone FROM info WHERE id = '", 35);
            strncat(sql_s_i6, id, IDlen-1);
            strncat(sql_s_i6, "';", 2);
            rc = sqlite3_exec(db, sql_s_i6, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (6.info.phone)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 6.info.phone\n");
                printf("이 메시지가 보이는데, 원하는 결과가 안 나왔다면\n");
                printf("방금 입력한 id로 등록된 phone 정보가 없기 때문입니다.\n");
                printf("등록을 원하면 DB 시작 메뉴에서 수정 기능을 이용하세요.\n");
            }
            break;

        case 7: // info.date
            __fpurge(stdin);
            strncpy(sql_s_i7, "SELECT date FROM info WHERE id = '", 34);
            strncat(sql_s_i7, id, IDlen-1);
            strncat(sql_s_i7, "';", 2);
            rc = sqlite3_exec(db, sql_s_i7, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (7.info.date)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 7.info.date\n");
                printf("보이는 날짜정보는 최초 등록일 기준으로 등록되며,\n");
                printf("등록 후에 관리자 정보항목을 수정하면 최후 수정일로 덮어씁니다.\n");
            }
            break;

        case 8: // whitelist.date
            __fpurge(stdin);
            strncpy(sql_s_i8, "SELECT date FROM whitelist WHERE id = '", 39);
            strncat(sql_s_i8, id, IDlen-1);
            strncat(sql_s_i8, "';", 2);
            rc = sqlite3_exec(db, sql_s_i8, callback, 0, &errmsg);
            if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't Search : %s (8.whitelist.date)\n", sqlite3_errmsg(db));
                return 1;
            }
            else {
                fprintf(stderr, "Search success : 8.whitelist.date\n");
                printf("보이는 날짜정보는 최초 등록일이며, 이후에 수정되지 않습니다.\n");
            }
            break;

        default:
            break;
    }

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
    sqlite3_close(db);

    return 0;
}

int search_wl() { // case 37 : whitelist를 기준으로 검색
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char sql_s_w[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int p_number = 0; // str_col 문자열이 1~8의 숫자가 아닐 때마다 1씩 증가되는 변수
    int col_number = 0; // int str_col 을 정수로 변환 (search_col(36) 함수의 반환 값)
    char str_col[COLlen] = { 0, }; // 사용자가 입력한 문자열(항목 번호)
    char col[COLNAMElen] = { 0, };
    int i = 0; // str_col 숫자 확인 반복문

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

    while(1) { // whitelist를 기준으로 검색
        puts("\nsearch whitelist (30bytes 보다 길면 다시 입력함):");
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
            printBOF_gets(str, strsize, WLlen);
    }
    strncpy(whitelist, str, WLlen-1);

    if(checkWL(whitelist) == 0) { // 사용자가 입력한 whitelist가 존재하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }

    printf("\n입력한 whitelist : %s를 기준으로 검색결과를 출력합니다.\n", whitelist);
    printf("\n검색 결과 항목은 pwd를 제외하고 모든 항목이 출력됩니다.\n");
    printf("모든 항목 출력이 아닌, 원하는 항목을 지정하려면 해당 번호를 누르세요.\n\n");

    while(1) { // 검색 결과 항목 선택
        print_search_w();
        gets(str_col);

        if(str_col[0] == '\n' || str_col[0] == '\0') // null 이면 모든 항목을 출력함
            break;
        if(str_col[0] == '0')  // 0을 선택하면 종료됨
            exit(0);

        removeSpace(str_col); // 공백이 입력된 경우는 제거
        strsize = strlen(str_col)+1;

        if(strsize > COLlen) { // 길이 제한을 초과한 경우는 반복
            printBOF_gets(str, strsize, COLlen);
            continue;
        }

        for(p_number = 0, i = 0; i < strsize; i++) { // 입력받은 숫자를 하나씩 확인하는 루프
            if((str_col[i] < '0') || (str_col[i] > '8')) // 숫자가 아닌 문자가 입력된 경우 반복
                p_number++; // 숫자가 아니면 p_number 변수 값에 1을 더함
        }
        if(p_number == 1) // 모두 9가 아닌 숫자로만 입력된 경우는 무한루프 탈출, 맨 끝의 null문자 때문에 p_number가 1이면 참이 됨
            break; //break 탈출이 안 됨 ㅠㅠ(세그 폴트 ㅠㅠ 단일 소스로는 이상없이 돌아감...이건..무슨 경우?)
            // 소스 수정없이 업데이트와 업그레이드 진행 후에 해결됨...(세그 폴트 발생 이유도, 해결된 이유도 여전히 모르겠음)
    }

}
