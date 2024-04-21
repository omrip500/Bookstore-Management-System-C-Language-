#ifndef __KITCHENWORKER__
#define __KITCHENWORKER__
#include "General.h"


struct Kitchen;
typedef struct
{
    char ID[STR_MAX_LEN];
    char* firstName;
    char* lastName;
    struct Kitchen* kitchen;
} KitchenWorker;

int initKitchenWorker(KitchenWorker* pKitchWorker);
void printKitchenWorker(const KitchenWorker* pKitchWorker);
int saveKitchenWorkerToTextFile(const KitchenWorker* pKitchWorker, FILE* file);
int writeKitchenWorkerToBinaryFileCompressed(const KitchenWorker* pKitchWorker, FILE* file);
int readKitchenWorkerFromBinaryFileCompressed(KitchenWorker* pKitchWorker, FILE* file);
int readKitchenWorkerFromTextFile(KitchenWorker* pKitchWorker, FILE* file);
void freeKitchenWorker(KitchenWorker* pKitchWorker);

#endif