#ifndef __PRODUCT__
#define __PRODUCT__

#define CODELENGTH 3

typedef struct
{
	char code[CODELENGTH + 1];
	char* productName;
	int amount;

}Product;

int initProduct(Product* pProduct, char* productCode);
void printProduct(const Product* pProduct);
void getProductCode(char* code);
int writeProductToBinaryFile(const Product* pProduct, FILE* file);
int readProductFromBinaryFile(Product* pProduct, FILE* file);
int initProductFromTextFile(Product* pProduct, FILE* fp);
int initProductFromBinaryFile(Product* pProduct, FILE* file);
int readProductFromTextFile(Product* pProduct, FILE* file);
int saveProductToTextFile(const Product* pProduct, FILE* file);
void freeKitchenProduct(void* pProduct);
void freeSupplierProduct(void* pProduct);

#endif
