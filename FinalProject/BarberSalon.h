#ifndef __BARBERSALON__
#define __BARBERSALON__

#include "Branch.h"

typedef struct
{
	Branch* pBranch;
	int branchCount;
}BarberSalon;

int initBarberSalon(BarberSalon* pBarberSalon, const char* textFileName, const char* binFileName, int loadingWay);
int initBarberSalonBinary(BarberSalon* pBarberSalon, const char* fileName);
int saveBarberSalonToTextFile(const BarberSalon* pBarberSalon, const char* fileName);
int loadBarberSalonFromBinaryFile(BarberSalon* pBarberSalon, FILE* fp);
int loadBarberSalonFromTextFile(BarberSalon* pBarberSalon, const char* fileName);
int addBranch(BarberSalon* pBarberSalon);
void printAllBarberShopsDetails(const BarberSalon* pBarberSalon);
Branch* getBranchByName(const BarberSalon* pBarberSalon);
int isNameExist(const Branch* allBranches, const int branchesAmount, const char* name);
int isAddressExist(const Branch* allBranches, const int branchesAmount, const Address* address);
int saveBarberSalonToBinaryFile(const BarberSalon* pBarberSalon, const char* fileName);
void freeBarberSalon(BarberSalon* pBarberSalon);









#endif
