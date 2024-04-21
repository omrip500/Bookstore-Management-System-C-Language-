#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"




char* getStrExactName(const char* msg)
{
    char* str;
    char temp[STR_MAX_LEN];
    printf("%s\t", msg);
    myGets(temp, STR_MAX_LEN);

    str = getDynStr(temp);
    return str;
}

char* getDynStr(char* str)
{
    char* theStr;
    theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
    if (!theStr)
        return NULL;

    strcpy(theStr, str);
    return theStr;
}

char* myGets(char* buffer, int size)
{
    char* ok;
    if (buffer != NULL && size > 0)
    {
        do { //skip only '\n' strings
            ok = fgets(buffer, size, stdin);
        } while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
        if (ok)
        {
            char* back = buffer + strlen(buffer);
            //trim end spaces
            while ((buffer < back) && (isspace(*--back)));
            *(back + 1) = '\0';
            return buffer;
        }
        buffer[0] = '\0';
    }
    return NULL;
}



char** splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
    char temp[255];
    char* delimiters = " ";
    char* word;
    int count = 0;

    strcpy(temp, str);
    char** wordsArray = NULL;
    *pTotalLength = 0;

    word = strtok(temp, delimiters);
    while (word != NULL)
    {
        char** tempArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
        if (!tempArray) {
            // Free memory allocated so far before returning
            for (int i = 0; i < count; ++i)
                free(wordsArray[i]);
            free(wordsArray);
            return NULL;
        }
        wordsArray = tempArray;
        wordsArray[count] = getDynStr(word);
        if (!wordsArray[count]) {
            // Free memory allocated so far before returning
            for (int i = 0; i < count; ++i)
                free(wordsArray[i]);
            free(wordsArray);
            return NULL;
        }
        count++;
        *pTotalLength += (int)strlen(word);
        word = strtok(NULL, delimiters);
    }
    *pCount = count;
    return wordsArray;
}

int readString(char* str, FILE* fp)
{
    if (fgets(str, STR_MAX_LEN, fp) == NULL) {
        return 0; // Failure - return 0
    }

    // Remove the trailing newline character if it exists
    char* newline = strchr(str, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    // Trim leading and trailing whitespaces
    char* start = str;
    while (*start && isspace(*start)) {
        ++start;
    }

    // Trim trailing whitespaces from the end
    char* end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) {
        --end;
    }
    *(end + 1) = '\0';

    return 1; // Success - return 1
}

int hasOnlyDigits(const char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (!isdigit(str[i])) {
            return 0;
        }
        i++;
    }

    if (str[i] == '\0') {
        return 1;
    }
    return 0;
}

int checkID(const char* id)
{
     // Check if the ID consists of digits only and has a length less than or equal to 9
    int idLen = (int)strlen(id);

    if (idLen > 9)
    {
        printf("You entered more than 9 digits.\n");
        return 0;
    }

    if(!hasOnlyDigits(id))
	{
		printf("ID must contain only digits.\n");
		return 0;
	}
    return 1;
}

int checkThatInputIsAPositiveNumber(int* number)
{
    int numberStatus = scanf("%d", number);

    while (!numberStatus || *number < 0)
    {
        if (!numberStatus)
        {
            printf("input must be a number\n");
            printf("Enter number again: ");
            while (getchar() != '\n');
            numberStatus = scanf("%d", number);
            continue;
        }

        if (*number < 0)
        {
            printf("Number must be positive\n");
            printf("Enter number again: ");
            numberStatus = scanf("%d", number);
        }
    }
    return 1;
}

int containsJustLetters(const char* str)
{
    int i = 0;
	while (str[i] != '\0')
	{
		if (!isspace(str[i]) && !isalpha(str[i]))
        {
			return 0;
		}
		i++;
	}
	return 1;
}


