#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2024

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
int		writeDateToTextFile(const Date* pDate, FILE* file);
int		writeDateToBinaryFile(const Date* pDate, FILE* file);
int		readDateFromTextFile(Date* pDate, FILE* file);
int		readDateFromBinaryFile(Date* pDate, FILE* file);

#endif
