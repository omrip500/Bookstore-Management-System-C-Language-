#ifndef __BARBER__
#define __BARBER__

#include "list.h"
#include "Date.h"

#define OPENING_HOUR 9
#define CLOSING_HOUR 19

typedef struct
{
	char* firstName;
	char* lastName;
	int age;
	LIST schedule;
}Barber;

int initBarber(Barber* pBarber);
void printBarber(void* pBarberPtr);
int compareDates(const Date* date1, const Date* date2);
NODE* printBarberScheduleInSpesificDate(Barber* pBarber, Date date, const int freeHours);
int addAppointmentToSchedule(Barber* pBarber);
void cancelAppointmentFromSchedule(Barber* pBarber);
int hourIsOccupied(Barber* pBarber, const Date date, const int hour);
int saveBarberToTextFile(Barber* pBarber, FILE* file);
int saveBarberToBinaryFile(Barber* pBarber, FILE* file);
int loadBarberFromTextFile(Barber* pBarber, FILE* file);
int loadBarberFromBinaryFile(Barber* pBarber, FILE* file);
int printBusyDates(Barber* pBarber);
int dateInSchedule(Barber* pBarber, const Date date);
void freeSchedule(Barber* pBarber);
void freeBarber(void* pBarber);



#endif
