#include "DB.h"

// DBProgram.c 로 파일명을 바꿔야함 (DB 확인을 이해 main.c 로 작성한 상태임)
int main(int argc, char *argv[]) {
// int selMenu(int menu) 로 함수명을 바꿔야함 (DB 확인을 위해 main 함수로 작성한 상태임)
// int selMenu() 로 함수명을 바꿔야함 (DB 확인을 위해 main 함수로 작성한 상태임)

    createDB(); // DB 와 Table 생성 함수 // DBCreate.c
    printf("\n");

    printMenu(); // DBPrintModule.c
    // 삽입, 삭제, 업데이트 등의 작업 후에는 sql 문으로 VACUM; 명령을 실행해야 최적화됨

    return 0;
}
