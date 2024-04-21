#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Barber.h"
#include "General.h"
#include "Date.h"
#include "time.h"
#include "Appointment.h"
#include "Schedule.h"
#include "Branch.h"
#include "List.h"
#include "def.h"
#include "macros.h"
int initBarber(Barber* pBarber)
{
	printf("Enter first name: \n");
	pBarber->firstName = (char*)malloc(STR_MAX_LEN);
	if(!pBarber->firstName)
	{
		return 0;
	}
	myGets(pBarber->firstName, STR_MAX_LEN);
	while(!containsJustLetters(pBarber->firstName))
	{
		printf("First name must contain just letters. Please enter a valid name: \n");
		myGets(pBarber->firstName, STR_MAX_LEN);
	}
	printf("Enter last name: \n");
	pBarber->lastName = (char*)malloc(STR_MAX_LEN);
	if(!pBarber->lastName)
	{
		free(pBarber->firstName);
		return 0;
	}
	myGets(pBarber->lastName, STR_MAX_LEN);
	while(!containsJustLetters(pBarber->lastName))
	{
		printf("Last name must contain just letters. Please enter a valid name: \n");
		myGets(pBarber->lastName, STR_MAX_LEN);
	}
	int age;
	printf("Enter age: \n");
	checkThatInputIsAPositiveNumber(&age);
	pBarber->age = age;
	L_init(&pBarber->schedule);
	return 1;

}

void printBarber(void* pBarberPtr)
{
	Barber* pBarber = *(Barber**)pBarberPtr;
	if (pBarber == NULL) {
		printf("Invalid barber pointer\n");
		return;
	}
	printf("Full Name: %s %s, Age: %d\n", pBarber->firstName, pBarber->lastName, pBarber->age);
}







NODE* printBarberScheduleInSpesificDate(Barber* pBarber, Date date, const int freeHours)
{
	NODE* pNode = &pBarber->schedule.head;
	while (pNode->next != NULL)
	{
		if (compareDates(&date, &((Schedule*)pNode->next->key)->date) == 0)
		{
			printf("\n\n");
			printAvailableAppointmentsInSpesificDate((Schedule*)pNode->next->key, date, freeHours);
			return pNode->next;
		}
		pNode = pNode->next;
	}
	//if the date is not found in the schedule we have to create a new NODE for this date
	Schedule* pSchedule = (Schedule*)malloc(sizeof(Schedule));
	if (!pSchedule)
	{
		return NULL;
	}
	initSchedule(pSchedule, pBarber, date);
	NODE* pNewNode = L_insert(&pBarber->schedule.head, pSchedule);
	printf("\n\n");
	printAvailableAppointmentsInSpesificDate(pSchedule, date, freeHours);
	printf("\n\n");
	return pNewNode;
}



int addAppointmentToSchedule(Barber* pBarber)
{
	Date date;
	int hour;
	Client client;
	Appointment pAppoint;

	// Get the correct date
	getCorrectDate(&date);

	// Print the barber's schedule for the specific date
	NODE* barberScheduleInDate = printBarberScheduleInSpesificDate(pBarber, date, 1);

	// Prompt user to enter the hour of the appointment
	printf("\nOur hours: 09:00-20:00 (last appointment is between 19:00 - 20:00)\n");
	printf("Enter appointment hour (9-19) without ':': ");
	printf("For example, for 16:00 PM, enter 16, Or for exit enter -1\n");


	// Input validation loop for the appointment hour
	while (1)
	{
		if (scanf("%d", &hour) != 1 || getchar() != '\n')
		{
			// Clear input buffer
			while (getchar() != '\n');
			printf("Invalid input. Please enter only a number.\n");
			continue;
		}

		if (hour == -1) // Added condition to return 0 if hour equals -1
			return 0;

		if (hour < OPENING_HOUR || hour > CLOSING_HOUR)
		{
			printf("The opening hours of the branch are between 09:00 and 19:00\n");
			continue;
		}

		if (hourIsOccupied(pBarber, date, hour))
		{
			printf("This hour is already occupied. Please choose another hour:\n");
			continue;
		}

		// Check if the hour has already passed for the current date
		time_t currentTime = time(NULL);
		struct tm* currentLocalTime = localtime(&currentTime);
		if (currentLocalTime->tm_year == date.year - 1900 && currentLocalTime->tm_mon == date.month - 1 && currentLocalTime->tm_mday == date.day && currentLocalTime->tm_hour > hour)
		{
			printf("This hour has already passed for today. Please choose another hour.\n");
			continue;
		}
		break;
	}

	// Initialize the client
	initClient(&client);

	// Initialize the appointment
	initAppointment(&pAppoint, hour, client);

	// Additional logic to check appointment availability can be added here

	// Assign the appointment to the schedule
	((Schedule*)barberScheduleInDate->key)->appointments[hour - OPENING_HOUR] = pAppoint;
	return 1;
}





void cancelAppointmentFromSchedule(Barber* pBarber)
{
	Date date;
	int hour;

	getCorrectDate(&date);

	if(!dateInSchedule(pBarber, date))
	{
		printf("\nThere are no appointments for this date.\n");
		return;
	}

	NODE* barberScheduleInDate = printBarberScheduleInSpesificDate(pBarber, date, 0);

	printf("\nPlease enter the hour of the appointment you want to cancel:\n");
	printf("Enter only the hour number (between 9-19) without ':'\n");
	printf("For example, enter 16 for 16:00\n\n");

	while (1)
	{
		if (scanf("%d", &hour) != 1 || getchar() != '\n')
		{
			// Clear input buffer
			while (getchar() != '\n');
			printf("Invalid input. Please enter only a number.\n");
			continue;
		}

		if (hour < OPENING_HOUR || hour > CLOSING_HOUR)
		{
			printf("Invalid hour. Please enter a valid hour between 9-19.\n");
			continue;
		}

		if (!hourIsOccupied(pBarber, date, hour))
		{
			printf("There is no appointment at this hour. Please choose another hour.\n");
			continue;
		}

		break; 
	}
	getInitialValues(&((Schedule*)barberScheduleInDate->key)->appointments[ARRLOCATION(hour)]);
	printf("Appointment canceled\n");


	if(!hasAppointments((Schedule*)barberScheduleInDate->key))
	{
		NODE* oneBeforeBarberScheduleInDate = &pBarber->schedule.head;
		while (oneBeforeBarberScheduleInDate->next != barberScheduleInDate)
		{
			oneBeforeBarberScheduleInDate = oneBeforeBarberScheduleInDate->next;
		}
		L_delete(oneBeforeBarberScheduleInDate, freeScheduleData);
	}
}


int hourIsOccupied(Barber* pBarber, const Date date, const int hour)
{
	NODE* pNode = &pBarber->schedule.head;
	while (pNode->next != NULL)
	{
		if (compareDates(&date, &((Schedule*)pNode->next->key)->date) == 0)
		{
			if (((Schedule*)pNode->next->key) ->appointments[hour - OPENING_HOUR].hour != 0) 
			{
				return 1;
			}
		}
		pNode = pNode->next;
	}
	return 0;
}

int saveBarberToTextFile(Barber* pBarber, FILE* file)
{
	fprintf(file, "%s\n", pBarber->firstName);
	fprintf(file, "%s\n", pBarber->lastName);
	fprintf(file, "%d\n", pBarber->age);
	int numOfSchedules = L_count(&pBarber->schedule);
	fprintf(file, "%d\n", numOfSchedules);
	LIST* pList = &pBarber->schedule;
	NODE* pNode = &pList->head;
	while (pNode->next != NULL)
	{
		saveScheduleToTextFile((Schedule*)pNode->next->key, file);
		pNode = pNode->next;
	}
	return 1;
}

int saveBarberToBinaryFile(Barber* pBarber, FILE* file)
{
	int firstNameSize = (int)strlen(pBarber->firstName) + 1;
	if (fwrite(&firstNameSize, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (fwrite(pBarber->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
	{
		return 0;
	}
	int lastNameSize = (int)strlen(pBarber->lastName) + 1;
	if (fwrite(&lastNameSize, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (fwrite(pBarber->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
	{
		return 0;
	}
	if(fwrite(&pBarber->age, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	int numOfSchedules = L_count(&pBarber->schedule);

	if(fwrite(&numOfSchedules, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	LIST* pList = &pBarber->schedule;
	NODE* pNode = &pList->head;
	while (pNode->next != NULL)
	{
		if (!saveScheduleToBinaryFile((Schedule*)pNode->next->key, file))
		{
			return 0;
		}
		pNode = pNode->next;
	}
	return 1;
}

int loadBarberFromTextFile(Barber* pBarber, FILE* file)
{
	pBarber->firstName = (char*)malloc(STR_MAX_LEN);
	if(!pBarber->firstName)
	{
		return 0;
	}
	if(!readString(pBarber->firstName, file))
	{
		free(pBarber->firstName);
		return 0;
	}
	pBarber->lastName = (char*)malloc(STR_MAX_LEN);
	if(!pBarber->lastName)
	{
		free(pBarber->firstName);
		return 0;
	}
	if(!readString(pBarber->lastName, file))
	{
		free(pBarber->firstName);
		free(pBarber->lastName);
		return 0;
	}
	if(fscanf(file, "%d\n", &pBarber->age) != 1)
	{
		free(pBarber->firstName);
		free(pBarber->lastName);
		return 0;
	}
	int numOfSchedules;
	if(fscanf(file, "%d\n", &numOfSchedules) != 1)
	{
		free(pBarber->firstName);
		free(pBarber->lastName);
		return 0;
	}
	L_init(&pBarber->schedule);
	for(int i=0; i<numOfSchedules; i++)
	{
		Schedule* pSchedule = (Schedule*)malloc(sizeof(Schedule));
		if(!pSchedule)
		{
			free(pBarber->firstName);
			free(pBarber->lastName);
			L_free(&pBarber->schedule, freeScheduleData);
			return 0;
		}
		if (!loadScheduleFromTextFile(pSchedule, file, pBarber))
		{
			return 0;
		}
		L_insert(&pBarber->schedule.head, pSchedule);
	}
	return 1;
}

int loadBarberFromBinaryFile(Barber* pBarber, FILE* file)
{
	int firstNameSize;
	if (fread(&firstNameSize, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	pBarber->firstName = (char*)malloc(firstNameSize);
	if(!pBarber->firstName)
	{
		return 0;
	}
	if (fread(pBarber->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
	{
		return 0;
	}
	int lastNameSize;
	if (fread(&lastNameSize, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	pBarber->lastName = (char*)malloc(lastNameSize);
	if(!pBarber->lastName)
	{
		free(pBarber->firstName);
		return 0;
	}
	if (fread(pBarber->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
	{
		return 0;
	}
	if(fread(&pBarber->age, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	int numOfSchedules;

	if(fread(&numOfSchedules, sizeof(int), 1, file) != 1)
	{
		return 0;
	}


	LIST pList;
	L_init(&pList);
	NODE* pNode = &pList.head;

	for(int i=0; i<numOfSchedules; i++)
	{
		Schedule* pSchedule = (Schedule*)malloc(sizeof(Schedule));
		if(!pSchedule)
		{
			free(pBarber->firstName);
			free(pBarber->lastName);
			L_free(&pList, freeScheduleData);

			return 0;
		}
		if (!loadScheduleFromBinaryFile(pSchedule, file, pBarber))
		{
			return 0;
		}
		pNode = L_insert(pNode, pSchedule);
	}
	pBarber->schedule = pList;
	return 1;
}

int printBusyDates(Barber* pBarber)
{
	NODE* pNode = &pBarber->schedule.head;
	if(pNode->next == NULL)
	{
		printf("The barber has no appointments at all\n");
		return 0;
	}
	printf("The barber has appointments at the following dates:\n");
	while (pNode->next != NULL)
	{
		printDate(&((Schedule*)pNode->next->key)->date);
		printf("\n");
		pNode = pNode->next;
	}
	return 1;
}

int dateInSchedule(Barber* pBarber, const Date date)
{
	NODE* pNode = &pBarber->schedule.head;
	while (pNode->next != NULL)
	{
		if (compareDates(&date, &((Schedule*)pNode->next->key)->date) == 0)
		{
			return 1;
		}
		pNode = pNode->next;
	}
	return 0;
}

void freeSchedule(Barber* pBarber)
{
	LIST* pList = &pBarber->schedule;
	L_free(pList, freeScheduleData);
}

void freeBarber(void* pBarber)
{
	Barber* barber = *(Barber**)pBarber;
	freeSchedule(barber);
	free(barber->firstName);
	free(barber->lastName);
	free(barber);
}

int compareDates(const Date* date1, const Date* date2)
{
	if (date1->year != date2->year)
	{
		return date1->year - date2->year;
	}
	if (date1->month != date2->month)
	{
		return date1->month - date2->month;
	}
	return date1->day - date2->day;
}

