#include <stdio.h>

#include "lib.h"

void registrationDesk() {

  divider();
  divider();
  printCenter("Welcome back to the");
  printCenter("Snazzy Video Registration");
  printCenter("Desk!");
  divider();
  divider();
  printCenter("What would you like to do?");
  divider();
  divider();

  char *options[] = {
      "To create a new customer account",
      "To view customer details",
      "To view a list of all customers",
      "To exit",
  };

  int customerCount;
  Customer customers[MAX_CUSTOMERS];
  retrieveCustomers(customers, &customerCount);

  switch (chooseFromOptions(4, options)) {
  case 1:
    { // create customer
      createCustomer();
      break;
    }
  case 2:
    { // view details
      int ID;

      askID(&ID);

      for (int i = 0; i < customerCount; i++) {
        if (customers[i].ID == ID) {
          displayCustomerInfo(customers[i]);
          break;
        }
      }
      break;
    }

  case 3:
    { // view all customers

      for (int i = 0; i < customerCount; i++) {
        displayCustomerInfo(customers[i]);
      }
      break;
    }

  case 4: // exit
    {
      divider();
      printf("Exiting program...\n");
      divider();
      puts("");
      break;
    }
  }
}
