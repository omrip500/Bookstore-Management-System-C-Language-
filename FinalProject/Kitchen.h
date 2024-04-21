#ifndef __KITCHEN__
#define __KITCHEN__

#include "KitchenWorker.h"
#include "list.h"
#include "Supplier.h"

struct KitchenWorker;
typedef struct
{
    LIST products;
    int numOfProducts;
    KitchenWorker* kWorker;
} Kitchen;



void initKitchen(Kitchen* pKitch);
int addProductFromSupplier(Kitchen* pKitch, Supplier* pSup);
int removeProduct(Kitchen* pKitch);
void printKitchen(const Kitchen* pKitch);
int orderProductToKitchenFromSupplier(Kitchen* pKitch, Supplier* pSup);
int addProductToKitchen(Kitchen* pKitch, const Supplier* pSup, const char* code);
Product* findProductInKitchen(Kitchen* pKitch, const char* code);
int saveKitchenToTextFile(Kitchen* pKitch, FILE* file);
int saveKitchenToBinaryFile(Kitchen* pKitch, FILE* file);
int loadKitchenFromTextFile(Kitchen* pKitch, FILE* file);
int loadKitchenFromBinaryFile(Kitchen* pKitch, FILE* file);
void freeProducts(Kitchen* pKitch);
void freeKitchen(Kitchen* pKitch);

#endif
