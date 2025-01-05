#include <stdio.h>
#include <string.h>

#include "lib.h"

void askID(int *id) {
  printf("=> Enter the customers ID number: ----------< ");
  scanf("%d", id);
  getchar();
}

void listCustomers(Customer *customers, int CustomerNo) {
  if (CustomerNo == 0) {
    divider();
    printf("=> No customers recorded on the system.\n");
    divider();
    return;
  }
  divider();
  printf("=> List of all customers recorded on the system:\n");
  divider();
  for (int i = 0; i < CustomerNo; i++) {
    printf("=> ID Number: %d, Name: %s\n", customers[i].ID, customers[i].name);
  }
  divider();
  fputs("\n\n", stdout);
}

void displayCustomerInfo(Customer *customer) {

  divider();
  printf("=> Customer information: \n");
  divider();
  printf("=> ID Number: ------------< %d\n", customer->ID);
  divider();
  printf("=> Name: -----------------< %s\n", customer->name);
  divider();
  printf("=> Mobile phone number: --< %s\n", customer->phone);
  divider();
  printf("=> Address: --------------< %s\n", customer->address);
  divider();
  printf("=> Pending Charges: ------< $%.2f\n", customer->pendingCharges);
  divider();
  if (customer->rentNo > 0) {

    divider();
    printf("=> Customer Rental History:\n");

    for (int i = 0; i < customer->rentNo; i++) {
      divider();
      printf("=> -----------------------< %d. %s (Rented movie(s) for: %d days)\n", i + 1, customer->rentHistory[i].Movie, customer->rentHistory[i].rentTime);
    }

  } else {
    divider();
    printf("=> Customer has not rented any movies yet.\n");
    divider();
  }
  divider();
  fputs("\n\n", stdout);
}

void addCustomer(Customer *customers, int *existingCustomers) {

  if (*existingCustomers >= MAX_CUSTOMERS) {

    divider();
    printf("=> ERROR: MAX CUSTOMERS REACHED - ABORTED\n");
    divider();
    fputs("\n\n", stdout);

    return;
  }

  Customer newCustomer;
  divider();
  printf("=> Create the customers ID number: ----------< ");
  scanf("%d", &newCustomer.ID);
  divider();
  getchar();
  printf("=> What is their full name?: ----------------< ");
  fgets(newCustomer.name, sizeof(newCustomer.name), stdin);
  newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;
  divider();
  printf("=> What is their mobile phone number?: ------< ");
  fgets(newCustomer.phone, sizeof(newCustomer.phone), stdin);
  newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;
  divider();
  printf("=> What is their address?: ------------------< ");
  fgets(newCustomer.address, sizeof(newCustomer.address), stdin);
  newCustomer.address[strcspn(newCustomer.address, "\n")] = 0;
  newCustomer.pendingCharges = 0.0;
  newCustomer.rentNo = 0;

  customers[*existingCustomers] = newCustomer;
  (*existingCustomers)++;

  divider();
  printf("=> All complete, the customer has been added to the Database!\n");
  divider();
  fputs("\n\n", stdout);
}
