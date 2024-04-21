#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kitchenWorker.h"
#include "branch.h"
#include "barberSalon.h"
#include "General.h"
#include "kitchen.h"


int initKitchenWorker(KitchenWorker* pKitchWorker)
{
	printf("Kitchen worker details: \n");
	KitchenWorker* kitchenWorker = pKitchWorker;
	printf("Enter first name: \n");
	kitchenWorker->firstName = (char*)malloc(STR_MAX_LEN);
	if(!kitchenWorker->firstName)
		return 0;
	myGets(pKitchWorker->firstName, STR_MAX_LEN);
	while (!containsJustLetters(kitchenWorker->firstName))
	{
		printf("First name must contain only letters. Please try again: \n");
		myGets(pKitchWorker->firstName, STR_MAX_LEN);
	}
	printf("Enter last name: \n");
	kitchenWorker->lastName = (char*)malloc(STR_MAX_LEN);
	if (!kitchenWorker->lastName)
	{
		free(kitchenWorker->firstName);
		return 0;
	}
	myGets(pKitchWorker->lastName, STR_MAX_LEN);
	while (!containsJustLetters(kitchenWorker->lastName))
	{
		printf("Last name must contain only letters. Please try again: \n");
		myGets(pKitchWorker->lastName, STR_MAX_LEN);
	}
	printf("Enter ID: \n");
	myGets(pKitchWorker->ID, STR_MAX_LEN);
	while (!checkID(pKitchWorker->ID))
	{
		printf("Enter the kitchen worker's id (up to 9 digits): ");
		myGets(pKitchWorker->ID, STR_MAX_LEN);
	}
	return 1;
}

void printKitchenWorker(const KitchenWorker* pKitchWorker)
{
	const char* firstName = pKitchWorker->firstName;
	const char* lastName = pKitchWorker->lastName;
	const char* ID = pKitchWorker->ID;

	printf("Kitchen Worker Details:\n");
	printf("------------------------\n");
	printf("First Name: %s\n", firstName);
	printf("Last Name: %s\n", lastName);
	printf("ID: %s\n", ID);
	printf("------------------------\n");
}

int saveKitchenWorkerToTextFile(const KitchenWorker* pKitchWorker, FILE* file)
{
	if (file == NULL)
	{
		return 0;
	}
	fprintf(file, "%s\n", pKitchWorker->ID);
	fprintf(file, "%s\n", pKitchWorker->firstName);
	fprintf(file, "%s\n", pKitchWorker->lastName);
	return 1;
}


int writeKitchenWorkerToBinaryFileCompressed(const KitchenWorker* pKitchWorker, FILE* file)
{
	int idSize = (int)strlen(pKitchWorker->ID);
	int firstNameSize = (int)strlen(pKitchWorker->firstName);
	int lastNameSize = (int)strlen(pKitchWorker->lastName);

	BYTE b[2];
	b[0] = (idSize << 4) | (firstNameSize >> 1);
	b[1] = (firstNameSize << 7) | (lastNameSize << 2);

	if (fwrite(b, sizeof(BYTE), 2, file) != 2)
		return 0;

	if (fwrite(pKitchWorker->ID, sizeof(char), idSize, file) != idSize)
		return 0;

	if (fwrite(pKitchWorker->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
		return 0;

	if (fwrite(pKitchWorker->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
		return 0;
	return 1;
}


int readKitchenWorkerFromBinaryFileCompressed(KitchenWorker* pKitchWorker, FILE* file)
{
	BYTE b[2];
	if (fread(b, sizeof(BYTE), 2, file) != 2)
		return 0;

	int idSize = b[0] >> 4;
	int firstNameSize = ((b[0] & 0x0F) << 1) | (b[1] >> 7);
	int lastNameSize = (b[1] & 0x7C) >> 2;


	if (fread(pKitchWorker->ID, sizeof(char), idSize, file) != idSize)
		return 0;
	pKitchWorker->ID[idSize] = '\0';

	pKitchWorker->firstName = (char*)calloc((firstNameSize + 1), sizeof(char));
	if (!pKitchWorker->firstName)
	{
		return 0;
	}
	if (fread(pKitchWorker->firstName, sizeof(char), firstNameSize, file) != firstNameSize)
		return 0;
	pKitchWorker->lastName = (char*)calloc((lastNameSize + 1), sizeof(char));
	if (!pKitchWorker->lastName)
		return 0;
	if (fread(pKitchWorker->lastName, sizeof(char), lastNameSize, file) != lastNameSize)
		return 0;

	return 1;
}

int readKitchenWorkerFromTextFile(KitchenWorker* pKitchWorker, FILE* file)
{
	if (file == NULL)
	{
		printf("Error: file is null\n");
		return 0;
	}
	if(!readString(pKitchWorker->ID, file))
	{
		printf("Error: reading ID\n");
		return 0;
	}
	pKitchWorker->firstName = (char*)malloc(STR_MAX_LEN);
	if (!pKitchWorker->firstName)
		return 0;

	if(!readString(pKitchWorker->firstName, file))
	{
		printf("Error: reading first name\n");
		return 0;
	}

	pKitchWorker->lastName = (char*)malloc(STR_MAX_LEN);
	if (!pKitchWorker->lastName)
	{
		free(pKitchWorker->firstName);
		return 0;
	}
	if(!readString(pKitchWorker->lastName, file))
	{
		printf("Error: reading last name\n");
		return 0;
	}
	return 1;
}





void freeKitchenWorker(KitchenWorker* pKitchWorker)
{
	free(pKitchWorker->firstName);
	free(pKitchWorker->lastName);
	free(pKitchWorker);
}


