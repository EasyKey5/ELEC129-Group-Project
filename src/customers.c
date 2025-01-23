#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

void retrieveCustomers(Customer *allCustomers, int *customerCount) {
  //
  FILE *file = fopen(CUSTOMER_DB_FILENAME, "rb");

  if (file == NULL) {
    printf("Could not locate file %s, attempting to create it\n", CUSTOMER_DB_FILENAME);
    file = fopen(CUSTOMER_DB_FILENAME, "wb");
    if (file) {
      printf("=> File created successfully\n");
      return;
    } else {
      printf("=> Something went wrong, aborting.\n");
      exit(-1);
    }
  }

  // fwrite(&cusName, sizeof(int), 1, text_file);
  *customerCount = 0;
  Customer currentCustomer;
  while (fread(&currentCustomer, sizeof(Customer), 1, file) == 1) {

    allCustomers[*customerCount] = currentCustomer;
    (*customerCount)++;
  }
  fclose(file);
}

void saveNewCustomer(Customer customer) {
  FILE *db = fopen(CUSTOMER_DB_FILENAME, "ab");

  if (!db) {
    printf("an error occurred while opening %s, aborting\n", CUSTOMER_DB_FILENAME);
    return;
  }

  fwrite(&customer, sizeof(Customer), 1, db);

  fclose(db);
}

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

void displayCustomerInfo(Customer customer) {

  divider();
  printf("=> Customer information: \n");
  printf("=> ID Number: ------------< %d\n", customer.ID);
  printf("=> Name: -----------------< %s\n", customer.name);
  printf("=> Mobile phone number: --< %s\n", customer.phone);
  printf("=> Address: --------------< %s\n", customer.address);
  printf("=> Pending Charges: ------< $%.2f\n", customer.pendingCharges);
  divider();
  if (customer.rentNo > 0) {

    printf("=> Customer Rental History:\n");

    for (int i = 0; i < customer.rentNo; i++) {
      printf("=> -----------------------< %d. %s (Rented movie(s) for: %d days)\n", i + 1, customer.rentHistory[i].Movie, customer.rentHistory[i].rentTime);
    }

  } else {
    printf("=> Customer has not rented any movies yet.\n");
  }
  divider();
  fputs("\n\n", stdout);
}

int createCustomer() {

  // if (*existingCustomers >= MAX_CUSTOMERS) {
  //
  //   divider();
  //   printf("=> ERROR: MAX CUSTOMERS REACHED - ABORTED\n");
  //   divider();
  //   fputs("\n\n", stdout);
  //
  //   return;
  // }

  Customer newCustomer;
  divider();
  printf("=> Create the customers ID number: ----------< ");
  scanf("%d", &newCustomer.ID);
  getchar();
  divider();
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

  // ensure that there isn't a customer already with this id
  int customerCount;
  Customer customers[MAX_CUSTOMERS];

  retrieveCustomers(customers, &customerCount);

  bool validID = false;

  while (!validID) {
    validID = true;
    for (int i = 0; i < customerCount; i++) {
      if (customers[i].ID == newCustomer.ID) {
        validID = false;
        break;
      }
    }

    if (!validID) {
      printf("=> This id is already taken, please choose another: ");
      scanf("%d", &newCustomer.ID);
      getchar();
      divider();
    }
  };

  saveNewCustomer(newCustomer);

  printf("=> All complete, the customer has been added to the Database!\n");
  divider();
  fputs("\n\n", stdout);
  return 1;
}
