#include <stdio.h>
#include <string.h>

#include "lib.h"

void returnMovie(Customer *customer) {
  if (customer->rentNo == 0) {
    divider();
    printf("=> This customer currently is not renting any movies.\n");
    divider();
    puts("\n");
    return;
  }
  // prin("=> To exit enter: ---------------------------< 6 \n");
  divider();
  printf("=> Customer is currently renting the following movies:\n");
  for (int i = 0; i < customer->rentNo; i++) {
    printf("=> -------< %d. %s (Rented movie for: %d days)\n", i + 1, customer->rentHistory[i].Movie, customer->rentHistory[i].rentTime);
  }

  int rentalIndex;
  divider();
  printf("=> What movie are they returning?\n");
  printf("=> Enter the moivie number here: ------------<");
  scanf("%d", &rentalIndex);
  rentalIndex--;

  if (rentalIndex < 0 || rentalIndex >= customer->rentNo) {
    divider();
    printf("=> Invalid selection.\n");
    divider();
    puts("\n");
    return;
  }
  float rentalCharge = customer->rentHistory[rentalIndex].rentTime * 1;
  customer->pendingCharges -= rentalCharge;

  // Shift the rental history to remove the returned film
  for (int i = rentalIndex; i < customer->rentNo - 1; i++) {
    customer->rentHistory[i] = customer->rentHistory[i + 1];
  }
  customer->rentNo--;
  divider();
  printf("=> Movie has been returned successfully and removed from their account!\n");
  printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
  divider();
  puts("\n");
}

void rentMovie(Customer *customer) {
  if (customer->rentNo >= MAX_RENTALS) {
    divider();
    printf("=> Customer has reached maximum amount of rentals permitted,\n");
    printf("=> They must return any loaned out movies before being allowed to rent more.\n");
    divider();
    return;
  }

  Rent newRental;
  divider();
  printf("=> What is the name of the movie?: ----------< ");
  fgets(newRental.Movie, sizeof(newRental.Movie), stdin);
  newRental.Movie[strcspn(newRental.Movie, "\n")] = 0;
  divider();
  printf("=> How many days will they rent the movie?:--< ");
  scanf("%d", &newRental.rentTime);
  getchar();

  customer->rentHistory[customer->rentNo] = newRental;
  customer->rentNo++;

  float rentalCharge = newRental.rentTime * 1; // Im makinng the price a £1 per day to rnt idk how rental prices work so just change if needed
  customer->pendingCharges += rentalCharge;
  divider();
  printf("=> Movie has been rented successfully and added to their account!\n");
  printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
  divider();
}
