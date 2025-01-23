#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

void listAllMovies() {

  Movie allMovies[MAX_MOVIES];
  int movieCount;

  retrieveMovies(allMovies, &movieCount);
  for (int i = 0; i < movieCount; i++) {

    printMovie(allMovies[i]);
  }
}

void printMovie(Movie movie) {

  printf("\n\n");
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
  printf("\n\n");
}

/// Returns a pointer to the first `count` movies
///
/// Sets count to the number of results
///
/// If an error occurs, sets count to -1
///
/// ALWAYS CHECK FOR NULL
/// REMEMBER TO `free(movie)` when finished
Movie *searchMoviesByTitle(char *query, int *count) {
  // allocate enough memory to hold `count` movies
  Movie *movie = malloc(sizeof(Movie) * (*count));

  int desiredCount = *count;

  Movie allMovies[MAX_MOVIES];

  // this will set count to the number of items in the db
  retrieveMovies(allMovies, count);

  // sets desiredCount to the maximum of count and desiredCount
  // this is so we can't retrieve more movies than there are in
  // the database
  desiredCount = desiredCount < *count ? desiredCount : *count;

  *count = 0;

  for (int i = 0; i < desiredCount; i++) {
    if (!strcmp(allMovies[i].title, query)) {
      printf("MATCH FOUND\n");
      printMovie(allMovies[i]);
      movie[*count] = allMovies[i];
      (*count)++;
    }
  }

  movie = realloc(movie, sizeof(Movie) * *count);

  return movie;
}

Movie *searchMoviesByID(int id) {
  Movie *movie = malloc(sizeof(Movie));

  Movie allMovies[MAX_MOVIES];
  int nMovies;
  retrieveMovies(allMovies, &nMovies);

  for (int i = 0; i < nMovies; i++) {
    if (allMovies[i].id == id) {
      *movie = allMovies[i];
      return movie;
    }
  }

  // could not find movie with matching id
  return NULL;
};

void alterMovie(int id, Movie movie) {
  Movie allMovies[MAX_MOVIES];
  int nMovies;

  retrieveMovies(allMovies, &nMovies);

  // get the index of this movie in the database
  // given we have already found the movie in
  // `searchMovieByID`, this should never fail
  int i;
  for (i = 0; i < nMovies; i++) {
    if (allMovies[i].id == movie.id)
      break;
  }

  printMovie(movie);

  FILE *db = fopen(MOVIE_DB_FILENAME, "wb");

  // move the file pointer to the start
  // of the movie we want to edit
  fseek(db, sizeof(Movie) * i, SEEK_SET);

  // write the altered movie over the top
  // of the one currently stored in the db
  fwrite(&movie, sizeof(Movie), 1, db);

  fclose(db);
}

// TODO: TEST
void deleteMovie(int id) {
  Movie *allMovies;
  int movieCount;

  retrieveMovies(allMovies, &movieCount);

  // write every entry but the one we want to delete to a
  // temp file, then overwrite the database with the temp file

  FILE *temp = fopen(TEMP_DB_FILENAME, "wb");

  // keep track of whether we actually deleted anything
  bool didDelete = false;

  if (temp) {
    for (int i = 0; i < movieCount; i++) {
      if (allMovies[i].id != id) {
        fwrite(&allMovies[i], sizeof(Movie), 1, temp);
        break;
      } else {
        didDelete = true;
      }
    }

    fclose(temp);

    if (didDelete) {

      // delete the database
      remove(MOVIE_DB_FILENAME);

      // rename the temporary file to the database name
      rename(TEMP_DB_FILENAME, MOVIE_DB_FILENAME);
    }
  }
}

void retrieveMovies(Movie *allMovies, int *movieCount) {
  FILE *file = fopen(MOVIE_DB_FILENAME, "rb");

  if (file == NULL) {
    printf("Could not locate file, aborting\n");

    exit(-1);
  }

  // fwrite(&cusName, sizeof(int), 1, text_file);
  *movieCount = 0;
  Movie currentMovie;
  while (fread(&currentMovie, sizeof(Movie), 1, file) == 1) {

    allMovies[*movieCount] = currentMovie;
    (*movieCount)++;
  }
  fclose(file);
}

void saveNewMovie(Movie movie) {
  FILE *file = fopen(MOVIE_DB_FILENAME, "ab");

  // file does not exist
  if (file == NULL) {
    printf("An error occurred, operation aborted.\n");
    return;
  }

  // generate an id for this new movie
  Movie currentMovies[MAX_MOVIES];
  int count = 1;
  retrieveMovies(currentMovies, &count);

  movie.id = 100;
  bool validID = true;
  do {
    for (int i = 0; i < count; i++) {
      if (currentMovies[i].id == movie.id) {

        validID = false;
      } else {
        movie.id++;
      }
    }
  } while (!validID);

  fwrite(&movie, sizeof(Movie), 1, file);

  fclose(file);
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
void returnMovie(Customer *customer, int movieID) {
  if (customer->rentCount == 0) {
    divider();
    printf("=> This customer currently is not renting any movies.\n");
    divider();
    puts("\n");
    return;
  }
  divider();
  printf("=> Customer is currently renting the following movies:\n");
  for (int i = 0; i < customer->rentCount; i++) {
    Movie *movie = searchMoviesByID(customer->rentHistory[i].movieID);
    printf("=> -------< %d. %s (Rented movie for: %d days)\n",
           i + 1, movie->title, customer->rentHistory[i].rentDuration);
    free(movie);
  }

  int rentalIndex;
  divider();
  printf("=> What movie are they returning?\n");
  printf("=> Enter the moivie number here: ------------<");
  scanf("%d", &rentalIndex);
  rentalIndex--;

  if (rentalIndex < 0 || rentalIndex >= customer->rentCount) {
    divider();
    printf("=> Invalid selection.\n");
    divider();
    puts("\n");
    return;
  }

  float rentalCharge = customer->rentHistory[rentalIndex].rentDuration * 1;
  customer->pendingCharges -= rentalCharge;

  // Shift the rental history to remove the returned film
  for (int i = rentalIndex; i < customer->rentCount - 1; i++) {
    customer->rentHistory[i] = customer->rentHistory[i + 1];
  }
  customer->rentCount--;
  divider();
  printf("=> Movie has been returned successfully and removed from their account!\n");
  printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
  divider();
  puts("\n");
}

// // TODO: alter implementation to use `Movie` type
// void rentMovie(Customer *customer) {
//   if (customer->rentCount >= MAX_RENTALS) {
//     divider();
//     printf("=> Customer has reached maximum amount of rentals permitted,\n");
//     printf("=> They must return any loaned out movies before being allowed to rent more.\n");
//     divider();
//     return;
//   }
//
//   Rent newRental;
//   divider();
//   printf("=> What is the name of the movie?: ----------< ");
//   Movie *movie = searchMoviesByID(newRental.movieID);
//   fgets(newRental.Movie, sizeof(newRental.Movie), stdin);
//   newRental.Movie[strcspn(newRental.Movie, "\n")] = 0;
//   divider();
//   printf("=> How many days will they rent the movie?:--< ");
//   scanf("%d", &newRental.rentTime);
//   getchar();
//
//   customer->rentHistory[customer->rentCount] = newRental;
//   customer->rentCount++;
//
//   float rentalCharge = newRental.rentTime * 1; // Im makinng the price a £1 per day to rnt idk how rental prices work so just change if needed
//   customer->pendingCharges += rentalCharge;
//   divider();
//   printf("=> Movie has been rented successfully and added to their account!\n");
//   printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
//   divider();
// }
