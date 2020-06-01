#ifndef DBManage_H
#define DBManage_H

static int callback(void *NotUsed, int argc, char **argv, char **azColName); // callback
void removeEnter(); // scanf()의 Enter key 제거
void removeSpace(char str_col[]); // str_col 문자열의 공백 제거
char *dateNow(struct tm *t); // date 가져오는 함수
void printBOF_gets(char str[], int strsize, int define_size); // gets() 버퍼오버플로우 방지 : 재입력 요구 메시지 출력
int checkDate(int date); // 입력받은 날짜의 유효성 검사-> 1이면 유효한 날짜 (0이면 없는 날짜임)
int checkIDPWD(char id[]); // 호출 직전에 입력한 id를 매개변수로 받아서 수정이나 삭제 시 id&pwd로 확인 (성공하면 1을 반환)
int checkID(char id[]); // 입력한 id를 매개변수로 받아서 등록된 id인지 확인 (성공하면 1을 반환)
int checkWL(char whitelist[]); // 입력한 whitelist를 매개변수로 받아서 등록된 whitelist인지 확인 (성공하면 1을 반환)

int inAD_INFO(); // DBInsertModule.c : case 16 ok
int inMAC(); // DBInsertModule.c : case 17 // 통신과 연결해야해서 작성 못함
int inWL(); // DBInsertModule.c : case 18 ok
int upAD_INFO(); // DBUpdateModule.c : case 26 ok
int upMAC(); // DBUpdateModule.c : case 27 // 통신과 연결해야해서 작성 못함
int search_INFO(); // DBSearchModule.c : case 36 // 관리자 정보를 기준으로 검색
int searchMAC(); // DBSearchModule.c : case 37 // 공개키와 맥을 기준으로 검색
int searchWL(); // DBSearchModule.c : case 38 // 화이트리스트를 기준으로 검색
int delAD_INFO(); // DBDeleteModule.c : case 46 // 아직 작성 못함
int delMAC(); //  DBDeleteModule.c : case 47 // 통신과 연결해야해서 작성 못함
int delWL(); //  DBDeleteModule.c : case 48  // 아직 작성 못함
int searchId(); // DBHintIdPwd.c : case 56 : id 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수 // 아직 작성 못함
int searchPwd(); // DBHintIdPwd.c : case 57 : pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수 // 아직 작성 못함
int search_ip(); // DBHintIdPwd.c : case 58 : id와 pwd 분실 시 지원 기능을 위한 관리자 정보를 호출하는 함수 // 작성 못함

#endif
