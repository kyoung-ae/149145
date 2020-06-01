#include "DB.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) { // callback
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int delAD_INFO() { // case 36
    sqlite3 *db;
 /*   char *errmsg;
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
    char date[DATElen] = { 0, };

    int b_date = 0; // birth 문자열을 정수형으로 받을 변수

    int i = 0;
    int pos_at = 0; // email 문자열의 @ 위치
    int pos_dot = 0; // email 문자열의 . 위치

    int p_number = 0; // phone 문자열이 숫자가 아닐 때마다 1씩 증가되는 변수

    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t);

    const char *NotUsed = "Callback Function Called";

    char sql_ua[SQLlen] = { 0, }; // ADMIN table 업데이트 sql
    char sql_uin[SQLlen] = { 0, }; // INFO name 업데이트 sql
    char sql_uib[SQLlen] = { 0, }; // INFO birth 업데이트 sql
    char sql_uie[SQLlen] = { 0, }; // INFO email 업데이트 sql
    char sql_uip[SQLlen] = { 0, }; // INFO phone 업데이트 sql

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

    NotUsed = "Callback Function Called";

    printf("ADMIN TABLE's Update\n");

    puts("삭제하는 데이터의 id(기본키) 입력:");
    fgets(id, IDlen, stdin);
    puts("삭제하는 id의 PWD 입력(PWD 틀리면 종료):");
    fgets(del_pwd, PWDlen, stdin);

    __fpurge(stdin);
    strncpy(input_sql, "SELECT id, access FROM ADMIN WHERE pwd = '", SQLlen-1);
    strncat(input_sql, del_pwd, SQLlen-1);
    strncat(input_sql, "' AND id = '", SQLlen-1);
    strncat(input_sql, id, SQLlen-1);
    strcat(input_sql, "';");

    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print Admin Table : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print Admin Table successfully\n");
    }

    rc = sqlite3_get_table(db, input_sql, &result, &row, &col, &errmsg);
    //printf("%s\n", result[0]);  // id 와 pwd 를 정확히 입력하면 id 라고 화면에 출력됨.
    if(!strcmp(result[0], "id")) {
        __fpurge(stdin);
        strncpy(input_sql, "DELETE from ADMIN where id = '", SQLlen-1);
        strncat(input_sql, id, SQLlen-1);
        strcat(input_sql, "';");
        //printf("%s\n", input_sql);

        rc = sqlite3_exec(db, input_sql, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
                fprintf(stderr, "Can't delete : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "Delete successfully\n");
        }
    }
    sqlite3_free_table(result);
    sqlite3_close(db);

    return 0;
}
int delINFO() {
    sqlite3 *db;
    char *errmsg;
    int rc;
    const char *NotUsed;
    char input_sql[SQLlen] = { 0, };
    char id[IDlen] = { 0, };
    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char date[DATElen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

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

    NotUsed = "Callback Function Called";

    puts("삭제하는 데이터의 id(기본키) 입력:");
    fgets(id, IDlen, stdin);

    __fpurge(stdin);;
    strncpy(input_sql, "SELECT * FROM INFO WHERE id = '", SQLlen-1);
    strncat(input_sql, id, SQLlen-1);
    strcat(input_sql, "';");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print Info Table : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print Info Table successfully\n");
    }

    __fpurge(stdin);
    strncpy(input_sql, "DELETE from INFO where id = '", SQLlen-1);
    strncat(input_sql, id, SQLlen-1);
    strcat(input_sql, "';");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, NotUsed, &errmsg);
    if(rc != SQLITE_OK) {
            fprintf(stderr, "Can't delete : %s\n", sqlite3_errmsg(db));
            return 1;
    }
    else {
        fprintf(stderr, "Delete successfully\n");
    }
    sqlite3_close(db);
*/
    return 0;
}

int delMAC() { // case 47 // 통신과 연결해야해서 작성 못함

}

int delWL() { // case 48
    sqlite3 *db;
   	char *errmsg;
    const char *NotUsed;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

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

    NotUsed = "Callback Function Called";

    puts("WHITELIST TABLE's records delete.\n");
    puts("input whitelist:");
    fgets(whitelist, WLlen, stdin);
    puts("input id:");
    fgets(id, IDlen, stdin);

    __fpurge(stdin);
    strncpy(input_sql, "DELETE from WHITELIST where whitelist = '", SQLlen-1);
    strncat(input_sql, whitelist, SQLlen-1);
    strcat(input_sql, "';");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, NotUsed, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't delete : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Delete successfully\n");
    }

    sqlite3_close(db);

    return 0;
}
