#ifndef __SCHEDULE__
#define __SCHEDULE__

#include "Appointment.h"
#define MAX_APPOINTMENTS 11
#define OPENING_HOUR 9


typedef struct
{
	Date date;
	Appointment* appointments;
}Schedule;

int initSchedule(Schedule* pSchedule, Barber* pBarber, Date date);
void printAvailableAppointmentsInSpesificDate(const Schedule* pSchedule, Date date, int freeHours);
void printOccupiedAppointments(const Schedule* pSchedule);
int saveScheduleToTextFile(const Schedule* pSchedule, FILE* file);
int saveScheduleToBinaryFile(const Schedule* pSchedule, FILE* file);
int loadScheduleFromTextFile(Schedule* pSchedule, FILE* file, Barber* pBarber);
int loadScheduleFromBinaryFile(Schedule* pSchedule, FILE* file, Barber* pBarber);
int getNumOfAppointments(const Schedule* pSchedule);
int hasAppointments(const Schedule* pSchedule);
void freeScheduleData(Schedule* pSchedule);
void freeAppointmentsArray(Appointment* pAppointments);

#endif
