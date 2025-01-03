#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

int main() {

  Customer customers[50];
  int existingCustomers = 0;
  int choice;

  puts("");

  divider();
  printCenter("12");
  printCenter("123");
  printCenter("Welcome back to the");
  printCenter("Snazzy Video Rental System!");
  printCenter("Who are you?");
  divider();
  divider();
  char *modeOptions[3] = {"For Manager", "For Registration", "For Sales Clerk"};

  chooseFromOptions(&choice, 3, modeOptions);
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

    printf("=> To create a new customer account enter: --< 1 \n");
    printf("==============================================================================================\n");
    printf("=> To rent a movie enter: -------------------< 2 \n");
    printf("==============================================================================================\n");
    printf("=> To return a movie enter: -----------------< 3 \n");
    printf("==============================================================================================\n");
    printf("=> To view customer details enter: ----------< 4 \n");
    printf("==============================================================================================\n");
    printf("=> To view a list of all customers enter: ---< 5 \n");
    printf("==============================================================================================\n");
    printf("=> To exit enter: ---------------------------< 6 \n");
    printf("==============================================================================================\n\n");
    printf("==============================================================================================\n");
    printf("=> Enter number here: -----------------------< ");
    scanf("%d", &choice);
    printf("==============================================================================================\n\n");
    getchar();

    switch (choice) {

    case 1:
      addCustomer(customers, &existingCustomers);
      break;

    case 2: {
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

    case 3: {
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

    case 4: {
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
      listCustomers(customers, existingCustomers);
      break;

    case 6:
      printf("==============================================================================================\n");
      printf("Exiting program...\n");
      printf("==============================================================================================\n\n\n");
      break;

    default:
      printf("==============================================================================================\n");
      printf("Invalid choice. Please try again.\n");
      printf("==============================================================================================\n\n\n");
    }
  } while (choice != 6);

  return 0;
}