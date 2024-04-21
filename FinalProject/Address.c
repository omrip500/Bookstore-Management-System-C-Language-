#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Address.h"
#include "General.h"



int initAddress(Address* pAddress)
{
	printf("Enter country: ");
	pAddress->country = (char*)malloc(STR_MAX_LEN);
	if(!pAddress->country)
	{
		printf("Alocation failed\n");
		return 0;
	}
	myGets(pAddress->country, STR_MAX_LEN);
	while(!containsJustLetters(pAddress->country))
	{
		printf("The country name must contain only letters\n");
		printf("Enter country: ");
		myGets(pAddress->country, STR_MAX_LEN);

	}
	printf("Enter city: ");
	pAddress->city = (char*)malloc(STR_MAX_LEN);
	if(!pAddress->city)
	{
		printf("Alocation failed\n");
		free(pAddress->country);
		return 0;
	}
	myGets(pAddress->city, STR_MAX_LEN);
	while (!containsJustLetters(pAddress->city))
	{
		printf("The city name must contain only letters\n");
		printf("Enter city: ");
		myGets(pAddress->city, STR_MAX_LEN);

	}
	printf("Enter street: ");
	pAddress->street = (char*)malloc(STR_MAX_LEN);
	if(!pAddress->street)
	{
		printf("Alocation failed\n");
		free(pAddress->country);
		free(pAddress->city);
		return 0;
	}
	myGets(pAddress->street, STR_MAX_LEN);
	while (!containsJustLetters(pAddress->street))
	{
		printf("The street name must contain only letters\n");
		printf("Enter street: ");
		myGets(pAddress->street, STR_MAX_LEN);

	}
	int number;
	printf("Enter number: ");
	checkThatInputIsAPositiveNumber(&number);
	while (number > MAX_ADDRESS_NUMBER)
	{
		printf("The number must be between 1 and %d\n", MAX_ADDRESS_NUMBER);
		printf("Enter number: ");
		checkThatInputIsAPositiveNumber(&number);
	}
	pAddress->number = number;
	return 1;
}

void printAddress(const Address* pAddress)
{
	printf("Country: %s, City: %s, Street: %s, Number: %d\n", pAddress->country, pAddress->city, pAddress->street, pAddress->number);
}

int saveAddressToTextFile(const Address* pAddress, FILE* file)
{
	fprintf(file, "%s\n", pAddress->country);
	fprintf(file, "%s\n", pAddress->city);
	fprintf(file, "%s\n", pAddress->street);
	fprintf(file, "%d\n", pAddress->number);
	return 1;
}



int saveAddressToBinaryFileCompressed(const Address* pAddress, FILE* file)
{
	int countryNameSize = (int)strlen(pAddress->country);
	int cityNameSize = (int)strlen(pAddress->city);
	int streetNameSize = (int)strlen(pAddress->street);

	BYTE b[3];
	b[0] = (countryNameSize << 3) | (cityNameSize >> 2);
	b[1] = (cityNameSize << 6) | (streetNameSize << 2) | (pAddress->number >> 7);
	b[2] = (pAddress->number << 1);

	if (fwrite(b, sizeof(BYTE), 3, file) != 3)
		return 0;

	if (fwrite(pAddress->country, sizeof(char), countryNameSize, file) != countryNameSize)
		return 0;

	if (fwrite(pAddress->city, sizeof(char), cityNameSize, file) != cityNameSize)
		return 0;

	if (fwrite(pAddress->street, sizeof(char), streetNameSize, file) != streetNameSize)
		return 0;

	return 1;
}

int loadAddressFromTextFile(Address* pAddress, FILE* file)
{
	pAddress->country = (char*)malloc(STR_MAX_LEN);
	if (!pAddress->country)
		return 0;

	pAddress->city = (char*)malloc(STR_MAX_LEN);
	if (!pAddress->city)
	{
		free(pAddress->country);
		return 0;
	}

	pAddress->street = (char*)malloc(STR_MAX_LEN);
	if (!pAddress->street)
	{
		free(pAddress->country);
		free(pAddress->city);
		return 0;
	}

	readString(pAddress->country, file);
	readString(pAddress->city, file);
	readString(pAddress->street, file);
	fscanf(file, "%d\n", &pAddress->number);

	return 1;

}

int loadAddressFromBinaryFileCompressed(Address* pAddress, FILE* file)
{
	BYTE b[3];
	if (fread(b, sizeof(BYTE), 3, file) != 3)
		return 0;

	int countryNameSize = b[0] >> 3;
	int cityNameSize = ((b[0] & 0x07) << 2) | (b[1] >> 6);
	int streetNameSize = (b[1] & 0x3C) >> 2;
	pAddress->number = (b[1] & 0x03) << 7 | (b[2] >> 1);

	pAddress->country = (char*)malloc(countryNameSize + 1);
	if (!pAddress->country)
		return 0;

	pAddress->city = (char*)malloc(cityNameSize + 1);
	if (!pAddress->city)
	{
		free(pAddress->country);
		return 0;
	}

	pAddress->street = (char*)malloc(streetNameSize + 1);
	if (!pAddress->street)
	{
		free(pAddress->country);
		free(pAddress->city);
		return 0;
	}

	if (fread(pAddress->country, sizeof(char), countryNameSize, file) != countryNameSize)
		return 0;
	pAddress->country[countryNameSize] = '\0';

	if (fread(pAddress->city, sizeof(char), cityNameSize, file) != cityNameSize)
		return 0;
	pAddress->city[cityNameSize] = '\0';

	if (fread(pAddress->street, sizeof(char), streetNameSize, file) != streetNameSize)
		return 0;
	pAddress->street[streetNameSize] = '\0';

	return 1;
}



void freeAddress(Address* pAddress)
{
	free(pAddress->country);
	free(pAddress->city);
	free(pAddress->street);
	free(pAddress);
}
