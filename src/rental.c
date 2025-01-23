#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

int displayMenu();
void rentFilm();
void returnFilm();

int saveNewRental(Rent rent) {
  FILE *db = fopen(RENTAL_DB_FILENAME, "ab");

  if (!db) {
    printf("=> An error occurred while opening %s, aborting\n", RENTAL_DB_FILENAME);
    return 0;
  }

  fwrite(&rent, sizeof(Rent), 1, db);

  fclose(db);
  return 1;
}

int assignMovieToCustomer(int movieID, int customerID, CopyType type, int duration) {

  // generate a rent struct
  Rent newRental = (Rent){
      .movieID = movieID,
      .customerID = customerID,
      .type = type,
      .rentDuration = duration};

  // alter the amount of copies of the movie accordingly

  Movie *movie = searchMoviesByID(movieID);
  if (!movie) {
    printf("Movie does not exist, exiting");
    return 0;
  }

  switch (type) {

  case Vhs:

    if (movie->copies.vhs > 0) {

      movie->copies.vhs--;
    } else {
      printf("=> There are no copies left\n");
      return 0;
    }

    break;
  case Dvd:
    if (movie->copies.dvd > 0) {

      movie->copies.dvd--;
    } else {
      printf("=> There are no copies left\n");
      return 0;
    }
    break;
  case BlueRay:
    if (movie->copies.blueRay > 0) {

      movie->copies.blueRay--;
    } else {
      printf("=> There are no copies left\n");
      return 0;
    }
    break;
  }

  alterMovie(movieID, *movie);

  free(movie);

  // store the rent struct in the database

  int status = saveNewRental(newRental);

  if (status == 0) {
    printf("=> Writing to file failed, exiting\n");
    exit(-1);
  }
  // add the rent to the customer's struct

  Customer *customer = searchCustomersByID(customerID);

  if (!customer) {
    printf("=> Searching for customer with id %u failed\n", customerID);
    return 0;
  }

  // TODO: Implement a queue
  // if the rent count is greater than the max,
  // wrap around and overwrite the first rental
  // to avoid a buffer overflow
  customer->rentCount %= MAX_RENTALS;

  customer->rentHistory[customer->rentCount] = newRental;
  customer->rentCount++;

  alterCustomer(customerID, *customer);

  free(customer);

  return 1;
}

void rentalDesk() {

  int choice;

  do {

    choice = displayMenu();
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

int displayMenu() {

  divider();
  printCenter("Rental Desk Menu\n");
  divider();

  char *options[] = {"To rent a film", "To return a film", "To exit"};

  return chooseFromOptions(3, options);
}

void rentFilm() {

  int customerID;
  char filmTitle[MAX_MOVIE_NAME_LENGTH];
  int status;
  int duration;

  printf("\nEnter Customer Pass Number: ");

  scanf("%u", &customerID);

  printf("Enter Film Title to Rent: ");
  scanf(" %[^\n]", filmTitle);

  printf("Enter the Rent Duration: ");
  scanf("%u", &duration);

  printf("Choose the type of film\n");
  char *typeOptions[] = {"For VHS", "For DVD", "For BlueRay"};
  CopyType type = chooseFromOptions(3, typeOptions);

  int count = 1;

  // TODO: Handle multiple results
  Movie *moviePtr = searchMoviesByTitle(filmTitle, &count);
  if (!moviePtr) {
    printf("COULDN'T FIND MOVIE");
    return;
  }
  Movie movie = *moviePtr;

  printMovie(movie);

  status = assignMovieToCustomer(movie.id, customerID, type, duration);

  if (status == 1) {
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
  // success = assignMovieToCustomer(movie.id, customerID);

  if (success) {
    printf("Film returned successfully!\n");

  } else {
    printf("Error: Unable to process return. Please verify the title and customer information.\n");
  }
}
