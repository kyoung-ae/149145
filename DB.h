#ifndef DB_H
#define DB_H

#define SQLlen 512 // input_sql
#define ACCESSlen 2 // int형의 access값을 문자로 받기 위한 변수 크기 -> 크기를 1로 했더니 id[0]을 null로 덮어쓰는 참사 발생 ㅠㅠ
#define WLlen 31
#define IDlen 10
#define DATElen 21
#define PKlen 271
#define MAClen 18
#define PWDlen 514
#define NAMElen 31
#define EMAILlen 51
#define PHONElen 21
#define BIRTHlen 7

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

#endif
