#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

int main() {

  divider();
  printCenter("Welcome back to the");
  printCenter("Snazzy Video Rental System!");
  printCenter("Who are you?");
  divider();
  divider();

  char *modeOptions[3] = {"For Manager", "For Registration", "For Sales Clerk"};

  int choice = chooseFromOptions(3, modeOptions);

  // Main menu
  switch (choice) {

  case 1:
    {
      managementConsole();
      break;
    }

  case 2:
    {
      registrationDesk();
      break;
    }

  case 3:
    {
      rentalDesk();
      break;
    }
  default:
    {
      printf("Invalid input, exiting");
      return 1;
    }
  }
  return 0;
}
