#ifndef DB_H
#define DB_H

// #define len 크기 지정은 실제크기+1 로 지정
#define SQLlen 512 // input_sql
#define ACCESSlen 2 // int형의 access값을 문자로 받기 위한 변수 크기 -> 크기를 1로 했더니 id[0]을 null로 덮어쓰는 참사 발생 ㅠㅠ
// access 변수를 int 형에서 2020년 5월 19일 20시경, 문자열로 변경하기로 팀에서 결정함.
#define WLlen 31
#define IDlen 2 // 편의상 짧게 변경하고 작업 중... 원래는 10으로 기록해야 함.
#define DATElen 21
#define PKlen 271
#define MAClen 18
#define PWDlen 3 // 편의상 짧게 변항하고 작업 중... 원래는 514로 기록해야 함.
#define NAMElen 31
#define EMAILlen 51
#define PHONElen 21
#define BIRTHlen 9
#define MAX 1024 // 입력받는 임시 문자열

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
    char access[ACCESSlen];
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
    char access[ACCESSlen];
};

enum Account {
    Robot, CPS, Root
};

#endif
