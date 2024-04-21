#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BarberSalon.h"
#include "Branch.h"
#include "General.h"
#include "macros.h"



int initBarberSalon(BarberSalon* pBarberSalon, const char* textFileName, const char* binFileName, int loadingWay)
{
	FILE* fText = fopen(textFileName, "r");
	FILE* fBinary = fopen(binFileName, "rb");

	if (!fText && !fBinary)
	{
		printf("This is the first time you are using the system, please initialize the BarberSalon manually\n");
		pBarberSalon->pBranch = NULL;
		pBarberSalon->branchCount = 0;
		return 0;
	}
	else
	{
		if (loadingWay == 0)
		{
			if (fBinary)
				fclose(fBinary);

			if (!loadBarberSalonFromTextFile(pBarberSalon, textFileName))
			{
				return 0;
			}
		}
		else
		{
			if (fText)
				fclose(fText);

			if (!initBarberSalonBinary(pBarberSalon, binFileName))
			{
				return 0;
			}
		}
	}
	return 1;
}

int initBarberSalonBinary(BarberSalon* pBarberSalon, const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (!fp)
	{
		pBarberSalon->pBranch = NULL;
		pBarberSalon->branchCount = 0;
		return 1;
	}

	if(!loadBarberSalonFromBinaryFile(pBarberSalon, fp))
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}

int saveBarberSalonToTextFile(const BarberSalon* pBarberSalon, const char* fileName)
{
	FILE* fp = fopen(fileName, "w");
	CHECK_RETURN_NULL(fp);

	fprintf(fp, "%d\n", pBarberSalon->branchCount);

	for (int i = 0; i < pBarberSalon->branchCount; i++)
	{
		saveBranchToTextFile(&pBarberSalon->pBranch[i], fp);
	}

	fclose(fp);
	return 1;

}


int loadBarberSalonFromBinaryFile(BarberSalon* pBarberSalon, FILE* fp)
{
	if (fread(&pBarberSalon->branchCount, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}

	pBarberSalon->pBranch = (Branch*)malloc(sizeof(Branch) * pBarberSalon->branchCount);

	if (!pBarberSalon->pBranch)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pBarberSalon->branchCount; i++)
	{
		if (!loadBranchFromBinaryFile(&pBarberSalon->pBranch[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	return 1;
}

int loadBarberSalonFromTextFile(BarberSalon* pBarberSalon, const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (!fp)
		return 0;

	if (fscanf(fp, "%d\n", &pBarberSalon->branchCount) != 1)
	{
		fclose(fp);
		return 0;
	}

	pBarberSalon->pBranch = (Branch*)malloc(sizeof(Branch) * pBarberSalon->branchCount);

	if (!pBarberSalon->pBranch)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pBarberSalon->branchCount; i++)
	{
		if (!loadBranchFromTextFile(&pBarberSalon->pBranch[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

int addBranch(BarberSalon* pBarberSalon)
{
	Branch branch;
	if (!initBranch(&branch, pBarberSalon->pBranch, pBarberSalon->branchCount))
		return 0;

	pBarberSalon->pBranch = realloc(pBarberSalon->pBranch, sizeof(Branch) * (pBarberSalon->branchCount + 1));
	if (!pBarberSalon->pBranch)
	{
		return 0;
	}
	pBarberSalon->pBranch[pBarberSalon->branchCount] = branch;
	pBarberSalon->branchCount++;
	return 1;
}

void printAllBarberShopsDetails(const BarberSalon* pBarberSalon)
{
	if (pBarberSalon->branchCount == 0)
	{
		printf("No branches found.\n");
		return;
	}

	printf("Barber Shop Branches:\n");
	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("| %-30s | %-20s | %-20s | %-15s | %-10s|\n", "Branch Name", "Country", "City", "Street", "Number");
	printf("--------------------------------------------------------------------------------------------------------------\n");

	generalArrayFunction(pBarberSalon->pBranch, pBarberSalon->branchCount, sizeof(Branch), printBranch);

	printf("--------------------------------------------------------------------------------------------------------------\n");
}




Branch* getBranchByName(const BarberSalon* pBarberSalon)
{
	if(pBarberSalon->branchCount == 0)
	{
		printf("No branches found.\n");
		return NULL;
	}
	printAllBarberShopsDetails(pBarberSalon);
	printf("Choose branch name,or enter -1 to come back to the main menu\n");

	char name[STR_MAX_LEN];
	myGets(name, STR_MAX_LEN);
	if(strcmp(name, "-1") == 0)
	{
		return NULL;
	}


	for (int i = 0; i < pBarberSalon->branchCount; i++)
	{
		if (strcmp(pBarberSalon->pBranch[i].name, name) == 0)
		{
			return &pBarberSalon->pBranch[i];
		}	
	}
	printf("Branch not found.\n");
	return NULL;
}


int isNameExist(const Branch* allBranches, const int branchesAmount, const char* name)
{
	for (int i = 0; i < branchesAmount; i++)
	{
		if(strcmp(allBranches[i].name, name) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int isAddressExist(const Branch* allBranches, const int branchesAmount, const Address* address)
{
	for (int i = 0; i < branchesAmount; i++)
	{
		if (strcmp(allBranches[i].adrr->country, address->country) == 0 &&
			strcmp(allBranches[i].adrr->city, address->city) == 0 &&
			strcmp(allBranches[i].adrr->street, address->street) == 0 &&
			allBranches[i].adrr->number == address->number)
		{
			return 1;
		}
	}
	return 0;
}


int saveBarberSalonToBinaryFile(const BarberSalon* pBarberSalon, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	CHECK_RETURN_NULL(fp);

	if (fwrite(&pBarberSalon->branchCount, sizeof(int), 1, fp) != 1)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pBarberSalon->branchCount; i++)
	{
		saveBranchToBinaryFile(&pBarberSalon->pBranch[i], fp);
	}

	fclose(fp);
	return 1;
}

void freeBarberSalon(BarberSalon* pBarberSalon)
{
	generalArrayFunction(pBarberSalon->pBranch, pBarberSalon->branchCount, sizeof(Branch), freeBranch);
	free(pBarberSalon->pBranch);
}