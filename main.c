#define _CTR_SECURE_NO_WARNINGS

#pragma foreign_keys = 1 // 참조키 활성화

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio_ext.h>

#include "sqlite3.h"
#include "DBCreate.h"
#include "DBPrintModule.h"
#include "DBSearchModule.h"
#include "DBManage.h"
#include "DBManagement.h"
#include "DBInsertModule.h"
#include "DBUpdateModule.h"
#include "DBDeleteModule.h"
#include "DBlen.h"
#include "DB.h"
#include "DBLogin.h"
#include "DBBackupR.h"
#include "BaseDefine.h"
#include "DBProgram.h"
#include "DBd.h"
#include "DBins.h"
#include "DBs.h"
#include "DBup.h"

//DBProgram.c로 파일명을 변경해야 함.
int main(int argc, char *argv[]) { // int selMenu() 로 함수명을 변경해야 함.
    createDB(); // DB 와 Table 생성 함수 // DBCreate.c
    if(login() == 1) { // DB 로그인 : id & pwd로 확인
        printf("\n");
        printMenu(); // DBPrintModule.c
    // 삽입, 삭제, 업데이트 등의 작업 후에는 sql 문으로 VACUM; 명령을 실행해야 최적화됨
    //system("echo '.dump' | sqlite CPS.db");
    }
    return 0;
}
//system("sqlite3 ex.db.dump > ex.sql");
//system("sqlite3 ex.db < ex.sql");
