#ifndef DBManage_H
#define DBManage_H

static int callback(void *NotUsed, int argc, char **argv, char **azColName); // callback
int inAD_INFO(); // case 16 ok
int inMAC(); // case 17 // 통신과 연결해야해서 작성 못함
int inWL(); // case 18 ok

#endif
