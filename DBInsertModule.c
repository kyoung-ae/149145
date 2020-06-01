#include "DB.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) { // callback
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int inAD_INFO() { // case 16 ok
    sqlite3 *db;
    char *errmsg;
    int rc;
    char input_sql_ia[SQLlen] = { 0, }; // admin 테이블
    char input_sql_ii[SQLlen] = { 0, }; // info 테이블
    char id[IDlen];// = { 0, };
    char pwd[PWDlen] = { 0, };
    char access[ACCESSlen] = { 0, 0};
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
    char *str_now = dateNow(t); // DBManage.c

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

    puts("\nADMIN INFO insert");

    while(1) { // id 필수로 입력받음
        printf("\nID는 등록 후에 수정이 불가능합니다!!!\n");
        printf("ID는 필수입력 정보입니다.\n");
        puts("input id (5~9bytes 길이로 입력하세요):");
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
            printBOF_gets(str, strsize, IDlen); // DBManage.c
    }
    strncpy(id, str, IDlen-1);

    while(1) { // access 입력받음
        printf("\nACCESS는 수정이 가능합니다.\n");
        puts("권한 취득 전이면 엔터키를 누르세요->기본 값(N)으로 등록됩니다.");
        puts("input access (E, C, R):");
        puts("E : EV3 ROBOT 관리자");
        puts("C : CPS 관리자");
        puts("R : ROOT");
        puts("N : default 아직 권한 취득 전");
        gets(str);

        strsize = strlen(str)+1;
        if(strsize > ACCESSlen) {
             printBOF_gets(str, strsize, ACCESSlen); // DBManage.c
             continue;
        }

        if(str[0] == 'E' || str[0] == 'C' || str[0] == 'R' || str[0] == 'N')
            break;
        if(str[0] == '\0') {
            str[0] = 'N';
            break;
        }
        else
            printf("잘못 입력하셨습니다.\n"); // 엔터키(N), E, C, R 을 제외하고는 무한루프
    }
    strncpy(access, str, ACCESSlen-1);

    while(1) { // pwd 필수로 입력받음
        printf("\nPWD는 수정이 가능합니다.\n");
        printf("PWD는 필수입력 정보입니다.\n");
        puts("input pwd (513bytes 보다 길면 다시 입력함):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize <= PWDlen)
            break;

        printBOF_gets(str, strsize, PWDlen); // DBManage.c
    }
    strncpy(pwd, str, PWDlen-1);

    __fpurge(stdin);
    strncpy(input_sql_ia, "insert into ADMIN(id, Access, pwd) values('", 43);
    strncat(input_sql_ia, id, IDlen-1);
    strncat(input_sql_ia, "','", 3);
    strncat(input_sql_ia, access, ACCESSlen-1);
    strncat(input_sql_ia, "','", 3);
    strncat(input_sql_ia, pwd, PWDlen-1);
    strncat(input_sql_ia, "');", 3);
    //printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql_ia, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input id, Access, pwd : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Insert success (id, Access, pwd)\n");
    }

    printf("\n지금 입력하는 id가 %s인 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n", id);
    printf("등록하려면 입력 후 EnterKey,\n");
    printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");

    while(1) { // name 입력받음
        printf("\n지금 입력하는 정보는 id, pwd 분실 시 확인 정보로 사용됩니다!!!\n");
        printf("NAME은 수정이 가능합니다.\n");
        printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");
        puts("input name (2~30bytes 길이로 입력하세요):");
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
            printBOF_gets(str, strsize, NAMElen); // DBManage.c
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
            printBOF_gets(str, strsize, EMAILlen); // DBManage.c
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
        printf("등록을 건너띄려면 바로 EnterKey를 누르세요.\n");
        printf("-표시 없이 숫자만 입력하세요.\n");
        puts("input phone (2~20개의 숫자로만 입력하세요):");
        gets(str);
        if(str[0] == '\0') // EnterKey를 누르면 무한루프 탈출
            break;
        strsize = strlen(str)+1;
        if(strsize > PHONElen) {
            printBOF_gets(str, strsize, PHONElen); // DBManage.c
            continue;
        }

        if(strsize < 3) { // 숫자 1개만 입력하면 반복
            printf("최소 2개의 숫자를 입력하세요!!!\n");
            continue;
        }

        for(p_number = 0, i = 0; i < strsize; i++) { // 입력받은 숫자를 하나씩 확인하는 루프
            if((str[i] < '0') || (str[i] > '9')) // 숫자가 아닌 문자가 입력된 경우 반복
                p_number++; // 숫자가 아니면 p_number 변수 값에 1을 더함
        }
        if(p_number == 1) // 전화번호가 길이제한을 넘기지 않고, 모두 숫자로만 입력된 경우는 무한루프 탈출
            break;
    }
    strncpy(phone, str, PHONElen-1);

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    __fpurge(stdin);
    strncpy(input_sql_ii, "insert into INFO(id, name, birth, email, phone, date) values('", 62);
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
    strncat(input_sql_ii, date, DATElen-1);
    strncat(input_sql_ii, "');", 3);
    //printf("%s\n", input_sql_ii);
    rc = sqlite3_exec(db, input_sql_ii, callback, 0, &errmsg);
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
    char input_sql_iw[SQLlen] = { 0, };
    char whitelist[WLlen] = { 0, };
    char id[IDlen] = { 0, };
    int strsize = 0; // 실제로 사용자에게 입력 받은 글자수를 확인
    char str[MAX] = { 0, }; // 사용자에게 입력받은 임시 문자열

    char date[DATElen] = { 0, };
    struct tm *t;
    time_t now;
    now = time(NULL);
    t = localtime(&now);
    char *str_now = dateNow(t); // DBManage.c

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

    puts("WHITELIST TABLE's insert\n");

    while(1) { // whitelist 필수로 입력받음
        printf("\nwhitelist는 등록 후에 수정이 불가능합니다!!!\n");
        printf("whitelist는 필수입력 정보입니다.\n");
        puts("input whitelist (3~30bytes 길이로 입력하세요.):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        strsize = strlen(str)+1;
        if(strsize >=4 && strsize <= WLlen)
            break;
        else if(strsize < 4) {
            printf("3 btyes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, WLlen); // DBManage.c
    }
    strncpy(whitelist, str, WLlen-1);

    while(1) { // id 필수로 입력받음
        printf("\nID는 등록 후에 수정이 불가능합니다!!!\n");
        printf("ID는 필수입력 정보입니다.\n");
        puts("input id (5~9bytes 길이로 입력하세요):");
        gets(str);

        if(str[0] == '\n' || str[0] == '\0') // 필수입력 정보여서 null 불가
            continue;
        if(strsize >= 6 && strsize <= IDlen) // id는 5~9bytes 길이 제한
            break;
        else if(strsize < 6) {
            printf("5 btyes 보다 길게 입력하세요!\n");
            continue;
        }
        else
            printBOF_gets(str, strsize, IDlen); // DBManage.c
    }
    strncpy(id, str, IDlen-1);

    puts("EnterKey를 누르세요.");
    printf("%s\n", str_now);
    strncpy(date, str_now, DATElen);

    __fpurge(stdin);
    strncpy(input_sql_iw, "insert into WHITELIST(whitelist, id, date) values('", 51);
    strncat(input_sql_iw, whitelist, WLlen-1);
    strncat(input_sql_iw, "','", 3);
    strncat(input_sql_iw, id, IDlen-1);
    strncat(input_sql_iw, "','", 3);
    strncat(input_sql_iw, date, DATElen-1);
    strncat(input_sql_iw, "');", 3);
    //printf("%s\n", input_sql_iw);
    rc = sqlite3_exec(db, input_sql_iw, callback, 0, &errmsg);
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
