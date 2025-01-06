#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

int main() {

  Customer customers[50];
  int existingCustomers = 0;

  divider();
  printCenter("Welcome back to the");
  printCenter("Snazzy Video Rental System!");
  printCenter("Who are you?");
  divider();
  divider();

  char *modeOptions[3] = {"For Manager", "For Registration", "For Sales Clerk"};

  int choice = chooseFromOptions(3, modeOptions);

  switch (choice) {

  case 1:
    {
      managementConsole();
    }

  default:
    {
    }
  }

  do {
    divider();
    divider();
    printCenter("Welcome back to the");
    printCenter("Snazzy Video Rental Customer");
    printCenter("Database!");
    divider();
    divider();
    printCenter("What would you like to do?");
    divider();
    divider();

    char *options[] = {
        "To create a new customer account",
        "To rent a movie",
        "To return a movie",
        "To view customer details",
        "To view a list of all customers",
        "To exit",
    };

    choice = chooseFromOptions(6, options);


    divider();

    switch (choice) {


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
            rentMovie(&customers[i]);
            break;
          }
        }
        break;
      }

    case 3:
      {
        int ID;
        askID(&ID);

        for (int i = 0; i < existingCustomers; i++) {
          if (customers[i].ID == ID) {
            returnMovie(&customers[i]);
            break;
          }
        }
        break;
      }

    case 4:
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

    case 5:
      {
        listCustomers(customers, existingCustomers);
        break;
      }

    case 6:
      {
        divider();
        printf("Exiting program...\n");
        divider();
        puts("");
        break;
      }

    default:
      {
        divider();
        printf("Invalid choice. Please try again.\n");
        divider();
        puts("");
      }

    }
  } while (choice != 6);

  return 0;
}
