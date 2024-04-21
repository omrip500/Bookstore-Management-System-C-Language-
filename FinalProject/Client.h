#ifndef __CLIENT__
#define __CLIENT__
#include "General.h"


typedef struct
{
	char id[STR_MAX_LEN]; 
	char* firstName; 
	char* lastName; 
}Client;

int initClient(Client* Client);
void printClient(const Client* client);
int saveClientToTextFile(const Client* client, FILE* file);
int readClientFromTextFile(Client* client, FILE* file);
int writeClientToBinaryFileCompressed(const Client* client, FILE* file);
int readClientFromBinaryFileCompressed(Client* client, FILE* file);
void freeClient(Client* Client);

#endif
