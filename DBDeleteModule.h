#ifndef DBDeleteModule_H
#define DBDeleteModule_H

static int callback(void *NotUsed, int argc, char **argv, char **azColName); // callback
int delAD_INFO(); // case 36
int delMAC(); // case 47 // 통신과 연결해야해서 작성 못함
int delWL(); // case 48

#endif
