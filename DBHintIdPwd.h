#ifndef DBHintIdPwd_H
#define DBHintIdPwd_H

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
int searchId(); // case 56 : id 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
int searchPwd(); // case 57 : pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수
int search_ip(); // case 58 : id, pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수

#endif
