#pragma once
#ifndef __SUPPLIER__
#define __SUPPLIER__

#include "Product.h"

typedef struct
{
	int numOfProducts;
	Product* products;
}Supplier;


int initSupplierManually(Supplier* pSupplier);
int updateSupplierProducts(Supplier* pSupplier);
int initSupplier(Supplier* pSupplier, char* textFileName, char* binFileName, int loadingWay);
int loadSupplierFromTextFile(Supplier* pSupplier, const char* fileName);
int loadSupplierFromBinaryFile(Supplier* pSupplier, char* fileName);
void printProducts(const Supplier* pSupplier);
int findProductInSupplier(const Supplier* pSupplier, const char* code);
Product* getProductFromSupplier(Supplier* pSupplier, const char* code);
int saveSupplierToTextFile(const Supplier* pSupplier, const char* fileName);
int saveSupplierToBinaryFile(const Supplier* pSupplier, const char* fileName);
void freeProductsArr(Supplier* pSupplier);
void freeSupplier(Supplier* pSupplier);

#endif
