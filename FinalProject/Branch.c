#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Branch.h"
#include "Barber.h"	
#include "Kitchen.h"
#include "Address.h"
#include "Client.h"
#include"Appointment.h"
#include "Date.h"
#include "General.h"
#include "Schedule.h"
#include "BarberSalon.h"


int initBranch(Branch* pBranch, Branch* allBranches, int branchesAmount)
{
	printf("Please enter the branch name, for exit insert -1\n");
	pBranch->name = (char*)malloc(STR_MAX_LEN);
	if(!pBranch->name)
	{
		return 0;
	}
	myGets(pBranch->name, STR_MAX_LEN);
	if (strcmp(pBranch->name, "-1") == 0)
	{
		free(pBranch->name);
		pBranch->name = NULL;
		return 0;
	}

	while (!containsJustLetters(pBranch->name) || isNameExist(allBranches, branchesAmount, pBranch->name) || strcmp(pBranch->name, "-1") == 0)
	{
		if (!containsJustLetters(pBranch->name))
		{
			printf("Branch name must contain only letters, please enter a different name:\n");
			printf("Please enter the branch name, for exit insert -1\n");
			myGets(pBranch->name, STR_MAX_LEN);
			continue;
		}
		else if(isNameExist(allBranches, branchesAmount, pBranch->name))
		{
			printf("Branch name already exists, please enter a different name:\n");
			printf("Please enter the branch name, for exit insert -1\n");
			myGets(pBranch->name, STR_MAX_LEN);
		}
		if (strcmp(pBranch->name, "-1") == 0)
		{
			free(pBranch->name);
			pBranch->name = NULL;
			return 0;
		}
	}


	pBranch->adrr = (Address*)malloc(sizeof(Address));
	if (!pBranch->adrr)
	{
		free(pBranch->name);
		return 0;
	}
	initAddress(pBranch->adrr);

	//maybe we should change it to void* because this function repeats itself with isNameExist
	while(isAddressExist(allBranches, branchesAmount, pBranch->adrr))
	{
		printf("Branch address already exists, please enter a different address:\n");
		initAddress(pBranch->adrr);
	}


	pBranch->pKitch = NULL;
	pBranch->barbers = NULL;
	pBranch->barbersCount = 0;
	pBranch->type = eNotSorted;
	return 1;
}



int addBarber(Branch* pBranch)
{
	// Allocate memory for an updated array of pointers to Barber structs
	Barber** pNewBarbers = (Barber**)realloc(pBranch->barbers, sizeof(Barber*) * (pBranch->barbersCount + 1));
	if (!pNewBarbers)
	{
		printf("Memory allocation failed\n");
		return 0;
	}

	// Update the Branch's barber array with the new memory allocation
	pBranch->barbers = pNewBarbers;

	// Allocate memory for the new Barber struct
	pBranch->barbers[pBranch->barbersCount] = (Barber*)malloc(sizeof(Barber));
	if (!pBranch->barbers[pBranch->barbersCount])
	{
		free(pNewBarbers);
		return 0;

	}
	if (!pBranch->barbers[pBranch->barbersCount])
	{
		printf("Memory allocation failed\n");
		return 0;
	}

	// Initialize the new Barber
	initBarber(pBranch->barbers[pBranch->barbersCount]);

	// Increment the Branch's barber count
	pBranch->barbersCount++;

	return 1;
}



// check that the user input is valid
int addAppointment(Branch* pBranch)
{
	if (pBranch->barbersCount == 0)
	{
		printf("There are no barbers in the branch\n");
		return 0;
	}

	char firstName[STR_MAX_LEN];
	char lastName[STR_MAX_LEN];
	
	printAllBarbers(pBranch);

	printf("Please enter the first name of the barber:\n");
	myGets(firstName, STR_MAX_LEN);
	printf("Please enter the last name of the barber:\n");
	myGets(lastName, STR_MAX_LEN);
	Barber* pBarber = locateBarberByFirstAndLastName(pBranch, firstName, lastName);
	if (!pBarber)
	{
		printf("Barber not found\n");
		return 0;
	}

	addAppointmentToSchedule(pBarber);


	return 1;
}


int cancelAppointment(Branch* pBranch)
{
	char firstName[STR_MAX_LEN];
	char lastName[STR_MAX_LEN];

	printAllBarbers(pBranch);

	printf("Please enter the first name of the barber:\n");
	myGets(firstName, STR_MAX_LEN);
	printf("Please enter the last name of the barber:\n");
	myGets(lastName, STR_MAX_LEN);
	Barber* pBarber = locateBarberByFirstAndLastName(pBranch, firstName, lastName);
	if (!pBarber)
	{
		printf("Barber not found\n");
		return 0;
	}
	if (!printBusyDates(pBarber))
		return 0;
	cancelAppointmentFromSchedule(pBarber);
	return 1;
}



int sortBarbers(Branch* pBranch)
{

	if (pBranch->barbersCount == 0)
	{
		printf("There are no barbers to sort\n");
		return 0;
	}
	SortType type = getSortType();

	switch (type)
	{
	case 1:
		qsort(pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByFirstName);
		pBranch->type = eFirstName;
		break;

	case 2:
		qsort(pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByLastName);
		pBranch->type = eLastName;
		break;

	case 3:
		qsort(pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByAge);
		pBranch->type = eAge;
		break;
	}
	return 1;
}


int findBarberByFirstName(const Branch* pBranch)
{
	char firstName[STR_MAX_LEN];
	printf("Please enter the first name of the barber:\n");
	scanf("%s", firstName);

	Barber temp = { 0 };
	//memset(&temp, 0, sizeof(Barber)); // Initialize temp

	temp.firstName = (char*)malloc(STR_MAX_LEN);
	if(!temp.firstName)
	{
		return 0;
	}
	strcpy(temp.firstName, firstName);
	Barber** found = (Barber**)bsearch(&temp, pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByFirstNameForFind);
	if (found)
	{
		printf("Barber found, ");
		printBarber(found);
	}
	else
		printf("Barber was not found\n");
	return 1;
}


int findBarberByLastName(const Branch* pBranch)
{
	char lastName[STR_MAX_LEN];
	printf("Please enter the last name of the barber:\n");
	scanf("%s", lastName);
	Barber temp = { 0 };
	temp.lastName = (char*)malloc(STR_MAX_LEN);
	if(!temp.lastName)
	{
		return 0;
	}
	strcpy(temp.lastName, lastName);
	Barber** found = (Barber**)bsearch(&temp, pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByLastNameForFind);
	if (found)
	{
		printf("Barber found, ");
		printBarber(found);
		return 1;
	}
	else
	{
		printf("Barber was not found\n");
		return 0;
	}
	return 1;
}

void findBarberByAge(const Branch* pBranch)
{
	int age;
	printf("Please enter the age of the barber:\n");
	scanf("%d", &age);
	Barber temp = { 0 };
	temp.age = age;
	Barber** found = (Barber**)bsearch(&temp, pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), compareByAgeForFind);
	if (found)
	{
		printf("Barber found, ");
		printBarber(found);
	}
	else
		printf("Barber was not found\n");
}

void findBarber(const Branch* pBranch)
{
	SortType type = pBranch->type;
	switch (type)
	{
		case eFirstName:
			findBarberByFirstName(pBranch);
			break;
		case eLastName:
			findBarberByLastName(pBranch);
			break;
		case eAge:
			findBarberByAge(pBranch);
			break;
		default:
			printf("The barbers are not sorted\n");
			break;
	}
}


Barber* locateBarberByFirstAndLastName(Branch* pBranch, const char* firstName, const char* lastName)
{
	for (int i = 0; i < pBranch->barbersCount; i++)
	{
		if (strcmp(pBranch->barbers[i]->firstName, firstName) == 0 && strcmp(pBranch->barbers[i]->lastName, lastName) == 0)
			return pBranch->barbers[i];
	}
	return NULL;
}


void printBranch(void* pBranch)
{
	Branch* branch = (Branch*)pBranch;
	printf("| %-30s | %-20s | %-20s | %-15s |%-10d |\n",
		branch->name,
		branch->adrr->country,
		branch->adrr->city,
		branch->adrr->street,
		branch->adrr->number);
}







int printAllBarbers(const Branch* pBranch)
{
	if (pBranch->barbersCount == 0)
	{
		printf("There are no barbers in the branch\n");
		return 0;
	}
	printf("\n\nBarbers in the branch:\n");
	generalArrayFunction(pBranch->barbers, pBranch->barbersCount, sizeof(Barber*), printBarber);
	printf("\n\n");
	return 1;
}



void freeBranch(void* pBranch)
{
	Branch* branch = (Branch*)pBranch;
	generalArrayFunction(branch->barbers, branch->barbersCount, sizeof(Barber*), freeBarber);
	free(branch->barbers);
	free(branch->name);
	freeAddress(branch->adrr);
	//free(branch->adrr);
	if(branch->pKitch)
		freeKitchen(branch->pKitch);
}



void generalArrayFunction(void* arr, int size, int typeSize, void(*f)(void* element))
{
	for (int i = 0; i < size; i++)
		f((char*)arr + i * typeSize);
}



int compareByFirstName(const void* barber1, const void* barber2)
{
	Barber* pBarber1 = *(Barber**)barber1;
	Barber* pBarber2 = *(Barber**)barber2;
	return strcmp(pBarber1->firstName, pBarber2->firstName);
}

int compareByFirstNameForFind(const void* barber1, const void* barber2)
{
	Barber* pBarber1 = (Barber*)barber1;
	Barber* pBarber2 = *(Barber**)barber2;
	return strcmp(pBarber1->firstName, pBarber2->firstName);
}


int compareByLastName(const void* barber1, const void* barber2)
{
	Barber* pBarber1 = *(Barber**)barber1;
	Barber* pBarber2 = *(Barber**)barber2;
	return strcmp(pBarber1->lastName, pBarber2->lastName);
}

int compareByLastNameForFind(const void* barber1, const void* barber2)
{
	Barber* pBarber1 = (Barber*)barber1;
	Barber* pBarber2 = *(Barber**)barber2;
	return strcmp(pBarber1->lastName, pBarber2->lastName);
}


int compareByAge(const void* barber1, const void* barber2)
{
	 Barber* pBarber1 = *(Barber**)barber1;
	 Barber* pBarber2 = *(Barber**)barber2;

	if (pBarber1->age < pBarber2->age)
		return -1;
	else if (pBarber1->age > pBarber2->age)
		return 1;
	else
		return 0;
}

int compareByAgeForFind(const void* barber1, const void* barber2)
{
	Barber* pBarber1 = (Barber*)barber1;
	Barber* pBarber2 = *(Barber**)barber2;

	if (pBarber1->age < pBarber2->age)
		return -1;
	else if (pBarber1->age > pBarber2->age)
		return 1;
	else
		return 0;
}

int addKitchenToBranch(Branch* pBranch, Supplier* pSup)
{
	if (pBranch->pKitch == NULL)
	{
		pBranch->pKitch = (Kitchen*)malloc(sizeof(Kitchen));
		if (!pBranch->pKitch)
		{
			return 0;
		}
		initKitchen(pBranch->pKitch);
	}
	else
	{
		printf("Kitchen already exists\n");
	}
	return 1;
}



int saveBranchToBinaryFile(const Branch* pBranch, FILE* file)
{

	int nameSize = (int)strlen(pBranch->name) + 1;


	if(fwrite(&nameSize, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	if(fwrite(pBranch->name, sizeof(char), nameSize, file) != nameSize)
	{
		fclose(file);
		return 0;
	}

	if(!saveAddressToBinaryFileCompressed(pBranch->adrr, file))
	{
		fclose(file);
		return 0;
	}

	if(fwrite(&pBranch->barbersCount, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	for(int i = 0; i < pBranch->barbersCount; i++)
	{
		if(!saveBarberToBinaryFile(pBranch->barbers[i], file))
		{
			fclose(file);
			return 0;
		}
	}

	if(!saveKitchenToBinaryFile(pBranch->pKitch, file))
	{
		fclose(file);
		return 0;
	}

	if(fwrite(&pBranch->type, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	return 1;
}

int saveBranchToTextFile(const Branch* pBranch, FILE* file)
{
	fprintf(file, "%s\n", pBranch->name);
	saveAddressToTextFile(pBranch->adrr, file);
	fprintf(file, "%d\n", pBranch->barbersCount);
	for (int i = 0; i < pBranch->barbersCount; i++)
	{
		saveBarberToTextFile(pBranch->barbers[i], file);
	}
	saveKitchenToTextFile(pBranch->pKitch, file);
	fprintf(file, "%d\n", pBranch->type);
	return 1;

}

int loadBranchFromTextFile(Branch* pBranch, FILE* file)
{
	pBranch->name = (char*)malloc(STR_MAX_LEN);
	if(!pBranch->name)
	{
		fclose(file);
		return 0;
	}
	if(!readString(pBranch->name, file))
	{
		fclose(file);
		return 0;
	}

	pBranch->adrr = (Address*)malloc(sizeof(Address));
	if(!pBranch->adrr)
	{
		free(pBranch->name);
		fclose(file);
		return 0;
	}
	if(!loadAddressFromTextFile(pBranch->adrr, file))
	{
		fclose(file);
		return 0;
	}

	if(fscanf(file, "%d\n", &pBranch->barbersCount) != 1)
	{
		fclose(file);
		return 0;
	}

	pBranch->barbers = (Barber**)malloc(sizeof(Barber*) * pBranch->barbersCount);
	if(!pBranch->barbers)
	{
		free(pBranch->name);
		freeAddress(pBranch->adrr);
		fclose(file);
		return 0;
	}

	for(int i = 0; i < pBranch->barbersCount; i++)
	{
		pBranch->barbers[i] = (Barber*)malloc(sizeof(Barber));
		if(!pBranch->barbers[i])
		{
			free(pBranch->name);
			freeAddress(pBranch->adrr);
			for(int j = 0; j < i; j++)
			{
				freeBarber(pBranch->barbers[j]);
			}
			free(pBranch->barbers);
			fclose(file);
			return 0;
		}
		if(!loadBarberFromTextFile(pBranch->barbers[i], file))
		{
			fclose(file);
			return 0;
		}
	}

	pBranch->pKitch = (Kitchen*)malloc(sizeof(Kitchen));
	if(!pBranch->pKitch)
	{
		free(pBranch->name);
		freeAddress(pBranch->adrr);
		for(int i = 0; i < pBranch->barbersCount; i++)
		{
			freeBarber(pBranch->barbers[i]);
		}
		free(pBranch->barbers);
		fclose(file);
		return 0;
	}
	
	int loadKitchenStatus = loadKitchenFromTextFile(pBranch->pKitch, file);
	if(loadKitchenStatus == 2)
	{
		free(pBranch->pKitch);
		pBranch->pKitch = NULL;
	}
	else if (loadKitchenStatus == 0)
	{
		fclose(file);
		return 0;
	}

	if(fscanf(file, "%d\n", &pBranch->type) != 1)
	{
		fclose(file);
		return 0;
	}
	return 1;
}


int loadBranchFromBinaryFile(Branch* pBranch, FILE* file)
{
	int nameSize;
	if(fread(&nameSize, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	pBranch->name = (char*)malloc(nameSize);
	if(!pBranch->name)
	{
		fclose(file);
		return 0;
	}
	if(fread(pBranch->name, sizeof(char), nameSize, file) != nameSize)
	{
		fclose(file);
		return 0;
	}

	pBranch->adrr = (Address*)malloc(sizeof(Address));
	if(!pBranch->adrr)
	{
		free(pBranch->name);
		fclose(file);
		return 0;
	}
	if(!loadAddressFromBinaryFileCompressed(pBranch->adrr, file))
	{
		fclose(file);
		return 0;
	}

	if(fread(&pBranch->barbersCount, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	pBranch->barbers = (Barber**)malloc(sizeof(Barber*) * pBranch->barbersCount);
	if(!pBranch->barbers)
	{
		free(pBranch->name);
		freeAddress(pBranch->adrr);
		fclose(file);
		return 0;
	}

	for(int i = 0; i < pBranch->barbersCount; i++)
	{
		pBranch->barbers[i] = (Barber*)malloc(sizeof(Barber));
		if(!pBranch->barbers[i])
		{
			free(pBranch->name);
			freeAddress(pBranch->adrr);
			for(int j = 0; j < i; j++)
			{
				freeBarber(pBranch->barbers[j]);
			}
			free(pBranch->barbers);
			fclose(file);
			return 0;
		}
		if(!loadBarberFromBinaryFile(pBranch->barbers[i], file))
		{
			fclose(file);
			return 0;
		}
	}

	pBranch->pKitch = (Kitchen*)malloc(sizeof(Kitchen));
	if(!pBranch->pKitch)
	{
		free(pBranch->name);
		freeAddress(pBranch->adrr);
		for(int i = 0; i < pBranch->barbersCount; i++)
		{
			freeBarber(pBranch->barbers[i]);
		}
		free(pBranch->barbers);
		fclose(file);
		return 0;
	}
	int loadKitchenStatus = loadKitchenFromBinaryFile(pBranch->pKitch, file);
	if(loadKitchenStatus == 2)
	{
		free(pBranch->pKitch);
		pBranch->pKitch = NULL;
	}
	else if (loadKitchenStatus == 0)
	{
		fclose(file);
		return 0;
	}

	if(fread(&pBranch->type, sizeof(int), 1, file) != 1)
	{
		fclose(file);
		return 0;
	}

	return 1;
}







SortType getSortType()
{
	printf("\nBase on what field do you want to sort?\n");
	int temp;
	do {
		for (int i = 0; i < eNofTypes - 1; i++)
			printf("Enter %d for %s\n", i + 1, SortTitle[i]);
		scanf("%d", &temp);
	} while (temp < 0 || temp >= eNofTypes);
	return (SortType)temp;
}