#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "Supplier.h"
#include "Product.h"
#include "General.h"
#include "macros.h"


int initSupplierManually(Supplier* pSupplier)
{
	printf("Currently, our barbershop chain lacks a Supplier for kitchen products.\n");
	printf("Before you begin or continue using the system, please make sure to initialize the provider.\n\n");
	printf("------------------------------------\n");
	printf("Enter number of products: \n");
	scanf("%d", &pSupplier->numOfProducts);
	getchar();
	pSupplier->products = (Product*)malloc(sizeof(Product) * pSupplier->numOfProducts);

	if(!pSupplier->products)
	{
		return 0;
	}

	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		char code[CODELENGTH + 1];
		getProductCode(code);
		while (findProductInSupplier(pSupplier, code))
		{
			printf("Product already exists\n");
			getProductCode(code);
		}
		Product p;
		initProduct(&p, code);
		pSupplier->products[i] = p;
	}
	return 1;
}


int updateSupplierProducts(Supplier* pSupplier)
{
	int stop = 0;
	do
	{
		int option;
		printf("1 - Update product amount\n");
		printf("2 - Add product\n");
		printf("-1 - Exit\n");
		scanf("%d", &option);
		switch (option)
		{
			case 1:
			{
				printProducts(pSupplier);
				printf("Enter code, for exit press -1 ");
				char code[CODELENGTH + 1];
				scanf("%s", code);
				if (strcmp(code, "-1") == 0) return 0;
				Product* foundProduct = getProductFromSupplier(pSupplier, code);
				if (foundProduct != NULL)
				{
					int amount;
					printf("Enter updated amount: ");
					checkThatInputIsAPositiveNumber(&amount);	
					foundProduct->amount = amount;
				}
				else
				{
					printf("Product not found\n");
				}
				break;
			}

			case 2:
			{
				Product product;
				char productCode[CODELENGTH + 1];
				getProductCode(productCode);
				while (findProductInSupplier(pSupplier, productCode))
				{
					printf("Product already exists\n");
					getProductCode(productCode);
				}
				initProduct(&product, productCode);
				pSupplier->products = (Product*)realloc(pSupplier->products, sizeof(Product) * (pSupplier->numOfProducts + 1));
				if (!pSupplier->products)
					return 0;

				pSupplier->products[pSupplier->numOfProducts] = product;
				pSupplier->numOfProducts++;
				printf("\nProduct added successfully!\n");
				break;
			}

			case -1:
			{
				stop = 1;
				break;
			}

			default:
				printf("Wrong option\n");
				break;
		}
	} while (!stop);
	return 1;
}


int initSupplier(Supplier* pSupplier, char* textFileName, char* binFileName, int loadingWay)
{
	FILE* fText = fopen(textFileName, "r");
	FILE* fBinary = fopen(binFileName, "rb");

	if (!fText && !fBinary)
	{
		printf("This is the first time you are using the system, please initialize the supplier\n");
		initSupplierManually(pSupplier);
		return 0;
	}
	else
	{
		if(loadingWay == 0)
		{
			if(fBinary)
				fclose(fBinary);

			if (!loadSupplierFromTextFile(pSupplier, textFileName))
			{
				return 0;
			}
		}
		else
		{
			if(fText)
				fclose(fText);

			if (!loadSupplierFromBinaryFile(pSupplier, binFileName))
			{
				return 0;
			}
		}
	}
	return 1;
}

int loadSupplierFromTextFile(Supplier* pSupplier, const char* fileName)
{
	FILE* fText = fopen(fileName, "r");
	if (!fText)
	{
		return 0;
	}

	if (!fscanf(fText, "%d\n", &pSupplier->numOfProducts))
	{
		return 0;
	}
	pSupplier->products = (Product*)malloc(sizeof(Product) * pSupplier->numOfProducts);
	if (!pSupplier->products)
	{
		return 0;
	}
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (!initProductFromTextFile(&pSupplier->products[i], fText))
		{
			freeProductsArr(pSupplier);
			return 0;
		}
	}
	fclose(fText);
	return 1;
}

int loadSupplierFromBinaryFile(Supplier* pSupplier, char* fileName)
{
	FILE* binaryFile = fopen(fileName, "rb");
	if (!binaryFile)
	{
		return 0;
	}
	if(!fread(&pSupplier->numOfProducts, sizeof(int), 1, binaryFile))
	{
		return 0;
	}
	pSupplier->products = (Product*)malloc(sizeof(Product) * pSupplier->numOfProducts);
	if (!pSupplier->products)
	{
		return 0;
	}
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (!initProductFromBinaryFile(&pSupplier->products[i], binaryFile))
		{
			freeProductsArr(pSupplier);
			return 0;
		}
	}
	fclose(binaryFile);
	return 1;
}




// can be made general
void printProducts(const Supplier* pSupplier)
{
	if (pSupplier->numOfProducts == 0)
	{
		printf("No products\n");
		return;
	}
	printf("Products:\n");
	printf("------------------------------------------------------\n");
	printf("| %-4s | %-20s | %-10s | %-10s |\n", "No.", "Product Name", "Code", "Amount");
	printf("------------------------------------------------------\n");

	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		printf("| %-4d | %-20s | %-10s | %-10d |\n", i + 1, pSupplier->products[i].productName,
			pSupplier->products[i].code, pSupplier->products[i].amount);
	}

	printf("------------------------------------------------------\n");
}


int findProductInSupplier(const Supplier* pSupplier, const char* code)
{
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (strcmp(pSupplier->products[i].code, code) == 0)
		{
			return 1;
		}
	}
	return 0;
}

Product* getProductFromSupplier(Supplier* pSupplier, const char* code)
{
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (strcmp(pSupplier->products[i].code, code) == 0)
		{
			return &pSupplier->products[i];
		}
	}
	return NULL;

}

int saveSupplierToTextFile(const Supplier* pSupplier, const char* fileName)
{
	FILE* fp = fopen(fileName, "w");
	if (!fp)
		CHECK_RETURN_NULL(fp);
	fprintf(fp, "%d\n", pSupplier->numOfProducts);
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (!saveProductToTextFile(&pSupplier->products[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;

}

int saveSupplierToBinaryFile(const Supplier* pSupplier, const char* fileName)
{
	FILE* fp = fopen(fileName, "wb");
	CHECK_RETURN_NULL(fp);
	if (!fwrite(&pSupplier->numOfProducts, sizeof(int), 1, fp))
	{
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		if (!writeProductToBinaryFile(&pSupplier->products[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}


// can be made general
void freeProductsArr(Supplier* pSupplier)
{
	for (int i = 0; i < pSupplier->numOfProducts; i++)
	{
		freeSupplierProduct(&pSupplier->products[i]);
	}
	free(pSupplier->products);
}

void freeSupplier(Supplier* pSupplier)
{
	freeProductsArr(pSupplier);
}
