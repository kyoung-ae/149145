#include "DB.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) { // callback
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int searchId() { // case 56 : id 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
/*    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };
    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, 0};
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };

    int b_date = 0; // birth 문자열을 정수형으로 받을 변수

    int i = 0;
    int pos_at = 0; // email 문자열의 @ 위치
    int pos_dot = 0; // email 문자열의 . 위치

    int p_number = 0; // phone 문자열이 숫자가 아닐 때마다 1씩 증가되는 변수

    switch(col) {
        case 1:
            while(1) { // id 검색
                puts("search id (9bytes 보다 길면 다시 입력함):");
                gets(str);

                if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize <= IDlen)
                    break;

                printBOF_gets(str, strsize, IDlen);
            }
            strncpy(id, str, IDlen-1);
            break;

        case 2:
            while(1) { // access 검색
                puts("search access (E, C, R):");
                puts("E : EV3 ROBOT 관리자");
                puts("C : CPS 관리자");
                puts("R : ROOT");
                puts("N : 권한 취득 전(등록 당시에 EnterKey로 넘긴 경우)");
                gets(str);

                strsize = strlen(str)+1;
                if(strsize > ACCESSlen) {
                    printBOF_gets(str, strsize, ACCESSlen);
                    continue;
                }

                if(str[0] == 'E' || str[0] == 'C' || str[0] == 'R' || str[0] == 'N')
                    break;
                else
                    printf("잘못 입력하셨습니다.\n"); // N, E, C, R 을 제외하고는 무한루프
            }
            strncpy(access, str, ACCESSlen-1);
            break;

        case 3:
            while(1) { // pwd 검색
                puts("search pwd (513bytes 보다 길면 다시 입력함):");
                gets(str);

                if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;
                if(strsize <= PWDlen)
                    break;

                printBOF_gets(str, strsize, PWDlen);
            }
            strncpy(pwd, str, PWDlen-1);
            break;

        case 4:
            while(1) { // name 검색
                puts("search name (30bytes 보다 길면 다시 입력함):");
                gets(str);

                str[0] == '\n'; // 검색 정보는 null 불가
                    continue;
                strsize = strlen(str)+1;

                if(strsize <= NAMElen)
                    break;

                printBOF_gets(str, strsize, NAMElen);
            }
            strncpy(name, str, NAMElen-1);
            break;

        case 5:
            while(1) { // 생년월일은 yyyymmdd 8개의 유효 날짜로 입력받음
                puts("search birth (yyyymmdd 8개 유효 날짜(숫자)가 아니면 다시 입력함):");
                gets(str);

                str[0] == '\0'; // 검색 정보는 null 불가
                    continue;
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
                    printf("yyyymmdd 8개의 유효 날짜(숫자)만 입력해야 합니다!\n");
            }
            strncpy(birth, str, BIRTHlen-1);
            break;

        case 6:
            while(1) { // 이메일 : @는 [1] ~ [끝-2] && .은 [@위치+2] ~ [끝]
                puts("search email (50bytes 보다 길면 다시 입력함):");
                gets(str);

                if(str[0] == '\n' || str[0] == '\0') // 검색 정보는 null 불가
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
            break;

        case 7:
            while(1) { // 전화번호는 자연수만 입력 가능
                puts("search phone (20bytes 보다 길면 다시 입력함):");
                gets(str);

                if(str[0] == '\0') // 검색 정보는 null 불가
                    continue;
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
            break;

        default:
            break;
    }*/
}

int searchPwd() { // case 57 : pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
    sqlite3 *db;
   	char *errmsg;
    int rc;

    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

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
        puts("\ninput id (9bytes 보다 길면 다시 입력함):");
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

    if(checkID(id) == 0) { // DBManage.c : 사용자가 입력한 id가 존재하지 않으면 종료
        sqlite3_close(db);
        return 0;
    }
}

int search_ip() { // case 58 : id, pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
/*    sqlite3 *db;
    sqlite3_stmt *res;
    char *errmsg;
    int rc;
    char input_sql[SQLlen] = { 0, };

    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char id[IDlen] = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, };

    char name[NAMElen] = { 0, };
    char birth[BIRTHlen] = { 0, };
    char email[EMAILlen] = { 0, };
    char phone[PHONElen] = { 0, };
    char date[DATElen] = { 0, };

    char whitelist[WLlen] = { 0, };

    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t);

    int b_date = 0; // birth 문자열을 정수형으로 받을 변수

    int i = 0;
    int pos_at = 0; // email 문자열의 @ 위치
    int pos_dot = 0; // email 문자열의 . 위치
    int p_number = 0; // phone 문자열이 숫자가 아닐 때마다 1씩 증가되는 변수

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

    puts("\nADMIN INFO insert\n");

    while(1) { // 수정, 삭제하려는 데이터 id의 pwd
        puts("수정, 삭제하려는 id의 원래 PWD 입력(PWD 틀리면 종료):");
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

    sqlite3_close(db);

    return 0;*/
}
