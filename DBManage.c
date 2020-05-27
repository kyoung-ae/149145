#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include "BaseDefine.h"
#include "DB.h"
#include "DBPrintModule.h"
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

void removeEnter() { // scanf()의 Enter key 제거
    char tmp;

    while((tmp = getchar()) != '\n') {
        putchar(tmp);
    }
}

char *dateNow(struct tm *t) { // date 가져오는 함수
    static char now[DATElen] = { 0, };

    sprintf(now, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900,
                t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return now;
}

void printBOF_gets(char str[], int strsize, int define_size) { // gets() 버퍼오버플로우 방지 : 재입력 요구 메시지 출력
    printf("입력한 byte(s)는 %dbyte(s)입니다.\n", strsize-1);
    printf("입력 가능한 최대 길이 %dbyte(s)보다 %dbyes(s)가 초과됐습니다.\n", define_size-1, strsize-define_size);
    printf("입력할 데이터를 다시 확인해보세요.\n");
}

int checkDate(int date) { // 입력받은 날짜의 유효성 검사-> 1이면 유효한 날짜 (0이면 없는 날짜임)
    int yyyy, mm, dd = 0;

	yyyy = date / 10000;
    mm = (date-(yyyy*10000)) / 100;
	dd = date % 100 ;
	printf("%d년 %d월 %d일\n", yyyy, mm, dd);

	if(yyyy <= 1000)
		return 0;
	if(mm == 0 || mm >= 13)
		return 0;
	if(dd == 0 || dd >= 32)
		return 0;

	if (mm == 2) { // 2월달의 윤년(29일), 평년(28일) 구분
        if(yyyy%400 == 0) { // 400으로 나눠지면 윤년
            if(dd <= 0 || dd >30)
            	return 0;
		}
		else if((yyyy%4 == 0) && (yyyy%100 != 0)) { // 4와 100으로 나눠지면 윤년
            if(dd <= 0 || dd >30)
            	return 0;
		}
		else { // 그 외에는 전부 평년
			if(dd <=0 || dd > 28)
					return 0;
		}
	}
	else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) { // 4, 6, 9, 11월 -> 30일까지 있음
        if (dd <= 0 || dd > 30)
			return 0;
	}
	else { // 1, 3, 5, 7, 8, 10, 12월 -> 31일까지 있음
		if (dd <= 0 || dd > 31)
			return 0;
	}
	return 1;
}

int search_ip() { // case 5: id, pwd 분실시 지원 기능을 위한 관리자 정보를 호출하는 함수 // 아직 작성 못함.

}

int checkIDPWD(char id[]) { // 호출 직전에 입력한 id를 매개변수로 받아서 수정이나 삭제 시 id&pwd로 확인 (성공하면 1을 반환)
    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    const char *NotUsed = "Callback Function Called";

    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char** result; // get table result
    int row, col; // get table row,column

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        //fprintf(stderr, "Opened CPS database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    while(1) { // 수정하려는 데이터 id의 pwd
        puts("수정할 데이터 id의 원래 PWD 입력(PWD 틀리면 종료):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen);
    }
    strncpy(pwd, str, PWDlen-1);

    __fpurge(stdin);
    strncpy(input_sql, "SELECT CASE WHEN id=='", 22);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "' AND pwd=='", 12);
    strncat(input_sql, pwd, PWDlen-1);
    strncat(input_sql, "' THEN 'Success' ELSE 'Failure' END FROM ADMIN;", 47);
    /*
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print Admin Table : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    */

    rc = sqlite3_prepare_v2(db, input_sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW)
        printf("%s\n", sqlite3_column_text(res, 0));

    if(strcmp("Failure",sqlite3_column_text(res, 0)) == 0) { // 일치하지 않음
        sqlite3_finalize(res);
        sqlite3_close(db);
        return 0;
    }
    else { // id와 pwd가 일치함
        sqlite3_finalize(res);
        sqlite3_close(db);
        return 1;
    }
}

int inAD_INFO() { // case 16 ok
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

    puts("\nADMIN INFO insert\n");

    while(1) { // id 필수로 입력받음
        printf("ID는 등록 후에 수정이 불가능합니다!!!\n");
        printf("ID는 필수입력 정보입니다.\n");
        puts("input id (9bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= IDlen)
            break;

        printBOF_gets(str, strsize, IDlen);
    }
    strncpy(id, str, IDlen-1);

    while(1) { // access 입력받음
        printf("ACCESS는 수정이 가능합니다.\n");
        puts("input access (1byte 보다 길면 다시 입력함):");
        gets(str);

        strsize = strlen(str)+1;
        if(strsize <= ACCESSlen)
            break;

        printBOF_gets(str, strsize, ACCESSlen);
    }
    strncpy(access, str, ACCESSlen-1);

    while(1) { // pwd 필수로 입력받음
        printf("PWD는 수정이 가능합니다.\n");
        printf("PWD는 필수입력 정보입니다.\n");
        puts("input pwd (513bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen);
    }
    strncpy(pwd, str, PWDlen-1);

    __fpurge(stdin);
    strncpy(input_sql, "insert into ADMIN(id, access, pwd) values('", 43);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, access, ACCESSlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, pwd, PWDlen-1);
    strncat(input_sql, "');", 3);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input id, access, pwd : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Insert success (id, access, pwd)\n");
    }

    printf("\n지금부터 입력하는 id가 %s인 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n", id);
    printf("등록하려면 입력 후 EnterKey,\n");
    printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");

    while(1) { // name 입력받음
        printf("\n지금 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
        printf("NAME은 수정이 가능합니다.\n");
        printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");
        puts("input name (30bytes 보다 길면 다시 입력함):");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;

        if(strsize <= NAMElen)
            break;

        printBOF_gets(str, strsize, NAMElen);
    }
    strncpy(name, str, NAMElen-1);

    while(1) { // 생년월일은 yyyymmdd 8개의 유효 날짜로 입력받음
        printf("\n지금 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
        printf("BIRTH는 수정이 가능합니다.\n");
        printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");
        puts("input birth (yyyymmdd 8개 유효 날짜(숫자)가 아니면 다시 입력함):");
        gets(str);

        str[0] == '\0'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;

        if(strsize == BIRTHlen) {
            b_date = atoi(str);
            if(checkDate(b_date) == 1) // 1이면 참(유효날짜)
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

    while(1) { // 이메일 : @는 [1] ~ [끝-2] && .은 [@위치+2] ~ [끝]
        printf("\n지금 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
        printf("EMAIL은 수정이 가능합니다.\n");
        printf("EMAIL은 필수 입력정보입니다..\n");
        puts("input email (50bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize > EMAILlen) {
            printBOF_gets(str, strsize, EMAILlen);
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

    while(1) { // 전화번호는 자연수만 입력 가능
        printf("\n지금 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
        printf("PHONE 번호는 수정이 가능합니다.\n");
        printf("등록을 건너띄려면 바로 Enter를 누르고, 등록을 원하면 2 ~ 20개의 수를 입력하세요.\n");
        printf("-표시 없이 숫자만 입력하세요.\n");
        puts("input phone (20bytes 보다 길면 다시 입력함):");
        gets(str);
        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > PHONElen) {
            printBOF_gets(str, strsize, PHONElen);
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

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    __fpurge(stdin);
    strncpy(input_sql, "insert into INFO(id, name, birth, email, phone, date) values('", 62);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, name, NAMElen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, birth, BIRTHlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, email, EMAILlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, phone, PHONElen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, date, DATElen-1);
    strncat(input_sql, "');", 3);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input id, name, birth, email, phone, date: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Insert success (id, name, birth, email, phone, date)\n");
    }
    sqlite3_close(db);

    return 0;
}

int inMAC() { // case 17 // 통신과 연결해야해서 작성 못함

}

int inWL() { // case 18 ok
    sqlite3 *db;
   	char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char date[DATElen] = { 0, };
    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t);

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

    puts("WHITELIST TABLE's insert\n");

    while(1) { // whitelist 필수로 입력받음
        printf("whitelist는 등록 후에 수정이 불가능합니다!!!\n");
        printf("whitelist는 필수입력 정보입니다.\n");
        puts("input whitelis (30bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= WLlen)
            break;

        printBOF_gets(str, strsize, WLlen);
    }
    strncpy(whitelist, str, WLlen-1);

    while(1) { // id 필수로 입력받음
        printf("ID는 등록 후에 수정이 불가능합니다!!!\n");
        printf("ID는 필수입력 정보입니다.\n");
        puts("input id (9bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= IDlen)
            break;

        printBOF_gets(str, strsize, IDlen);
    }
    strncpy(id, str, IDlen-1);

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    __fpurge(stdin);
    strncpy(input_sql, "insert into WHITELIST(whitelist, id, date) values('", 51);
    strncat(input_sql, whitelist, WLlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "','", 3);
    strncat(input_sql, date, DATElen-1);
    strncat(input_sql, "');", 3);
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print input successfully\n");
    }
    sqlite3_close(db);

    return 0;
}

int upAD_INFO() { // case 26 ok
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
        fprintf(stderr, "Can't open CPS.db : %s\n", sqlite3_errmsg(db));
       	return 1;
    }
   	else {
        //fprintf(stderr, "Opened CPS.db\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    printf("ADMIN INFO Update\n");

    while(1) { // 수정하려는 데이터의 id
        puts("수정할 데이터의 id(기본키) 입력:");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= IDlen)
            break;

        printBOF_gets(str, strsize, IDlen);
    }
    strncpy(id, str, IDlen-1);

    if(checkIDPWD(id) == 1); // 사용자가 입력한 id&pwd 동일한 경우에 실행되는 수정 기능

    __fpurge(stdin);
    strncpy(input_sql, "SELECT name, birth, email, phone, date FROM INFO WHERE id = '", 61);
    strncat(input_sql, id, IDlen-1);
    strncat(input_sql, "';", 2);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print (name, birth, email, phone, date): %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        //fprintf(stderr, "Print successfully\n");
    }

    while(1) { // 수정하려는 id의 access 수정
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("ACCESS(권한) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize <= ACCESSlen)
            break;

        printBOF_gets(str, strsize, ACCESSlen);
    }
    strncpy(access, str, ACCESSlen-1);
    //update admin set access = 'access' where access is not null and id = 'id'; // null이 아닌 access 값 수정

    while(1) { // 수정하려는 id의 pwd 수정
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("PWD(비밀번호) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen);
    }
    strncpy(pwd, str, PWDlen-1);

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
            fprintf(stderr, "access 값을 %s로 수정 성공\n", access);
        }
    }

    if((strlen(access) == 0) && (strlen(pwd) != 0)) { // pwd만 수정
        __fpurge(stdin);
        strncpy(sql_ua, "UPDATE ADMIN SET pwd = '", 24);
        strncat(sql_ua, pwd, PWDlen-1);
        strncat(sql_ua, "' WHERE id = '", 14);
        strncat(sql_ua, id, IDlen-1);
        strncat(sql_ua, "';", 2);
        //printf("%s\n", sql_ua);

        rc = sqlite3_exec(db, sql_ua, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (pwd) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "pwd 값을 %s로 수정 성공\n", pwd);
        }
    }

    if((strlen(access) != 0) && (strlen(pwd) != 0)) { // access & pwd 함께 수정
        __fpurge(stdin);
        strncpy(sql_ua, "UPDATE ADMIN SET (access, pwd) = ('", 35);
        strncat(sql_ua, access, ACCESSlen-1);
        strncat(sql_ua, "', '", 4);
        strncat(sql_ua, pwd, PWDlen-1);
        strncat(sql_ua, "') WHERE id = '", 15);
        strncat(sql_ua, id, IDlen-1);
        strncat(sql_ua, "';", 2);
        //printf("%s\n", sql_ua);

        rc = sqlite3_exec(db, sql_ua, callback, NotUsed, &errmsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "Update ERROR (access&pwd) : %s\n", sqlite3_errmsg(db));
            return 1;
        }
        else {
            fprintf(stderr, "access 값은 %s로, pwd 값은 %s로 수정 성공\n", access, pwd);
        }
    }

    printf("\n지금부터 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
    printf("기존 정보에서 수정을 원하지 않는 항목은 EnterKey만 누르세요.\n");

    while(1) { // 수정하려는 id의 name 수정
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("NAME(이름) 변경 값 입력:");
        gets(str);

        str[0] == '\n'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize <= NAMElen)
            break;

        printBOF_gets(str, strsize, NAMElen);
    }
    strncpy(name, str, NAMElen-1);

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    if(strlen(name) != 0) { // name 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uin, "UPDATE INFO SET (name, date) = ('", 33);
        strncat(sql_uin, name, NAMElen-1);
        strncat(sql_uin, "','", 3);
        strncat(sql_uin, date, DATElen-1);
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
            fprintf(stderr, "name 값을 %s로 수정 성공\n", name);
        }
    }

    while(1) { // 수정하려는 id의 생년월일(yyyymmdd 8개의 유효 날짜) 수정
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("birth(생년월일 yyyymmdd 8개 날짜) 변경 값 입력:");
        gets(str);

        str[0] == '\0'; // EnterKey를 누르면 무한푸프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize == BIRTHlen) {
            b_date = atoi(str);
            if(checkDate(b_date) == 1) // 1이면 참(유효날짜)
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

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    if(strlen(birth) != 0) { // birth 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uib, "UPDATE INFO SET (birth, date) = ('", 34);
        strncat(sql_uib, birth, BIRTHlen-1);
        strncat(sql_uib, "','", 3);
        strncat(sql_uib, date, DATElen-1);
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
            fprintf(stderr, "birth 값을 %s로 수정 성공\n", birth);
        }
    }

    while(1) { // 이메일 수정 : @는 [1] ~ [끝-2] && .은 [@위치+2] ~ [끝]
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        puts("email 변경 값 입력:");
        gets(str);

        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > EMAILlen) {
            printBOF_gets(str, strsize, EMAILlen);
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

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    if(strlen(email) != 0) { // email 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uie, "UPDATE INFO SET (email, date) = ('", 34);
        strncat(sql_uie, email, EMAILlen-1);
        strncat(sql_uie, "','", 3);
        strncat(sql_uie, date, DATElen-1);
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
            fprintf(stderr, "email 값을 %s로 수정 성공\n", email);
        }
    }

    while(1) { // 전화번호 수정
        printf("수정을 건너띄려면 EnterKey만 누르세요.\n");
        printf("수정을 원하면 2 ~ 20개의 수를 -표시 없이 입력하세요.\n");
        puts("phone(전화번호) 변경 값 입력:");
        gets(str);

        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > PHONElen) {
            printBOF_gets(str, strsize, PHONElen);
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

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    if(strlen(phone) != 0) { // phone 변경 값이 있으면 수정
        __fpurge(stdin);
        strncpy(sql_uip, "UPDATE INFO SET (phone, date) = ('", 34);
        strncat(sql_uip, phone, PHONElen-1);
        strncat(sql_uip, "','", 3);
        strncat(sql_uip, date, DATElen-1);
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
    }
    sqlite3_close(db);

    return 0;
}

int upMAC() { // case 27 // 통신과 연결해야해서 작성 못함

}

int upWL() { // case 28
    sqlite3 *db;
   	char *errmsg;
    const char *NotUsed;
    int rc;
    char *sql;
    char input_sql[SQLlen] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char src_white[WLlen] = { 0, }; // 수정할 화이트리스트 ->다른 화이트리스트로 변경됨.
    char whitelist[WLlen] = { 0, };

    char date[DATElen] = { 0, };
    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t);

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

    puts("WHITELIST TABLE's Update\n");
    puts("update source whitelist:");
    gets(src_white);
    puts("update whitelist:");
    gets(whitelist);

    puts("date Enter:");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    __fpurge(stdin);
    strncpy(input_sql, "UPDATE WHITELIST SET (date, whitelist) = ('", SQLlen-1);
    strncat(input_sql, date, SQLlen-1);
    strncat(input_sql, "', '", SQLlen-1);
    strncat(input_sql, whitelist, SQLlen-1);
    strncat(input_sql, "') ", SQLlen-1);
    strncat(input_sql, "where whitelist = '", SQLlen-1);
    strncat(input_sql, src_white, SQLlen-1);
    strcat(input_sql, "';");
    printf("%s\n", input_sql);

    rc = sqlite3_exec(db, input_sql, callback, NotUsed, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't whitelist Update : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Whitelist update successfully\n");
    }

    sqlite3_close(db);

    return 0;
}

int delAD_INFO() { // case 36
    sqlite3 *db;
    char *errmsg;
    const char *NotUsed;
    int rc;
    char input_sql[SQLlen-1] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char** result; // get table result
    int row, col; // get table row,column

    int access;

    char del_pwd[PWDlen] = { 0, }; // 삭제되는 PWD
    char pwd[PWDlen] = { 0, };

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

int delMAC() { // case 37 // 통신과 연결해야해서 작성 못함

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

    return 0;
}

int delWL() { // case 38
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

int selAD_INFO() { // case 46
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
   	char *sql; // table schema sql
    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char cmp_pwd[PWDlen]; // 원본 pwd값과 비교 할 변수
    int access;
    char tmp; //엔터키 삭제 변수
    char buf_access[2]; // int형의 access값을 문자로 받을 변수
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    int menu; // search menu

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

int selMAC() { // case 47 // 통신과 연결해야해서 작성 못함

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

int selWL() { // case 48
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
    //Itmenu();
    sqlite3_close(db);

    return 0;
}
