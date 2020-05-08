#include <stdio.h>
#include "DB.h"

void printMenu(int pr_menu) {
    int tb = 0;
    int func = 0;
    int menu = 0;

    char tmp; // 엔터키 삭제 변수

    printf("Select Number\n");
    printf("1 : 등록 (insert)\n");
    printf("2 : 수정 (update)\n");
    printf("3 : 삭제 (delete)\n");
    printf("4 : 검색 (search)\n");
    printf("0 : EXIT\n");
    printf("input :");
    scanf("%d", &func);
    if (func == 0)
        exit(0);

    printf("\nSelect Number\n");
    printf("6 : 허용 명령어 목록 (WHITELIST t)\n");
    printf("7 : 공개키, MAC (MAC t)\n");
    printf("8 : id(사원번호), 접근권한, 비밀번호 (ADMIN t)\n");
    printf("9 : 사원 정보 - 이름, 생년월일, 이메일, 전화번호 (INFO t)\n");
    printf("0 : EXIT\n");
    printf("input : ");
    scanf("%d", &tb);
    menu =  func*10 + tb;

    while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
        putchar(tmp);
    }

    switch(menu) {
        case 16: // ok
            insWhiteListTable();
            break;

        case 17:
            insMAC();
            break;

        case 18: // ok
            //insAdminInfo();
            insAdminTable();
            break;

        case 19: // ok
            //insAdminInfo();
            insInfoTable();
            break;

        case 26: // ok
            updateAdminwl();
            break;

        case 27:
            updateMac();
            break;

        case 28: // ok
            //updateAdmin();
            updateAdminTable();
            break;

        case 29: // ok
            //updateAdmin();
            updateInfoTable();
            break;

        case 36: // ok
            delWhiteList();
            break;

        case 37:
            delAdmin();
            break;

        case 38: // ok
            //delAdmin();
            delAdminTable();
            break;

        case 39: // ok
            //delAdmin();
            delInfoTable();
            break;

        case 46: // ok
            selWhitelist();
            break;

        case 47:
            selAdminInfo();
            break;

        case 48: // ok
            //selAdminInfo();
            selAdminTable();
            break;

        case 49: // ok
            //selAdminInfo();
            selInfoTable();
            break;

       default :
           break;
    }
}

void printWhiteListResult(struct WhiteListTable pr_wt) {
    struct WhiteListTable wt;
}

void printAdminResult(struct InsInfo pr_i) {
    struct InsInfo i;
}
