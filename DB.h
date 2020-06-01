#ifndef DB_H
#define DB_H

#define _CTR_SECURE_NO_WARNINGS

#pragma foreign_keys = 1 // 참조키 활성화

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "BaseDefine.h"
#include "sqlite3.h"
#include "DBCreate.h"
#include "DBPrintModule.h"
#include "DBSearchModule.h"
#include "DBManage.h"
#include "DBManagement.h"
#include "DBProgram.h"
#include "DBInsertModule.h"
#include "DBUpdateModule.h"
#include "DBDeleteModule.h"
#include "DBHintIdPwd.h"

// #define len 크기 지정은 실제크기+1 로 지정
#define SQLlen 800 // 입력 길이 제한이 가장 긴 PWDlen(514) 를 포함하는 admin 테이블을 기준으로 설정함
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
#define BIRTHlen 9
#define MAX 1024 // 입력받는 임시 문자열
#define COLlen 9 // 검색 항목 선택 문자열 길이
#define COLNAMElen 12

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
    char access[ACCESSlen]; // N, E, C, R만 입력 가능함
    // N은 아무 권한 없음(권한 부여 전의 default), E는 EV3 Robot, C는 CPS, R은 Root 를 나타냄
};

#endif
