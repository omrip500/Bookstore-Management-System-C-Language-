#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Supplier.h"
#include "Kitchen.h"
#include "KitchenWorker.h"
#include "Product.h"
#include "def.h"
#include "List.h"
#include "macros.h"

#define TEXT_FILE_NAME "supplier.txt"


int kitchenMenu()
{
	int option;
	int inputStaus;
	printf("\n\n");
	printf("1 - Add a product\n");
	printf("-1 - Exit\n");
	printf("Please choose an option: ");
	inputStaus = scanf("%d", &option);
	while(!inputStaus)
	{
		printf("Option must be a number: 1 or -1\n");
		printf("Please choose an option: ");
		while (getchar() != '\n');
		inputStaus = scanf("%d", &option);
	}
	return option;
}


void initKitchen(Kitchen* pKitch)
{
	pKitch->numOfProducts = 0;
	pKitch->products.head.next = NULL;
	//pKitch->supplier = pSup;
	KitchenWorker* kWorker = (KitchenWorker*)malloc(sizeof(KitchenWorker)); 

	if (!pKitch->kWorker)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}

	(Kitchen*)kWorker->kitchen = pKitch; 
	initKitchenWorker(kWorker);
	pKitch->kWorker = kWorker; 
}



int addProductFromSupplier(Kitchen* pKitch, Supplier* pSup)
{
	int stop = 0;
	do
	{
		int option = kitchenMenu();
		switch (option)
		{
			case 1:
				printProducts(pSup);
				char code[CODELENGTH + 1];
				printf("Enter code, for exit press -1 ");
				scanf("%s", code);
				if (strcmp(code, "-1") == 0) return 0;
				if(findProductInSupplier(pSup, code))
					addProductToKitchen(pKitch, pSup, code);
				else
					printf("Product not found in supplier\n");
				break;

			case -1:
				printf("You have successfully exited the process of ordering products from the supplier\n");
				stop = 1;
				break;

			default:
			{
				printf("Wrong option\n");
				break;
			}
		}
	} while (!stop);
	return 1;
}



int removeProduct(Kitchen* pKitch)
{
	char code[CODELENGTH + 1];
	printf("Enter code, for exit remove process enter -1 ");
	scanf("%s", code);

	if(strcmp(code, "-1") == 0) return 1;

	Product* pProduct = findProductInKitchen(pKitch, code);

	if (pProduct == NULL) return 0;

	NODE* temp = &pKitch->products.head;
	while (temp != NULL && temp->next->key != pProduct)
	{
		temp = temp->next;
	}

	L_delete(temp, freeKitchenProduct);
	pKitch->numOfProducts -= pProduct->amount;
	printf("Product removed successfully\n");
	return 1;
}


void printKitchen(const Kitchen* pKitch)
{
	if (pKitch != NULL)
	{
		printKitchenWorker(pKitch->kWorker);
		printf("Kitchen contains %d products:\n", pKitch->numOfProducts);
		printf("-------------------------------------------------\n");
		printf("| %-20s | %-10s | %-10s |\n", "Product Name", "Code", "Amount");
		printf("-------------------------------------------------\n");

		NODE* p = pKitch->products.head.next;
		while (p != NULL)
		{
			Product* product = (Product*)p->key;
			printf("| %-20s | %-10s | %-10d |\n", product->productName, product->code, product->amount);
			p = p->next;
		}

		printf("-------------------------------------------------\n");
	}
	else
	{
		printf("There is no Kitchen in the branch, you can add a kitchen in option 3 \n");
	}
}

int orderProductToKitchenFromSupplier(Kitchen* pKitch, Supplier* pSup)
{
	if (pKitch != NULL)
	{
		if (!addProductFromSupplier(pKitch, pSup))
		{
			printf("You have successfully exited the process of ordering products from the supplier\n");
			return 0;
		}
	}
	else
	{
		printf("There is no Kitchen in the branch, you can add a kitchen in option 3 \n");
	}
	return 1;
}



void freeProducts(Kitchen* pKitch)
{
	L_free(&pKitch->products, freeKitchenProduct);
}


void freeKitchen(Kitchen* pKitch)
{
	freeProducts(pKitch);
	freeKitchenWorker(pKitch->kWorker);
	free(pKitch);
}




//add integrity checks
int addProductToKitchen(Kitchen* pKitch, const Supplier* pSup, const char* code)
{
	Product* foundedProductInKitchen = findProductInKitchen(pKitch, code);

	int amount;
	printf("Enter amount: \n");
	scanf("%d", &amount);

	NODE* temp = pKitch->products.head.next;

	while (temp != NULL && temp->next != NULL)
	{
		temp = temp->next;
	}

	for (int i = 0; i < pSup->numOfProducts; i++)
	{
		if (strcmp(pSup->products[i].code, code) == 0)
		{
			if(MIN(pSup->products[i].amount, amount) || pSup->products[i].amount <=0 || amount == 0)
			{
				printf("Illegal amount\n");
				return 0;
			}

			if (foundedProductInKitchen == NULL)
			{
				Product* pProduct = (Product*)malloc(sizeof(Product));

				if (!pProduct)
				{
					printf("Memory allocation failed\n");
					return 0;
				}
				pProduct->productName = (char*)malloc(strlen(pSup->products[i].productName) + 1);
				if (!pProduct->productName)
				{
					free(pProduct);
					return 0;
				}

				strcpy(pProduct->productName, pSup->products[i].productName);
				strcpy(pProduct->code, pSup->products[i].code);
				pProduct->amount = amount;
				temp = L_insert(&pKitch->products.head, pProduct);
				pKitch->numOfProducts = SUM(pKitch->numOfProducts, amount);

			}
			else
			{
				foundedProductInKitchen->amount = SUM(foundedProductInKitchen->amount, amount);
				pKitch->numOfProducts = SUM(pKitch->numOfProducts, amount);
			}
			pSup->products[i].amount = SUM(pSup->products[i].amount, -amount);
		}
	}
	return 1;
}


Product* findProductInKitchen(Kitchen* pKitch, const char* code)
{
	if (pKitch == NULL) {
		return NULL; // Handle NULL pointer gracefully
	}

	NODE* p = pKitch->products.head.next;

	while (p != NULL)
	{
		Product* product = (Product*)p->key; // Assuming key points to a Product struct
		if (strcmp(product->code, code) == 0)
		{
			return product;
		}
		p = p->next;
	}

	return NULL; // Product not found
}

int saveKitchenToTextFile(Kitchen* pKitch, FILE* file)
{
	int isKitchenNull = 0;
	if (pKitch == NULL || file == NULL)
	{
		isKitchenNull = 1;

		if(isKitchenNull)
		{
			fprintf(file, "%d\n", isKitchenNull);
			return 1;
		}
	}
	fprintf(file, "%d\n", isKitchenNull);

	int numOfProductsTypes = L_count(&pKitch->products);

	fprintf(file, "%d\n", numOfProductsTypes);

	fprintf(file, "%d\n", pKitch->numOfProducts);

	LIST* pList = &pKitch->products;
	NODE* pNode = &pList->head;
	while (pNode->next != NULL)
	{
		if(saveProductToTextFile((Product*)pNode->next->key, file) == 0)
		{
			return 0;
		}
		pNode = pNode->next;
	}

	if(saveKitchenWorkerToTextFile(pKitch->kWorker, file) == 0)
		return 0;

	return 1;
}

int saveKitchenToBinaryFile(Kitchen* pKitch, FILE* file)
{
	int isKitchenNull = 0;
	if (pKitch == NULL || file == NULL)
	{
		isKitchenNull = 1;

		if (fwrite(&isKitchenNull, sizeof(int), 1, file) != 1);
		return 1;
	}
	if (fwrite(&isKitchenNull, sizeof(int), 1, file) != 1);

	int numOfProductsTypes = L_count(&pKitch->products);

	if (fwrite(&numOfProductsTypes, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if (fwrite(&pKitch->numOfProducts, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	LIST* pList = &pKitch->products;
	NODE* pNode = &pList->head;
	while (pNode->next != NULL)
	{
		if(writeProductToBinaryFile((Product*)pNode->next->key, file) == 0)
		{
			return 0;
		}
		pNode = pNode->next;
	}

	if(writeKitchenWorkerToBinaryFileCompressed(pKitch->kWorker, file) == 0)
		return 0;

	return 1;
}

int loadKitchenFromTextFile(Kitchen* pKitch, FILE* file)
{
	int isKitchenNull;
	if (fscanf(file, "%d\n", &isKitchenNull) != 1)
	{
		return 0;
	}
	if (isKitchenNull)
	{
		return 2;
	}

	int numOfProductsTypes;
	if (fscanf(file, "%d\n", &numOfProductsTypes) != 1)
	{
		return 0;
	}

	if (fscanf(file, "%d\n", &pKitch->numOfProducts) != 1)
	{
		return 0;
	}

	LIST pList;
	L_init(&pList);
	NODE* pNode = &pList.head;

	for (int i = 0; i < numOfProductsTypes; i++)
	{
		Product* pProduct = (Product*)malloc(sizeof(Product));
		if (!pProduct)
		{
			return 0;
		}

		if(readProductFromTextFile(pProduct, file) == 0)
		{
			return 0;
		}
		pNode = L_insert(pNode, pProduct);
	}
	pKitch->products = pList;


	pKitch->kWorker = (KitchenWorker*)malloc(sizeof(KitchenWorker));
	if (!pKitch->kWorker)
	{
		for(int i = 0; i < numOfProductsTypes; i++)
		{
			L_free(&pList, freeKitchenProduct);
		}
		fclose(file);
		return 0;
	}
	if(readKitchenWorkerFromTextFile(pKitch->kWorker, file) == 0)
		return 0;

	return 1;

}

int loadKitchenFromBinaryFile(Kitchen* pKitch, FILE* file)
{
	int isKitchenNull;
	if (fread(&isKitchenNull, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (isKitchenNull)
	{
		return 2;
	}

	int numOfProductsTypes;
	if (fread(&numOfProductsTypes, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	if (fread(&pKitch->numOfProducts, sizeof(int), 1, file) != 1)
	{
		return 0;
	}

	LIST pList;
	L_init(&pList);
	NODE* pNode = &pList.head;


	for (int i = 0; i < numOfProductsTypes; i++)
	{
		Product* pProduct = (Product*)malloc(sizeof(Product));
		if (!pProduct)
		{
			return 0;
		}

		if(readProductFromBinaryFile(pProduct, file) == 0)
		{
			return 0;
		}
		pNode = L_insert(pNode, pProduct);
	}
	pKitch->products = pList;


	pKitch->kWorker = (KitchenWorker*)malloc(sizeof(KitchenWorker));
	if (!pKitch->kWorker)
	{
		for(int i = 0; i < numOfProductsTypes; i++)
		{
			L_free(&pList, freeKitchenProduct);
		}
		return 0;
	}
	if(readKitchenWorkerFromBinaryFileCompressed(pKitch->kWorker, file) == 0)
		return 0;

	return 1;
}

