#ifndef __GENERAL__
#define __GENERAL__

#define STR_MAX_LEN 255
typedef unsigned char BYTE;

char* getStrExactName(const char* msg);
char* myGets(char* buffer, int size);
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, int* pTotalLength);
int readString(char* str, FILE* fp);
int hasOnlyDigits(const char* str);
int checkID(const char* id);
int containsJustLetters(const char* str);
int checkThatInputIsAPositiveNumber(int* number);


#endif

