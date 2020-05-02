#include <stdio.h>
#include "DB.h"

void printMenu(int pr_menu) {
    int tb = 0;
    int func = 0;
    int menu = 0;

    char tmp; //Enter Key remove

    printf("Select DB Table\n");
    printf("1 : WHITELIST DB Table\n");
    printf("2 : ADMINISTRATOR DB MAC Table\n");
    printf("3 : ADMINISTRATOR DB ADMIN Table\n");
    printf("4 : ADMINISTRATOR DB INFO Table\n");
    printf("0 : EXIT\n");
    printf("Table :");
    scanf("%d", &tb);

    printf("\nSelect Number\n");
    printf("6 : insert\n");
    printf("7 : update\n");
    printf("8 : delete\n");
    printf("9 : search\n");
    printf("0 : EXIT\n");
    printf("menu : ");
    scanf("%d", &func);
    menu =  tb*10 + func;

    while((tmp = getchar()) != '\n') { //엔터키 삭제 함수
        putchar(tmp);
    }

    switch(menu) {
        case 16: // ok
            insWhiteListTable();
            break;

        case 17: // ok
            updateAdminwl();
            break;

        case 18: // ok
            delWhiteList();
            break;

        case 19: // ok
            selWhitelist();
            break;

        case 26:
            insMAC();
            break;

        case 27:
            updateMac();
            break;

        case 28:
            delAdmin();
            break;

        case 29:
            selAdminInfo();
            break;

        case 36: //
            insAdminTable();
            break;

        case 37:
            updateAdmin();
            break;

        case 38:
            delAdmin();
            break;

        case 39:
            selAdminInfo();
            break;

        case 46:
            //insInfoTable();
            break;

        case 47:
            updateAdmin();
            break;

        case 48:
            delAdmin();
            break;

        case 49:
            selAdminInfo();
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
