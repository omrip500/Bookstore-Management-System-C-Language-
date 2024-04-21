#ifndef __BRANCH__
#define __BRANCH__

#include "Barber.h"
#include "Kitchen.h"
#include "Address.h"
#include "Client.h"
#include "Supplier.h"


typedef enum { eFirstName, eLastName, eAge, eNotSorted, eNofTypes } SortType;
static const char* SortTitle[eNofTypes] = { "First name", "Last name", "Age", "Not Sorted" };

typedef struct
{
	char* name;
	Address* adrr;
	Barber** barbers;
	int barbersCount;
	Kitchen* pKitch;
	SortType type;
}Branch;


int initBranch(Branch* pBranch, Branch* allBranches, int branchesAmount);
int addBarber(Branch* pBranch);
int addAppointment(Branch* pBranch);
int cancelAppointment(Branch* pBranch);
int sortBarbers(Branch* pBranch);
int findBarberByFirstName(const Branch* pBranch);
int findBarberByLastName(const Branch* pBranch);
void findBarberByAge(const Branch* pBranch);
void findBarber(const Branch* pBranch);
Barber* locateBarberByFirstAndLastName(Branch* pBranch, const char* firstName, const char* lastName);
void printBranch(void* pBranch);
int printAllBarbers(const Branch* pBranch);
void freeBranch(void* pBranch);
void generalArrayFunction(void* arr, int size, int typeSize, void(*f)(void* element));
int compareByFirstName(const void* barber1, const void* barber2);
int compareByFirstNameForFind(const void* barber1, const void* barber2);
int compareByLastName(const void* barber1, const void* barber2);
int compareByLastNameForFind(const void* barber1, const void* barber2);
int compareByAge(const void* barber1, const void* barber2);
int compareByAgeForFind(const void* barber1, const void* barber2);
int addKitchenToBranch(Branch* pBranch, Supplier* pSup);
int saveBranchToBinaryFile(const Branch* pBranch, FILE* file);
int saveBranchToTextFile(const Branch* pBranch, FILE* file);
int loadBranchFromTextFile(Branch* pBranch, FILE* file);
int loadBranchFromBinaryFile(Branch* pBranch, FILE* file);
SortType getSortType();

#endif
