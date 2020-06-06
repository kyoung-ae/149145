#include "BaseDefine.h"
#include "DBlen.h"

#ifndef DB_H
#define DB_H

struct WhiteListTable {
    char whitelist[WLlen];
    char id[IDlen];
    char W_date[DATElen];
};

struct MACTable {
    char id[IDlen];
    char public_key[PKlen];
    char mac0[MAClen];
    char mac1[MAClen];
    char mac2[MAClen];
};

struct AdminTable {
    char id[IDlen];
    char access[ACCESSlen];
    char pwd[PWDlen];
};

struct InfoTable {
    char id[IDlen];
    char name[NAMElen];
    char birth[BIRTHlen];
    char email[EMAILlen];
    char phone[PHONElen];
    char I_date[DATElen];
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
    char I_date[DATElen];
    char access[ACCESSlen]; // N, E, C, R만 입력 가능함
    // N은 아무 권한 없음(권한 부여 전의 default), E는 EV3 Robot, C는 CPS, R은 Root 를 나타냄
};

#endif
