#include <stdio.h>
#include "BaseDefine.h"
#include "DB.h"
#include <stdlib.h>
#include <string.h>
#include "DBManagement.h"
#include "DBManage.h"

#pragma foreign_keys = 1 // 참조키 활성화

void printMenu() {
    int tb = 0;
    int func = 0;
    int menu = 0;
    int col;

    printf("Select Number\n");
    printf("1 : 등록 insert\n");
    printf("2 : 수정 update\n");
    printf("3 : 검색 search\n");
    printf("4 : 삭제 delete\n");
    printf("5 : id, pwd (분실)찾기\n");
    printf("0 : 종료 EXIT\n");
    printf("input :");
    scanf("%d", &func);
    removeEnter();

    switch(func) { // 1, 2, 3, 4, 5를 제외한 수를 선택하면 종료
        case 0:
            exit(0);
            break;

        case 1: // insert 기능
            printf("\nSelect Number\n");
            printf("6 : id, 접근권한, pwd, 이름, 생년월일, 이메일, 전화번호 (=>관리자 정보)\n");
            printf("7 : 공개키, MAC (=>통신 정보)\n");
            printf("8 : 허용 명령어 목록 (=>Whitelist tbl)\n");
            printf("0 : 종료 EXIT\n");
            printf("input : ");
            scanf("%d", &tb);
            menu =  func*10 + tb;
            removeEnter();
            break;

        case 2: // update 기능
            printf("\nSelect Number\n");
            printf("6 : 접근권한, pwd, 이름, 생년월일, 이메일, 전화번호 (=>관리자 정보)\n");
            printf("7 : 공개키, MAC (=>통신 정보)\n");
            printf("0 : 종료 EXIT\n");
            printf("input : ");
            scanf("%d", &tb);
            menu =  func*10 + tb;
            removeEnter();
            break;

        case 3: // search 기능
            printf("\nSearch Number\n");
            printf("6 : id를 기준으로 검색\n");
            printf("7 : whitelist를 기준으로 검색\n");
            printf("0 : 종료 EXIT\n");
            printf("input : ");
            scanf("%d", &tb);
            menu =  func*10 + tb;
            removeEnter();
            break;

        case 4: // delete 기능
            printf("\nSelect Number\n");
            printf("6 : id, 접근권한, pwd, 이름, 생년월일, 이메일, 전화번호 (=>관리자 정보)\n");
            printf("7 : 공개키, MAC (=>통신 정보)\n");
            printf("8 : 허용 명령어 목록 (=>Whitelist tbl)\n");
            printf("0 : 종료 EXIT\n");
            printf("input : ");
            scanf("%d", &tb);
            menu =  func*10 + tb;
            removeEnter();
            break;

        case 5: // id, pwd 분실지원 찾기 기능
            printf("\nSelect Number\n");
            printf("6 : id (분실)찾기\n");
            printf("7 : pwd (분실)찾기\n");
            printf("8 : id와 pwd 둘 다 찾기\n");
            printf("0 : 종료 EXIT\n");
            printf("input : ");
            scanf("%d", &tb);
            menu =  func*10 + tb;
            removeEnter();
            break;

        default:
            printf("입력 값이 잘못되어 종료됩니다.\n");
            exit(0);
    }

    switch(menu) {
        case 10:
            exit(0);
            break;

        case 16:
            inAD_INFO();
            break;

        case 17:
            inMAC();
            break;

        case 18:
            inWL();
            break;

        case 20:
            exit(0);
            break;

        case 26:
            upAD_INFO();
            break;

        case 27:
            upMAC();
            break;

        case 30:
            exit(0);
            break;

        case 36:
            search_id();
            break;

        case 37:
            search_wl();
            break;

        case 40:
            exit(0);
            break;

        case 46:
            delAD_INFO();
            break;

        case 47:
            delMAC();
            break;

        case 48:
            delWL();
            break;

        case 50:
            exit(0);
            break;

        case 56:
            searchId(); // case 56 : id 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
            break;

        case 57:
            searchPwd();
            break;

        case 58:
            search_ip();
            break;

       default :
            printf("입력 값이 잘못되어 종료됩니다.\n");
    }
/*
    const char sel_wl[WLlen]; //OUT struct WhiteListTable sel_wt; // case 46
    const char sel_id[IDlen]; //OUT struct InfoTable sel_it; // case 48 & 49 참고
    const char sel_pwd[PWDlen]; //OUT char sel_pk[PKlen];
    const char search_id[IDlen], seearch_pwd[PWDlen]; // case 48 참고 // pwd 인증:pwd를 알고 있는 경우
    const char del_id[IDlen], del_pwd[PWDlen]; // case 38 & 39 참고
    char del_wl[WLlen]; // case 36
    struct WhiteListTable ins_wt; // case 16
    struct InsInfo ins_ai; // case 18 & 19 참고
    struct AdminTable ins_at; // case 18
    struct InfoTable ins_it; // case 19
    struct InsInfo up_a; // case 28 & 29 참고
    struct InfoTable up_it; // case 29
    struct AdminTable up_at; // case 28
    struct WhiteListTable up_w; // case 26
    const char insmac_id[IDlen], insmac_mac[MAClen];
    const char upmac_id[IDlen], upmac_mac[MAClen];
    const char uppwd_id[IDlen], uppwd_pwd[PWDlen];
    const char upacc_id[IDlen]; int upacc_acc;
    struct AdminTable del_at; // case 38
    struct AdminTable sel_at; // case 48
    struct InfoTable del_it; // case 39
    struct InfoTable sel_it; // case 49

    printf("\nSelect Number\n");
    printf("6 : 허용 명령어 목록 (Whitelist tbl)\n");
    printf("7 : 공개키, MAC (MAC tbl)\n");
    printf("8 : id(사원번호), 접근권한, 비밀번호 (Admin tbl)\n");
    printf("9 : 사원 정보 - 이름, 생년월일, 이메일, 전화번호 (Info tbl)\n");
    printf("0 : EXIT\n");
    printf("input : ");
    scanf("%d", &tb);
    menu =  func*10 + tb;

    while((tmp = getchar()) != '\n') { //EnterKey remove
        putchar(tmp);
    }

    switch(menu) {
        case 16:
            insWhiteListTable(ins_wt);
            break;

        case 17:
            insMAC(insmac_id[IDlen], insmac_mac[MAClen]);
            break;

        case 18:
            //insAdminInfo();
            insAdminTable(ins_at);
            break;

        case 19:
            //insAdminInfo();
            insInfoTable(ins_it);
            break;

        case 26:
            updateAdminwl(up_w);
            break;

        case 27:
            updateMac(upmac_id[IDlen], upmac_mac[MAClen]);
            break;

        case 28:
            //updateAdmin();
            updateAdminTable(up_at);
            break;

        case 29:
            //updateAdmin();
            updateInfoTable(up_it);
            break;

        case 36:
            delWhiteList(del_wl[WLlen]);
            break;

        case 37:
            //delAdmin(del_at);
            break;

        case 38:
            //delAdmin();
            delAdminTable(del_at);
            break;

        case 39:
            //delAdmin();
            delInfoTable(del_it);
            break;

        case 46:
            //selWhitelist(sel_wl[WLlen], sel_wt);
            break;

        case 47:
            //selAdminInfo();
            break;

        case 48:
            //selAdminInfo();
            selAdminTable(sel_at);
            break;

        case 49:
            //selAdminInfo();
            selInfoTable(sel_it);
            break;

       default :
           break;
    }
    */
}

void printWhiteListResult(struct WhiteListTable pr_wt) {
    struct WhiteListTable wt;
}

void printAdminResult(struct InsInfo pr_i) {
    struct InsInfo i;
}
