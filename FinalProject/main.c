#include <stdio.h>
#include <stdlib.h>
#include "General.h"
#include "BarberSalon.h"
#include "Branch.h"
#include "Barber.h"
#include "Supplier.h"
#include "Kitchen.h"
#include "KitchenWorker.h"
#include "macros.h"
typedef enum
{
	eAddBranch, eAddBarber, eAddAppointment, eAddKithcn, eOrderProductToKitchenFromSupplier, eUpdateSupplierProducts, eCancelAppointment, eRemoveProductFromKitchen, eSortBarbers, eFindBarber, ePrintAllBarberShops,
	ePrintAllBarbers, ePrintBarberSchedule, ePrintKitchenDetails, eNofOptions
} eMenuOptions;

const char* str[eNofOptions] = { "Add Branch","Add Barber","Add Appointment", "Add Kitchen", "Order Product To Kitchen From Supplier", "Update Supplier Products", "Cancel Appointment", "Remove Product From Kitchen",
								"Sort Barbers", "Find Barber", "Print All BarberShops", "Print All Barbers", "Print Barber Schedule",
								"Print Kitchen Details"
								};

#define EXIT -1
#define BIN_FILE_NAME_SUPPLIER "supplier.bin"
#define TEXT_FILE_NAME_SUPPLIER "supplier.txt"
#define BIN_FILE_NAME_BARBER_SALON "barberSalon.bin"
#define TEXT_FILE_NAME_BARBER_SALON "barberSalon.txt"


int menu();


int main()
{
	Supplier supplier;
	BarberSalon barberSalon;


	printf("\n\n");
	printf("****************************************\n");
	printf("*                                      *\n");
	printf("*        Welcome to the Barber Shop    *\n");
	printf("*                 System               *\n");
	printf("*                                      *\n");
	printf("*        Hours of Operation:           *\n");
	printf("*        Everyday: 09:00 - 20:00       *\n");
	printf("*                                      *\n");
	printf("****************************************\n");


	int loadOption;
	int inputStatus;
	printf("How do you want to load the system? press 0 from Text Files or 1 for Binary Files\n");
	inputStatus = scanf("%d", &loadOption);
	while (inputStatus != 1 || (loadOption != 0 && loadOption != 1))
	{
		printf("Invalid input, please try again\n");
		while (getchar() != '\n');
		inputStatus = scanf("%d", &loadOption);
	}

	initSupplier(&supplier, TEXT_FILE_NAME_SUPPLIER, BIN_FILE_NAME_SUPPLIER, loadOption);
	initBarberSalon(&barberSalon, TEXT_FILE_NAME_BARBER_SALON, BIN_FILE_NAME_BARBER_SALON, loadOption);



	int option;
	int stop = 0;


	do
	{
		option = menu();
		switch (option)
		{
			case eAddBranch:
			{
				addBranch(&barberSalon);
				break;
			}


			case eAddBarber:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				addBarber(pBranch);
				break;
			}

			case eAddAppointment:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				addAppointment(pBranch);
				break;
			}

			case eCancelAppointment:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				cancelAppointment(pBranch);
				break;
			}


			case eSortBarbers:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				sortBarbers(pBranch);
				break;
			}

			case ePrintBarberSchedule:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				if(!printAllBarbers(pBranch))
				{
					break;
				}

				char firstName[STR_MAX_LEN];
				char lastName[STR_MAX_LEN];
				Date date;
				printf("Please enter the first name of the barber:\n");
				myGets(firstName, STR_MAX_LEN);
				printf("Please enter the last name of the barber:\n");
				myGets(lastName, STR_MAX_LEN);
				Barber* pBarber = locateBarberByFirstAndLastName(pBranch, firstName, lastName);
				if (pBranch == NULL)
					break;
				if (!printBusyDates(pBarber))
					break;
				printf("\n\nPlease enter the date of the appointment (dd/mm/yyyy):\n");
				getCorrectDate(&date);
				printBarberScheduleInSpesificDate(pBarber, date, 0);
				break;
			}

			case eFindBarber:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				findBarber(pBranch);
				break;
			}

			case ePrintAllBarberShops:
			{
				printAllBarberShopsDetails(&barberSalon);
				break;
			}

			case eAddKithcn:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				addKitchenToBranch(pBranch, &supplier);
				break;
			}


			case ePrintAllBarbers:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				printAllBarbers(pBranch);
				break;
			}

			case eOrderProductToKitchenFromSupplier:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				orderProductToKitchenFromSupplier(pBranch->pKitch, &supplier);
				break;
			}


			case eUpdateSupplierProducts:
			{
				updateSupplierProducts(&supplier);
				break;
			}

			case eRemoveProductFromKitchen:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				if (pBranch->pKitch != NULL)
				{
					printKitchen(pBranch->pKitch);
					if(!removeProduct(pBranch->pKitch))
					{
						printf("Product not found\n");
					}
				}
				else
				{
					printf("There is no Kitchen in the branch, you can add a kitchen in option 3 \n");
				}
				break;
			}

			case ePrintKitchenDetails:
			{
				Branch* pBranch = getBranchByName(&barberSalon);
				if (pBranch == NULL)
					break;
				printKitchen(pBranch->pKitch);
				break;
			}

			case EXIT:
				printf("Bye bye\n");
				stop = 1;
				break;

			default:
				printf("Wrong option\n");
				break;
			}
	} while (!stop);

	saveBarberSalonToBinaryFile(&barberSalon, BIN_FILE_NAME_BARBER_SALON);
	saveBarberSalonToTextFile(&barberSalon, TEXT_FILE_NAME_BARBER_SALON);
	saveSupplierToBinaryFile(&supplier, BIN_FILE_NAME_SUPPLIER);
	saveSupplierToTextFile(&supplier, TEXT_FILE_NAME_SUPPLIER);
	freeBarberSalon(&barberSalon);
	freeSupplier(&supplier);

	return 1;
}

int menu()
{
	int option;
	printf("\n\nPlease choose an option:\n");
	// Displaying options
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]); // Adding 1 to align option numbers with 1-based indexing

	printf("%d - Quit\n", EXIT);
	printf("----------------------------------------\n");
	printf("Enter your choice: ");

	// Taking user input
	scanf("%d", &option);

	// Clearing buffer
	while ((getchar()) != '\n');

	return option;
}
