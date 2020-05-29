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

int inAD_INFO(); // case 16 ok
int inMAC(); // case 17 // 통신과 연결해야해서 작성 못함
int inWL(); // case 18 ok
int upAD_INFO(); // case 26 ok
int upMAC(); // case 27 // 통신과 연결해야해서 작성 못함
int search_id(); // case 36 // id를 기준으로 검색
int search_wl(); // case 37 // whitelist를 기준으로 검색
int delAD_INFO(); // case 46
int delMAC(); // case 47 // 통신과 연결해야해서 작성 못함
int delWL(); // case 48
int searchId(); // case 56 : id 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
int searchPwd(char id[]); // case 57 : pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
int search_ip(); // case 58 : id와 pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수

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

int checkIDPWD(char id[]) { // 입력한 id를 매개변수로 받아서 수정이나 삭제 시 id&pwd로 확인 (성공하면 1을 반환)
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

    // CPS.db OPEN
    rc = sqlite3_open("CPS.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open CPS DB : %s\n", sqlite3_errmsg(db));
       	return 0;
    }
   	else {
        //fprintf(stderr, "Opened CPS database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    while(1) { // 수정, 삭제하려는 데이터 id의 pwd
        puts("수정, 삭제하려는 id의 원래 PWD 입력(PWD 틀리면 종료):");
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
