#ifndef DBManagement_H
#define DBManagement_H

#include "DB.h"
#include "BaseDefine.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName); // callback
char *dateNow(struct tm *t); // date 가져오는 함수
int openDB(); // CPS.DB open
int selWhitelist(const char sel_wl[WLlen], OUT struct WhiteListTable sel_wt); // case 46
int selAdminInfo(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT struct InfoTable sel_it); // case 48 & 49 참고
int selPublicKey(const char sel_id[IDlen], const char sel_pwd[PWDlen], OUT char sel_pk[PKlen]);
int searchPWD(const char search_id[IDlen], const char seearch_pwd[PWDlen]); // case 48 참고 // pwd 인증:pwd를 알고 있는 경우
int delAdmin(const char del_id[IDlen], const char del_pwd[PWDlen]); // case 38 & 39 참고
int delWhiteList(char del_wl[WLlen]); // case 36
int insWhiteListTable(struct WhiteListTable ins_wt); // case 16
int insAdminInfo(struct InsInfo ins_ai); // case 18 & 19 참고
int insAdminTable(struct AdminTable ins_at); // case 18
int insInfoTable(struct InfoTable ins_it); // case 19
int updateAdmin(struct InsInfo up_a); // case 28 & 29 참고
int updateInfoTable(struct InfoTable up_it); // case 29
int updateAdminTable(struct AdminTable up_at); // case 28
int updateAdminwl(struct WhiteListTable up_w); // case 26
int insMAC(const char insmac_id[IDlen], const char insmac_mac[MAClen]);
int updateMac(const char upmac_id[IDlen], const char upmac_mac[MAClen]);
int updatePWD(const char uppwd_id[IDlen], const char uppwd_pwd[PWDlen]);
int updateAccess(const char upacc_id[IDlen], int upacc_acc);
int delAdminTable(struct AdminTable del_at); // case 38
int selAdminTable(struct AdminTable sel_at); // case 48
int delInfoTable(struct InfoTable del_it); // case 39
int selInfoTable(struct InfoTable sel_it); // case 49

#endif
