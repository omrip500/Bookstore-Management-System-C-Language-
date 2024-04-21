#include <stdio.h>
#include <stdlib.h>
#include "Appointment.h"
#include "Date.h"
#include <string.h>

void initAppointment(Appointment* appointment, int hour, Client pClient)
{
	appointment->hour = hour;
	appointment->pClient = pClient;
}


void printAppointment(const Appointment* appointment)
{
	printf("Appointment date: ");
	printf("Hour: %d\n", appointment->hour);
	printf("Barber: ");
	printf("Client: ");
	printClient(&appointment->pClient);
}

int writeAppointmentToTextFile(const Appointment* appointment, FILE* file)
{
	fprintf(file, "%d\n", appointment->hour);
	saveClientToTextFile(&appointment->pClient, file);
	return 1;
}

int readAppointmentFromTextFile(Appointment* appointment, FILE* file)
{

	if (fscanf(file, "%d\n", &appointment->hour) != 1)
	{
		return 0;
	}

	if(readClientFromTextFile(&appointment->pClient, file) == 0)
	{
		return 0;
	}
	return 1;
}

int writeAppointmentToBinaryFile(const Appointment* appointment, FILE* file)
{

	if (fwrite(&appointment->hour, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if(writeClientToBinaryFileCompressed(&appointment->pClient, file) == 0)
	{
		return 0;
	}
	return 1;
}

int readAppointmentFromBinaryFile(Appointment* appointment, FILE* file)
{
	if (fread(&appointment->hour, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (readClientFromBinaryFileCompressed(&appointment->pClient, file) == 0)
	{
		return 0;
	}
	return 1;
}

void getInitialValues(Appointment* appointment)
{
	appointment->hour = 0;
	appointment->pClient.firstName = NULL;
	appointment->pClient.lastName = NULL;
	strcpy(appointment->pClient.id, "-1");
}

void freeAppointment(void* appointment)
{
	Appointment* pAppointment = (Appointment*)appointment;
	freeClient(&pAppointment->pClient);
}
