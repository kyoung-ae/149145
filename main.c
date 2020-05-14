#define _CTR_SECURE_NO_WARNINGS // prevent compilation errors due to scanf security warnings

#include <stdio.h>
#include <sqlite3.h>
#include "BaseDefine.h"
#include "DBCreate.h"
#include "DBPrintModule.h"
#include "DB.h"

// DBProgram.c 로 파일명을 바꿔야 함 (DB 확인을 위해 main.c 로 작성한 상태임)
int main(int argc, char *argv[]) { // int selMenu(int menu) 로 함수명을 바꿔야 함 (DB 확인을 위해 main 함수로 작성한 상태임)
    int menu;

    createDB(); //DB와 TABLE 생성 함수이므로 최초 1번만 호출해야 함.
    printf("\n");

    printMenu(menu);
    // 삽입, 삭제, 업데이트 등의 작업 후에는 sql 문으로 VACUUM; 명령을 실행해야 최적화됨(윈도우의 디스크 조각모음 같은 기능)
    return 0;
}