#define _CTR_SECURE_NO_WARNINGS // prevent compilation errors due to scanf security warnings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "BaseDefine.h"
#include "DBlen.h"

//DBManagement.c ~
struct WhiteListTable {
    char whitelist[WLlen];
    char id[IDlen];
    char date[DATElen];
};

struct MACTable {
    char id[IDlen];
    char public_key[PKlen];
    char mac0[MAClen];
    char mac1[MAClen];
    char mac2[MAClen];
};

struct AdminTable {
    char idp[IDlen];
    int access;
    char pwd[PWDlen];
};

struct InfoTable {
    char id[IDlen];
    int access;
    char name[NAMElen];
    char birth[BIRTHlen];
    char email[EMAILlen];
    char phone[PHONElen];
    char date[DATElen];
};

struct InsInfo {
    char id[IDlen];
    char pwd[PWDlen];
    char public_key[PKlen];
    char mac[MAClen];
    char name[NAMElen];
    char birth[BIRTHlen];
    char email[EMAILlen];
    char phone[PHONElen];
    char date[DATElen];
    int access;
};

enum Account {
    Robot, CPS, Root
};

int selWhitelist(const char sel_wl[WLlen], OUT struct WhiteListTable sel_wt) {
    char whitelist[WLlen];
    OUT struct WhiteListTable wt;
}

int selAdminInfo(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT struct InfoTable sel_it) {
    char id[IDlen];
    char pwd[PWDlen];
    OUT struct InfoTable it;
}

int selPublicKey(const char sel_id[], const char sel_pwd[], OUT char sel_pk[]) {
    char id[IDlen];
    char pwd[PWDlen];
    OUT char public_key[PKlen]; // OUT publickey를 반환함
}

int searchPWD(const char search_id[], const char seearch_pwd[]) {
    char id[IDlen];
    char pwd[PWDlen];
}

int delAdmin(const char del_id[], const char del_pwd[]) {
    char id[IDlen];
    char pwd[PWDlen];
}

int delWhiteList(char del_wl[WLlen]) {
    char whitelist[WLlen];
}

int insWhiteListTable(struct WhiteListTable ins_wt) {
    struct WhiteListTable wt;
}

int insAdminTable(struct InsInfo ins_at) {
    struct InsInfo at;
}

int updateAdmin(struct InsInfo up_a) {
    struct InsInfo a;
}

int updateAdminwl(struct WhiteListTable up_a) {
    struct WhiteListTable wt;
}

int insMAC(const char insmac_id[IDlen], const char isnmac_mac[MAClen]) {
    char id[IDlen];
    char mac[MAClen];
}

int updateMac(const char upmac_id[IDlen], const char upmac_mac[MAClen]) {
    char id[IDlen];
    char mac[MAClen];
}

int updatePWD(const char uppwd_id[IDlen], const char uppwd_pwd[PWDlen]) {
    char id[IDlen];
    char pwd[PWDlen];
}

int updateAccess(const char upacc_id[IDlen], int upacc_acc) {
    char id[IDlen];
    int access;
}

void sqlString(char str[PKlen]) {
    int i;
    char sql_str[PKlen];
}

int selMenu(int sel_menu) {
    int i;

    printf("1. WHITELIST DB\n");
    printf("2. ADMINISTRATOR DB\n");
    printf("0. EXIT\n");
    puts("input : ");
    gets(i);

    return i;
}
// ~DBManagement.c

//DBPrintModule.c~
void printMenu(int pr_menu) {
    int menu;
}

void printWhiteListResult(struct WhiteListTable pr_wt) {
    struct WhiteListTable wt;
}

void printAdminResult(struct InsInfo pr_i) {
    struct InsInfo i;
}
// ~DBPrintModule.c

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i]:"NULL");
    }
    printf("\n");

    return 0;
}

int main(int argc, char *argv[]) {
    sqlite3 *db;
    char *errmsg;
    sqlite3_stmt *res;
    int rc;
    char *sql;
    char input_sql[512];
    char whitelist[30] = { 0, };
    char id[9] = { 0, };
    char date[12];

    rc = sqlite3_open("WHITELIST.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_busy_timeout(db, 500); //db open시 timeout 500ms로 설정

    sql = "CREATE TABLE WHITELIST ("\
            "whitelist TEXT(30) PRIMARY KEY NOT NULL,"\
            "id TEXT(9) NOT NULL,"\
            "date TEXT(12));";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't create table : %s\n", sqlite3_errmsg(db));
        //return 1;
    }
    else {
        fprintf(stderr, "Created table successfully\n");
    }

    puts("WHITELIST TABLE's insert\n");
    puts("input whitelist:");
    gets(whitelist);
    puts("input id:");
    gets(id);

    fflush(stdin);
    strcpy(input_sql, "insert into WHITELIST values('");
    strcat(input_sql, whitelist); //whitelist
    strcat(input_sql, "','");
    strcat(input_sql, id); //id
    strcat(input_sql, "','");

    //sql = "select datetimes(strftime('%s', 'now'), 'unixepoch', 'localtime'))"; //time, date
    strcat(input_sql, date); //date
    strcat(input_sql, "');");
    printf("%s\n", input_sql);
    rc = sqlite3_exec(db, input_sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't input : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print input successfully\n");
    }

    sql = "select * from WHITELIST";
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't print schema : %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else {
        fprintf(stderr, "Print schema successfully\n");
    }

    sqlite3_close(db);

	return 0;
}
