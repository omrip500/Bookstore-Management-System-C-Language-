#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Schedule.h"
#include "list.h"
#include "Appointment.h"
#include "Branch.h"
#include "macros.h"


//maybe we should find another solution instead of using memset
int initSchedule(Schedule* pSchedule, Barber* pBarber, Date date)
{
	pSchedule->appointments = (Appointment*)malloc((MAX_APPOINTMENTS + 1) * sizeof(Appointment));
	if (!pSchedule->appointments)
	{
		printf("Memory allocation failed\n");
		return 0;
	}
	pSchedule->date = date;

	for (int i = 0; i < MAX_APPOINTMENTS + 1; i++)
	{
		getInitialValues(&pSchedule->appointments[i]);
	}

	return 1;
}


void printOccupiedAppointments(const Schedule* pSchedule)
{
	for (int i = 0; i < MAX_APPOINTMENTS; i++)
	{
		if (pSchedule->appointments[i].hour != 0)
		{
			printAppointment(&pSchedule->appointments[i]);
		}
	}
}

int saveScheduleToTextFile(const Schedule* pSchedule, FILE* file)
{
	if (writeDateToTextFile(&pSchedule->date, file) == 0)
	{
		return 0;
	}

	int numOfAppointments = getNumOfAppointments(pSchedule);
	if (fprintf(file, "%d\n", numOfAppointments) < 0)
	{
		return 0;
	}

	for (int i = 0; i < MAX_APPOINTMENTS + 1; i++)
	{

		if (!(EQUALS(pSchedule->appointments[i].hour, 0)))
		{
			if (writeAppointmentToTextFile(&pSchedule->appointments[i], file) == 0)
			{
				return 0;
			}
		}
	}
	return 1;

}


int saveScheduleToBinaryFile(const Schedule* pSchedule, FILE* file)
{
	if (writeDateToBinaryFile(&pSchedule->date, file) == 0)
	{
		return 0;
	}

	int numOfAppointments = getNumOfAppointments(pSchedule);
	if (fwrite(&numOfAppointments, sizeof(int), 1, file) != 1)
	{
		return 0;
	}


	for (int i = 0; i < MAX_APPOINTMENTS + 1; i++)
	{
		if (!(EQUALS(pSchedule->appointments[i].hour, 0)))
		{
			if (writeAppointmentToBinaryFile(&pSchedule->appointments[i], file) == 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

int loadScheduleFromTextFile(Schedule* pSchedule, FILE* file, Barber* pBarber)
{
	if (readDateFromTextFile(&pSchedule->date, file) == 0)
	{
		return 0;
	}
	pSchedule->appointments = (Appointment*)malloc((MAX_APPOINTMENTS + 1) * sizeof(Appointment));

	if (!pSchedule->appointments)
	{
		printf("Memory allocation failed\n");
		return 0;
	}

	int numOfAppointments;
	if (fscanf(file, "%d\n", &numOfAppointments) != 1)
	{
		return 0;
	}

	for (int i = 0; i < MAX_APPOINTMENTS + 1; i++)
	{
		getInitialValues(&pSchedule->appointments[i]);
	}

	for (int i = 0; i < numOfAppointments; i++)
	{
		Appointment* appointment = &pSchedule->appointments[MAX_APPOINTMENTS];


		if (readAppointmentFromTextFile(appointment, file) == 0)
		{
			freeAppointment(appointment);
			return 0;
		}

		pSchedule->appointments[ARRLOCATION(appointment->hour)] = *appointment;
	}
	getInitialValues(&pSchedule->appointments[MAX_APPOINTMENTS]);
	return 1;
}

int loadScheduleFromBinaryFile(Schedule* pSchedule, FILE* file, Barber* pBarber)
{
	if (readDateFromBinaryFile(&pSchedule->date, file) == 0)
	{
		return 0;
	}
	pSchedule->appointments = (Appointment*)malloc((MAX_APPOINTMENTS + 1) * sizeof(Appointment));

	if (!pSchedule->appointments)
	{
		printf("Memory allocation failed\n");
		return 0;
	}

	int numOfAppointments;
	if (fread(&numOfAppointments, sizeof(int), 1, file) != 1)
	{
		return 0;
	}


	for (int i = 0; i < MAX_APPOINTMENTS + 1; i++)
	{
		getInitialValues(&pSchedule->appointments[i]);
	}


	for (int i = 0; i < numOfAppointments; i++)
	{
		Appointment* appointment = &pSchedule->appointments[MAX_APPOINTMENTS];

		if (readAppointmentFromBinaryFile(appointment, file) == 0)
		{
			freeAppointment(appointment);
			return 0;
		}

		pSchedule->appointments[ARRLOCATION(appointment->hour)] = *appointment;

	}
	getInitialValues(&pSchedule->appointments[MAX_APPOINTMENTS]);
	return 1;
}

int getNumOfAppointments(const Schedule* pSchedule)
{
	int count = 0;
	for (int i = 0; i < MAX_APPOINTMENTS; i++)
	{
		if (pSchedule->appointments[i].hour != 0)
		{
			count++;
		}
	}
	return count;
}

int hasAppointments(const Schedule* pSchedule)
{
	for (int i = 0; i < MAX_APPOINTMENTS; i++)
	{
		if (pSchedule->appointments[i].hour != 0)
		{
			return 1;
		}
	}
	return 0;
}


void printAvailableAppointmentsInSpesificDate(const Schedule* pSchedule, Date date, int freeHours)
{

	if (freeHours == 1)
	{
		time_t t = time(NULL);
		struct tm currentTime = *localtime(&t);

		int currentDay = currentTime.tm_mday;
		int currentMonth = currentTime.tm_mon + 1;
		int currentYear = currentTime.tm_year + 1900;

		if(currentDay == date.day && currentMonth == date.month && currentYear == date.year)
		{
			if (currentTime.tm_hour >= CLOSING_HOUR)
			{
				printf("The branch is closed for today\n");
				return;
			}
			else
			{
				for (int i = currentTime.tm_hour - OPENING_HOUR; i <= 19 - OPENING_HOUR; i++)
				{
					if (pSchedule->appointments[i].hour == 0)
					{
						if (pSchedule->date.year == currentYear &&
							pSchedule->date.month == currentMonth &&
							pSchedule->date.day == currentDay)
						{
							printf("\nAvailable at %02d:00 on %d-%02d-%02d\n", i + OPENING_HOUR, date.year, date.month, date.day);
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < MAX_APPOINTMENTS; i++)
			{
				if (pSchedule->appointments[i].hour == 0)
				{
					if (pSchedule->date.year == date.year &&
						pSchedule->date.month == date.month &&
						pSchedule->date.day == date.day)
					{
						printf("\nAvailable at %02d:00 on %d-%02d-%02d\n", i + OPENING_HOUR, date.year, date.month, date.day);
					}
				}
			}
		}


		
	}



	else
	{
		int barberIsOccupiedInDate = 0;
		for (int i = 0; i < MAX_APPOINTMENTS; i++)
		{
			if (pSchedule->appointments[i].hour != 0)
			{
				if (pSchedule->date.year == date.year && pSchedule->date.month == date.month && pSchedule->date.day == date.day)
				{
					barberIsOccupiedInDate = 1;
					printf("\nOccupied at Hour: %d:00\n", i + OPENING_HOUR);
					printClient(&pSchedule->appointments[i].pClient);
				}
			}
		}
		if (!barberIsOccupiedInDate)
		{
			printf("The barber has not booked appointments on this date\n");
		}
	}
}

void freeScheduleData(Schedule* pSchedule)
{
	if (pSchedule == NULL) return;

	freeAppointmentsArray(pSchedule->appointments);
	free(pSchedule);
}

void freeAppointmentsArray(Appointment* pAppointments)
{
	if (pAppointments == NULL) return;

	generalArrayFunction(pAppointments, MAX_APPOINTMENTS, sizeof(Appointment), freeAppointment);
	free(pAppointments);
}