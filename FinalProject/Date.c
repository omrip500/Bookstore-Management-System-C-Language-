#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '#'


void getCorrectDate(Date* pDate)
{
    char date[STR_MAX_LEN];
    int ok = 1;

    // Getting current system time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentYear = tm.tm_year + 1900;
    int currentMonth = tm.tm_mon + 1;
    int currentDay = tm.tm_mday;

    do {
        printf("Enter appointment Date dd%c%cmm%c%cyyyy  minimum year %d\t",
            SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
        myGets(date, STR_MAX_LEN);
        ok = checkDate(date, pDate);

        // Check if the entered date is not earlier than today's date
        if (!ok || (pDate->year < currentYear) ||
            (pDate->year == currentYear && pDate->month < currentMonth) ||
            (pDate->year == currentYear && pDate->month == currentMonth && pDate->day < currentDay)) {
            printf("Error: The date cannot be earlier than today's date. Please try again.\n");
            ok = 0;
        }
    } while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ((date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		|| (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d", pDate->day, pDate->month, pDate->year);
}

int	writeDateToTextFile(const Date* pDate, FILE* file)
{
	fprintf(file, "%d\n", pDate->day);
	fprintf(file, "%d\n", pDate->month);
	fprintf(file, "%d\n", pDate->year);
	return 1;
}

int	writeDateToBinaryFile(const Date* pDate, FILE* file)
{
    if(fwrite(&pDate->day, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
    if(fwrite(&pDate->month, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if(fwrite(&pDate->year, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	return 1;
}

int	readDateFromTextFile(Date* pDate, FILE* file)
{
	if(fscanf(file, "%d\n", &pDate->day) != 1)
	{
		return 0;
	}
	if(fscanf(file, "%d\n", &pDate->month) != 1)
	{
		return 0;
	}
	if(fscanf(file, "%d\n", &pDate->year) != 1)
	{
		return 0;
	}
	return 1;
}


int	readDateFromBinaryFile(Date* pDate, FILE* file)
{
	if(fread(&pDate->day, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if(fread(&pDate->month, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if(fread(&pDate->year, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	return 1;
}

