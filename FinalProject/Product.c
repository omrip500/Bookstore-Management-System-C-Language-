#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Product.h"
#include "General.h"
#include "macros.h"



int initProduct(Product* pProduct, char* productCode)
{
	strcpy(pProduct->code, productCode);

    printf("Enter product name: ");
    pProduct->productName = (char*)malloc(sizeof(char) * STR_MAX_LEN);
	if (!pProduct->productName)
		return 0;
    myGets(pProduct->productName, STR_MAX_LEN);
	while (!containsJustLetters(pProduct->productName))
	{
		printf("Product name should contain only letters\n");
		printf("Enter product name: ");
		myGets(pProduct->productName, STR_MAX_LEN);
	}

	int amount;
    printf("Enter Product amount: ");
    checkThatInputIsAPositiveNumber(&amount);
	pProduct->amount = amount;

    // Clear input buffer
    while (getchar() != '\n');
	return 1;

}


void printProduct(const Product* pProduct)
{
	printf("Product %s details: \n", pProduct->code);
	printf("Name: %s\n", pProduct->productName);
	printf("Amount: %d\n", pProduct->amount);

}

void getProductCode(char* code)
{
	char temp[STR_MAX_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter product code  - %d UPPER CASE letters\t", CODELENGTH);
		myGets(temp, STR_MAX_LEN);
		//!EQUALS(strlen(temp), CODELENGTH)
		if (!(EQUALS(strlen(temp), CODELENGTH)))
		{
			printf("code should be %d letters\n", CODELENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < CODELENGTH; i++)
			{
				if (isupper(temp[i]) == 0)
				{
					printf("Need to be upper case letter\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

int writeProductToBinaryFile(const Product* pProduct, FILE* file)
{
	int codeLength = (int)strlen(pProduct->code) + 1;

	if (fwrite(&codeLength, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if (fwrite(pProduct->code, sizeof(char), codeLength, file) != codeLength)
	{
		return 0;
	}

	int productNameLength = (int)strlen(pProduct->productName) + 1;
	if(fwrite(&productNameLength, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if(fwrite(pProduct->productName, sizeof(char), productNameLength, file) != productNameLength)
	{
		return 0;
	}

	if(fwrite(&pProduct->amount, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	return 1;

}

int readProductFromBinaryFile(Product* pProduct, FILE* file)
{
	int codeLength;
	if(fread(&codeLength, sizeof(int), 1, file) != 1)
	{
		printf("Error reading product code length from file\n");
		return 0;
	}

	if(fread(pProduct->code, sizeof(char), codeLength, file) != codeLength)
	{
		printf("Error reading product code from file\n");
		return 0;
	}

	int productNameLength;
	if(fread(&productNameLength, sizeof(int), 1, file) != 1)
	{
		printf("Error reading product name length from file\n");
		return 0;
	}

	pProduct->productName = (char*)malloc(sizeof(char) * productNameLength);
	if(!pProduct->productName)
	{
		return 0;
	}

	if(fread(pProduct->productName, sizeof(char), productNameLength, file) != productNameLength)
	{
		printf("Error reading product name from file\n");
		return 0;
	}

	if(fread(&pProduct->amount, sizeof(int), 1, file) != 1)
	{
		printf("Error reading product amount from file\n");
		return 0;
	}

	return 1;

}

int initProductFromTextFile(Product* pProduct, FILE* fp)
{
	if (!pProduct || !fp)
		return 0;

	pProduct->productName = (char*)malloc(sizeof(char) * STR_MAX_LEN);
	if (!pProduct->productName)
	{
		return 0;
	}
	if (!readString(pProduct->code, fp) || !readString(pProduct->productName, fp) ||
		!fscanf(fp, "%d\n", &pProduct->amount))
	{
		return 0;
	}

	return 1;
}

int initProductFromBinaryFile(Product* pProduct, FILE* file)
{
	int codeLength;
	if(fread(&codeLength, sizeof(int), 1, file) != 1)
		return 0;

	if(fread(pProduct->code, sizeof(char), codeLength, file) != codeLength)
		return 0;

	int productNameLength;
	if(fread(&productNameLength, sizeof(int), 1, file) != 1)
		return 0;

	pProduct->productName = (char*)malloc(sizeof(char) * productNameLength);
	if(!pProduct->productName)
		return 0;

	if(fread(pProduct->productName, sizeof(char), productNameLength, file) != productNameLength)
		return 0;

	if(fread(&pProduct->amount, sizeof(int), 1, file) != 1)
		return 0;

	return 1;
}

int readProductFromTextFile(Product* pProduct, FILE* file)
{
	if(!readString(pProduct->code, file))
		return 0;
	pProduct->productName = (char*)malloc(sizeof(char) * STR_MAX_LEN);
	if (!pProduct->productName)
		return 0;
	if(!readString(pProduct->productName, file))
		return 0;
	fscanf(file, "%d\n", &pProduct->amount);
	return 1;
}

int saveProductToTextFile(const Product* pProduct, FILE* file)
{
	fprintf(file, "%s\n", pProduct->code);
	fprintf(file, "%s\n", pProduct->productName);
	fprintf(file, "%d\n", pProduct->amount);
	return 1;
}

void freeKitchenProduct(void* pProduct)
{
	Product* product = (Product*)pProduct;
	if (product)
	{
		free(product->productName);
		free(product);
	}
}

void freeSupplierProduct(void* pProduct)
{
	Product* product = (Product*)pProduct;
	if (product)
	{
		free(product->productName);
	}
}