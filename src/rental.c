#include "lib.h"
#include <stdio.h>

void displayMenu();
void rentFilm();
void returnFilm();

int assignMovieToCustomer(int movieID, int customerID) {
  return 1;
};

void rentalDesk() {

  int choice;

  do {

    displayMenu();
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    switch (choice) {

    case 1:
      rentFilm();
      break;

    case 2:
      returnFilm();
      break;

    case 3:
      printf("Exiting rental desk system.\n");
      break;

    default:
      printf("Invalid choice. Please try again.\n");
    }

  } while (choice != 3);
}

void displayMenu() {

  printf("\n--- Rental Desk Menu ---\n");

  printf("1. Rent a Film\n");

  printf("2. Return a Film\n");

  printf("3. Exit\n");
}

void rentFilm() {

  int customerID;

  char filmTitle[50];

  int success;

  printf("\nEnter Customer Pass Number: ");

  scanf("%u", &customerID);

  printf("Enter Film Title to Rent: ");

  scanf(" %49[^\n]", filmTitle);

  int count = 1;

  // TODO: implement safety for getting multiple results

  Movie movie = searchMoviesByTitle(filmTitle, &count)[0];
  printMovie(movie);

  success = assignMovieToCustomer(movie.id, customerID);

  if (success) {
    printf("Film rented successfully!\n");

  } else {
    printf("Error: Unable to rent film. Please check availability or customer status.\n");
  }
}

void returnFilm() {

  int customerID;
  char filmTitle[50];
  int success;

  printf("\nEnter Customer Pass Number: ");
  scanf("%u", &customerID);
  printf("Enter Film Title to Return: ");
  scanf(" %49[^\n]", filmTitle);

  // FIXME: handle multiple results
  int count = 1;
  Movie movie = searchMoviesByTitle(filmTitle, &count)[0];
  printMovie(movie);

  // FIXME: FIX
  success = assignMovieToCustomer(movie.id, customerID);

  if (success) {
    printf("Film returned successfully!\n");

  } else {
    printf("Error: Unable to process return. Please verify the title and customer information.\n");
  }
}
