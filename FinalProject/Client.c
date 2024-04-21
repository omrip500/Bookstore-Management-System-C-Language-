#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Client.h"
#include "General.h"
#include "KitchenWorker.h"



int initClient(Client* client)
{
    printf("Enter the client's id (up to 9 digits): ");
    myGets(client->id, STR_MAX_LEN);
    while (!checkID(client->id))
    {
        printf("Enter the client's id (up to 9 digits): ");
        myGets(client->id, STR_MAX_LEN);
    }

    // Now, copy the validated ID into the Client structure

    printf("Enter the client's first name: \n");
    client->firstName = (char*)malloc(STR_MAX_LEN * sizeof(char));
    if(!client->firstName)
	{
		return 0;
	}
    myGets(client->firstName, STR_MAX_LEN);
    while(!containsJustLetters(client->firstName))
	{
        printf("First name must contain only letters.\n");
		printf("Enter the client's first name: \n");
		myGets(client->firstName, STR_MAX_LEN);
	}

    printf("Enter the client's last name: \n");
    client->lastName = (char*)malloc(STR_MAX_LEN * sizeof(char));
    if (!client->lastName)
    {
        free(client->firstName);
		return 0;
    }
    myGets(client->lastName, STR_MAX_LEN);
    while (!containsJustLetters(client->lastName))
    {
        printf("Last name must contain only letters.\n");
        printf("Enter the client's last name: \n");
		myGets(client->lastName, STR_MAX_LEN);
    }
    return 1;
}


void printClient(const Client* client)
{
	printf("Client Information: ");
	printf("ID: %s, First Name: %s, Last Name: %s\n", client->id, client->firstName, client->lastName);
}

int saveClientToTextFile(const Client* client, FILE* file)
{
    fprintf(file, "%s\n", client->id);
    fprintf(file, "%s\n", client->firstName);
    fprintf(file, "%s\n", client->lastName);
    return 1;
}

int readClientFromTextFile(Client* client, FILE* file)
{
    if (!readString(client->id, file))
		return 0;
    client->firstName = malloc(STR_MAX_LEN * sizeof(char));
	if (!readString(client->firstName, file))
		return 0;
    client->lastName = malloc(STR_MAX_LEN * sizeof(char));
	if (!readString(client->lastName, file))
		return 0;
	return 1;
}


int writeClientToBinaryFileCompressed(const Client* client, FILE* file)
{
    int idSize = (int)strlen(client->id);
    int firstNameSize = (int)strlen(client->firstName);
    int lastNameSize = (int)strlen(client->lastName);

    BYTE b[2];
    b[0] = (idSize << 4) | (firstNameSize >> 1);
    b[1] = (firstNameSize << 7) | (lastNameSize << 2);

    if (fwrite(b, sizeof(BYTE), 2, file) != 2)
        return 0;

    if (fwrite(client->id, sizeof(char), idSize, file) != idSize)
        return 0;

    if (fwrite(client->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
        return 0;

    if (fwrite(client->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
        return 0;
    
    return 1;

}

int readClientFromBinaryFileCompressed(Client* client, FILE* file)
{
    BYTE b[2];
	if (fread(b, sizeof(BYTE), 2, file) != 2)
		return 0;

	int idSize = b[0] >> 4;
	int firstNameSize = ((b[0] & 0x0F) << 1) | (b[1] >> 7);
	int lastNameSize = (b[1] & 0x7C) >> 2;

    if (fread(client->id, sizeof(char), idSize, file) != idSize)
        return 0;

    client->id[idSize] = '\0';

    client->firstName = (char*)calloc((firstNameSize + 1), sizeof(char));
    if (!client->firstName)
	{
		return 0;
	}
    if (fread(client->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
		return 0;
    client->lastName = (char*)calloc((lastNameSize + 1), sizeof(char));
    if (!client->lastName)
        return 0;
    if (fread(client->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
        return 0;

	return 1;
}



void freeClient(Client* Client)
{
    if(strcmp(Client->id, "-1") != 0)
    {
	    free(Client->firstName);
	    free(Client->lastName);
    }
}