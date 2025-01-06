#include <stdio.h>
#include <string.h>

#include "lib.h"

void printMovie(Movie movie) {

  printf("=> Title: %s\n", movie.title);
  printf("=> ID: %i\n", movie.id);
  printf("=> Genre: %s\n", getGenreName(movie.genre));
  printf("=> Actors:\n");
  for (int i = 0; i < movie.nActors; i++) {
    printf("===> %s\n", movie.actors[i]);
  }
  printf("=> Copies:\n");
  printf("===> DVD: %i\n", movie.copies.dvd);
  printf("===> VHS: %i\n", movie.copies.vhs);
  printf("===> BlueRay: %i\n", movie.copies.blueRay);
}

/// TODO: Implement proper search function
///
/// Returns the number of results and points `result` to
/// the first one (if there are any).
///
/// SAFETY: If no results are found, `result` is uninitialised
/// ERRORS: Any errors will be caught and return a negative number
///         (This must be checked and guarded by the caller)
int searchMoviesByTitle(char *query, Movie *result) {

  Movie movie =
      (Movie){
          .id = 1,
          .title = "Jumanji: Welcome to the jungle",
          .actors = {"Dwayne Johnson", "Jack Black", "Kevin Hart"},
          .nActors = 3,
          .genre = Action,
          .copies = (Copies){.dvd = 3, .vhs = 1, .blueRay = 5}};

  result[0] = movie;

  return 1;
}

// TODO: Implement
void deleteMovie(int id) {
  printf("=> Deleted movie %u\n", id);
}

// TODO: Implement
void retrieveMovies(Movie *allMovies, int *nMovies) {
}

// TODO: Implement
void saveMovie(Movie movie) {
  puts("=> Saving movie ...");
}

Genre pickGenre() {
  char *options[6] = {
      "For Action",
      "For Comedy",
      "For Horror",
      "For Musical",
      "For Romance",
      "For SciFi",
  };

  int choice =
      chooseFromOptions(6, options);

  return choice - 1;
}

// TODO: alter implementation to use `Movie` type
void returnMovie(Customer *customer) {
  if (customer->rentNo == 0) {
    divider();
    printf("=> This customer currently is not renting any movies.\n");
    divider();
    puts("\n");
    return;
  }
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

// TODO: alter implementation to use `Movie` type
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
