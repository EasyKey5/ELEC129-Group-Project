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

  // TODO: Retrieve customers from file
  Customer customers[50];
  int existingCustomers = 0;

  switch (chooseFromOptions(4, options)) {
  case 1:
    {
      addCustomer(customers, &existingCustomers);
      break;
    }
  case 2:
    {
      int ID;

      askID(&ID);

      for (int i = 0; i < existingCustomers; i++) {
        if (customers[i].ID == ID) {
          displayCustomerInfo(&customers[i]);
          break;
        }
      }
      break;
    }

  case 3:
    {
      listCustomers(customers, existingCustomers);
      break;
    }

  case 4:
    {
      divider();
      printf("Exiting program...\n");
      divider();
      puts("");
      break;
    }
  }
}
