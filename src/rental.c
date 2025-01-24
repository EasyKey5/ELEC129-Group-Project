#include "lib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int rentalMenu();
void rentFilm();
void returnMovie();

// RETURNS 0 FOR FAILURE, 1 FOR SUCCESS
int saveNewRental(Rent rent) {

  // check if there is already a rental with this customer and movie
  // (customer <-rents-> film) is a 1:1 relationship
  Rent allRentals[MAX_RENTALS];
  int count = MAX_RENTALS;
  retrieveRentals(allRentals, &count);
  for (int i = 0; i < count; i++) {
    if (allRentals[i].movieID == rent.movieID && allRentals[i].customerID == rent.customerID) {
      printf("=> The customer has already rented out this movie \n");
      return 0;
    }
  }

  // write rental to the file
  FILE *db = fopen(RENTAL_DB_FILENAME, "ab");

  if (!db) {
    printf("=> An error occurred while opening %s, aborting\n", RENTAL_DB_FILENAME);
    return 0;
  }

  fwrite(&rent, sizeof(Rent), 1, db);

  fclose(db);
  return 1;
}

// displays the menu
int rentalMenu() {

  divider();
  printCenter("Rental Desk Menu");
  divider();

  char *options[] = {"To rent a film", "To return a film", "To display all films", "To list all customers", "To exit"};

  return chooseFromOptions(5, options);
}

// console for the staff member in charge of renting out movies
//
// has the ability to rent and return movies, as well as see
// movies and customer details
void rentalDesk() {

  int choice;

  do {

    choice = rentalMenu();
    switch (choice) {

    case 1:
      rentFilm();
      break;

    case 2:
      returnMovie();
      break;

    case 3:
      listAllMovies();
      break;

    case 4:
      listCustomers();
      break;

    case 5:
      printf("Exiting rental desk system.\n");
      break;

    default:
      printf("Invalid choice. Please try again.\n");
    }

  } while (choice != 5);
}

// gets every rental in the db and points `allRentals` to them
void retrieveRentals(Rent *allRentals, int *rentalCount) {

  FILE *file = fopen(RENTAL_DB_FILENAME, "rb");

  if (file == NULL) {
    printf("Could not locate file %s, aborting\n", RENTAL_DB_FILENAME);

    return;
  }

  // fwrite(&cusName, sizeof(int), 1, text_file);
  *rentalCount = 0;
  Rent currentRental;
  while (fread(&currentRental, sizeof(Rent), 1, file) == 1) {

    allRentals[*rentalCount] = currentRental;
    (*rentalCount)++;
  }

  fclose(file);
}

// get every rental associated with a customer by using their id
Rent *getRentalsByCustomerID(int id, int *count) {

  Rent allRentals[MAX_RENTALS];

  int desiredCount = *count;
  // sets count to the number of rentals in the db
  retrieveRentals(allRentals, count);

  Rent *rentsToReturn = malloc(sizeof(Rent) * *count);

  // desiredCount = min(desiredCount, count)
  // this is to ensure we don't read more data than we have in the file
  desiredCount = desiredCount < *count ? desiredCount : *count;

  *count = 0;

  bool found = false;
  for (int i = 0; i < desiredCount; i++) {
    if (allRentals[i].customerID == id) {
      rentsToReturn[*count] = allRentals[i];
      (*count)++;
      found = true;
    }
  }

  // resize to save wasted space
  rentsToReturn = realloc(rentsToReturn, sizeof(Rent) * *count);

  if (found)
    return rentsToReturn;
  else
    return NULL;
};

// interactively generate a new rental, then save to the rental db
void rentFilm() {

  int customerID;
  char filmTitle[MAX_MOVIE_NAME_LENGTH];
  int status;
  int duration;

  printf("\nEnter Customer Pass Number: ");

  scanf("%u", &customerID);

  printf("Enter Film Title to Rent: ");
  scanf(" %[^\n]", filmTitle);

  // TODO: Handle multiple results
  int count = MAX_MOVIES;
  Movie *moviePtr = searchMoviesByTitle(filmTitle, &count);
  if (!moviePtr) {
    printf("COULDN'T FIND MOVIE");
    return;
  }
  Movie movie = *moviePtr;

  printf("Enter the Rent Duration: ");
  scanf("%u", &duration);

  printf("Choose the type of film\n");
  char *typeOptions[] = {"For VHS", "For DVD", "For BlueRay"};

  // choose from options will return 1 - 3 so subtract 1
  // to make it 0 - 2 to ensure the correct variant is selected
  CopyType type = chooseFromOptions(3, typeOptions) - 1;

  // save to rental db
  status = assignMovieToCustomer(movie.id, customerID, type, duration);

  if (status == 1) {
    printf("Film rented successfully!\n");

  } else {
    printf("Error: Unable to rent film. Please check availability or customer status.\n");
  }
}

// interactively alters the rental db to reflect a customer's return
void returnMovie() {

  int customerID;
  char filmTitle[50];

  printf("\nEnter Customer Pass Number: ");
  scanf("%u", &customerID);
  getchar();
  Customer *customer = searchCustomersByID(customerID);
  if (!customer) {

    printf("=> Could not find customer with id %u\n", customerID);
    return;
  }

  printf("Enter Film Title to Return: ");
  scanf("%49[^\n]", filmTitle);
  getchar();

  int count = MAX_MOVIES;
  Movie *movie = searchMoviesByTitle(filmTitle, &count);

  if (!movie) {

    printf("=> Could not find movie called '%s'\n", filmTitle);
    free(customer);
    return;
  }

  // get all the rentals assigned to this customer
  count = MAX_RENTALS;
  Rent *allRentals = getRentalsByCustomerID(customer->ID, &count);

  if (!allRentals) {

    printf("=> Rental does not exist\n");
    free(customer);
    free(movie);
    return;
  }

  // get the rental for *this* movie

  bool found = false;
  Rent thisRental;

  // save the index so we can overwrite the db file using it
  int rentIndex;
  for (int i = 0; i < count; i++) {

    if (allRentals[i].movieID == movie->id) {
      thisRental = allRentals[i];
      rentIndex = i;
      found = true;
      break;
    }
  }

  if (!found) {
    printf("=> Rental does not exist\n");
    free(customer);
    free(movie);
    return;
  }

  // change this record to returned
  thisRental.status = Returned;

  // write altered record to db
  FILE *db = fopen(RENTAL_DB_FILENAME, "wb");
  if (!db) {
    printf("=> Could not read file %s\n", RENTAL_DB_FILENAME);
    free(customer);
    free(movie);
    return;
  }

  fseek(db, sizeof(Rent) * rentIndex, SEEK_SET);
  fwrite(&thisRental, sizeof(Rent), 1, db);

  fclose(db);

  // update the number of copies of the movie
  switch (thisRental.type) {

  case Vhs:
    movie->copies.vhs++;
    break;
  case BlueRay:
    movie->copies.blueRay++;
    break;
  case Dvd:
    movie->copies.dvd++;
    break;
  }

  // alter the movie in the db
  alterMovie(movie->id, *movie);

  printf("=> Movie returned successfully\n");

  free(customer);
  free(movie);
}
