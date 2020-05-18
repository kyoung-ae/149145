#ifndef DBManage_H
#define DBManage_H

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void removeEnter();
char *dateNow(struct tm *t);
char *secuBOF(char str[], int strsize);
int inADMIN();
int inMAC();
int inINFO();
int inWL();
int upADMIN();
int upMAC();
int upINFO();
int upWL();
int delADMIN();
int delMAC();
int delINFO();
int delWL();
int selADMIN();
int selMAC();
int selINFO();
int selWL();

#endif
