#include "BaseDefine.h"
#include "DB.h"

#ifndef _DBMANAGEMENT_H_
#define _DBMANAGEMENT_H_

static int callback(void* NotUsed, int argc, char** argv, char** azColName);
int insWhiteListTable(struct WhiteListTable ins_wt); // 화이트리스트 등록 // case 16
int insAdminInfo(struct InsInfo ins_ai); // 관라자 정보 등록 // case 17
int insMAC(const char insmac_id[IDlen], const char isnmac_mac[MAClen]); // 맥 등록 // case 18
int updateAdminInfo(struct InsInfo up_a); // 관리자 정보 수정 // case 26
int updateMac(const char upmac_id[IDlen], const char upmac_mac[MAClen]); // 맥 수정 // case 27
int selWhitelist(/*const*/ char whitelist[]/*, OUT struct WhiteListTable sel_wt*/); // 화이트리스트 검색 // case 36
int selAdminInfo(const char id[], const char pwd[]/*, OUT struct InfoTable sel_it*/); // 관리자 정보 검색 // case 37
int searchPWD(const char search_id[IDlen], const char seearch_pwd[PWDlen]); // 비번 검색 // case 38
int selPublicKey(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT char sel_pk[PKlen]); // 공개키 검색 // case 39
int delWhiteList(char del_wl[WLlen]); // 화이트리스트 삭제 // case 46
int delAdmin(const char del_id[IDlen], const char del_pwd[PWDlen]); // 관리자 삭제 // case 47

#endif
