#ifndef __APPOINTMENT__
#define __APPOINTMENT__

#include "Barber.h"
#include "Client.h"
#include "Date.h"

typedef struct
{
	int hour;
	Client pClient;
}Appointment;

void initAppointment(Appointment* appointment, int hour, Client pClient);
void printAppointment(const Appointment* appointment);
int writeAppointmentToTextFile(const Appointment* appointment, FILE* file);
int readAppointmentFromTextFile(Appointment* appointment, FILE* file);
int writeAppointmentToBinaryFile(const Appointment* appointment, FILE* file);
int readAppointmentFromBinaryFile(Appointment* appointment, FILE* file);
void getInitialValues(Appointment* appointment);
void freeAppointment(void* appointment);


#endif
