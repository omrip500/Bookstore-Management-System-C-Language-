#ifndef __ADDRESS__
#define __ADDRESS__

#define MAX_ADDRESS_NUMBER 255

typedef struct
{
	char* country;
	char* city;
	char* street;
	int number;
}Address;

int initAddress(Address* pAddress);
void printAddress(const Address* pAddress);
int saveAddressToTextFile(const Address* pAddress, FILE* file);
int saveAddressToBinaryFileCompressed(const Address* pAddress, FILE* file);
int loadAddressFromTextFile(Address* pAddress, FILE* file);
int loadAddressFromBinaryFileCompressed(Address* pAddress, FILE* file);
void freeAddress(Address* pAddress);


#endif
