#include <stdio.h>
#include <stdlib.h>
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
/// Returns a pointer to the first `count` movies
///
/// Sets count to the number of results
///
/// If an error occurs, sets count to -1
///
/// ALWAYS CHECK FOR NULL
/// REMEMBER TO `free(movie)` when finished
Movie *searchMoviesByTitle(char *query, int *count) {
  Movie *movie = malloc(sizeof(Movie) * *count);
  *movie =
      (Movie){
          .id = 1,
          .title = "Jumanji: Welcome to the jungle",
          .actors = {"Dwayne Johnson", "Jack Black", "Kevin Hart"},
          .nActors = 3,
          .genre = Action,
          .copies = (Copies){.dvd = 3, .vhs = 1, .blueRay = 5}};

  // simulating only one result
  *count = 1;
  movie = realloc(movie, sizeof(Movie) * *count);

  return movie;
}

// TODO: IMPLEMENT
///
/// Returns a reference to the movie struct
/// If movie doesn't exist, pointer is null
///
/// ALWAYS CHECK FOR NULL
Movie *getMovieByID(int id) {
  Movie *movie = malloc(sizeof(Movie));
  *movie =
      (Movie){
          .id = 1,
          .title = "Jumanji: Welcome to the jungle",
          .actors = {"Dwayne Johnson", "Jack Black", "Kevin Hart"},
          .nActors = 3,
          .genre = Action,
          .copies = (Copies){.dvd = 3, .vhs = 1, .blueRay = 5}};

  return movie;
};

void alterMovie(int id) {
  Movie *movie = getMovieByID(id);
  if (movie) {

    char *alterOptions[] = {
        "To edit the title",
        "To edit the actors",
        "To edit the copies",
        "To change the genre"

    };
    int choice = chooseFromOptions(4, alterOptions);

    divider();

    switch (choice) {
    case 1: // title
      {
        printf("=> Enter the new title: --< ");
        scanf("%[^\n]", movie->title);
        break;
      }

    case 2: // actors
      {
        printf("=> Enter the names of the actors or \"q\" to finish\n");
        char input[MAX_ACTOR_NAME_LENGTH] = "";

        for (movie->nActors = 0; movie->nActors < MAX_ACTORS; movie->nActors++) {
          if (!strcmp(input, "q")) // strings are equal
            break;

          printf("=> Enter actor %u: --< ", movie->nActors + 1);
          scanf("%[^\n]", input);
          getchar();

          movie->actors[movie->nActors] = input;
        }
        break;
      }

    case 3: // copies
      {
        printf("=> Enter the number of VHS copies: ------< ");
        scanf("%u", &movie->copies.vhs);
        getchar();

        printf("=> Enter the number of dvd copies: ------< ");
        scanf("%u", &movie->copies.dvd);
        getchar();

        printf("=> Enter the number of BlueRay copies: --< ");
        scanf("%u", &movie->copies.blueRay);
        getchar();
        break;
      }

    case 4: // genre
      {
        movie->genre = pickGenre();
        break;
      }
    }
    saveMovie(*movie);
    free(movie);
  }
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

  int choice = chooseFromOptions(6, options);

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
