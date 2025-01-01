#include <stdio.h>
#include <string.h>

#include "lib.h"

void askID(int *id) {
    printf("=> Enter the customers ID number: ----------< ");
    scanf("%d", id);
    getchar();
}


void listCustomers(Customer* customers, int CustomerNo) {
    if (CustomerNo == 0) {
    	printf("==============================================================================================\n");
        printf("=> No customers recorded on the system.\n");
        printf("==============================================================================================\n\n\n");
        return;
    }
    printf("==============================================================================================\n");
    printf("=> List of all customers recorded on the system:\n");
    printf("==============================================================================================\n");
    for (int i = 0; i < CustomerNo; i++) {
        printf("=> ID Number: %d, Name: %s\n", customers[i].ID, customers[i].name);
    }
    printf("==============================================================================================\n\n\n");
}


void DisplayCustomerInfo(Customer* customer) {

    printf("==============================================================================================\n");
    printf("=> Customer information: \n");
    printf("==============================================================================================\n");
    printf("=> ID Number: -------------------------------< %d\n", customer->ID);
    printf("==============================================================================================\n");
    printf("=> Name: ------------------------------------< %s\n", customer->name);
    printf("==============================================================================================\n");
    printf("=> Mobile phone number: ---------------------< %s\n", customer->phone);
    printf("==============================================================================================\n");
    printf("=> Address: ---------------------------------< %s\n", customer->address);
    printf("==============================================================================================\n");
    printf("=> Pending Charges: -------------------------< $%.2f\n", customer->pendingCharges);
    printf("==============================================================================================\n");
    if (customer->rentNo > 0) {
    printf("==============================================================================================\n");
    printf("=> Customer Rental History:\n");
    for (int i = 0; i < customer->rentNo; i++) {
    printf("==============================================================================================\n");
    printf("=> -----------------------< %d. %s (Rented movie(s) for: %d days)\n", i + 1, customer->rentHistory[i].Movie, customer->rentHistory[i].rentTime);
        }
    } else {printf("==============================================================================================\n");
        printf("=> Customer has not rented any movies yet.\n");
        printf("==============================================================================================\n\n\n");
    }
    printf("==============================================================================================\n\n\n");
}

void addCustomer(Customer* customers, int* CustomerNo) {
    Customer newCustomer;
    printf("==============================================================================================\n");
    printf("=> Create the customers ID number: ----------< ");
    scanf("%d", &newCustomer.ID);
    printf("==============================================================================================\n");
    getchar();
    printf("=> What is their full name?: ----------------< ");
    fgets(newCustomer.name, sizeof(newCustomer.name), stdin);
    newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;
    printf("==============================================================================================\n");
    printf("=> What is their mobile phone number?: ------< ");
    fgets(newCustomer.phone, sizeof(newCustomer.phone), stdin);
    newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;
	printf("==============================================================================================\n");
    printf("=> What is their address?: ------------------< ");
    fgets(newCustomer.address, sizeof(newCustomer.address), stdin);
    newCustomer.address[strcspn(newCustomer.address, "\n")] = 0;
    newCustomer.pendingCharges = 0.0;
    newCustomer.rentNo = 0;

    customers[*CustomerNo] = newCustomer;
    (*CustomerNo)++;
    printf("==============================================================================================\n");
    printf("=> All complete, the customer has been added to the Database!\n");
    printf("==============================================================================================\n\n\n");
}
