#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

// Lists every movie in the db
void listAllMovies() {

  Movie allMovies[MAX_MOVIES];
  int movieCount;

  retrieveMovies(allMovies, &movieCount);
  for (int i = 0; i < movieCount; i++) {

    printMovie(allMovies[i]);
  }
}

// friendly display for a movie
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

/* Returns a pointer to the first `count` movies
 *
 * Sets count to the number of results
 *
 * If an error occurs, returns NULL
 *
 * ALWAYS CHECK FOR NULL
 * REMEMBER TO `free(movie)` when finished
 *
 * Parameters:
 * char *query - title to search for
 * int *count - the maximum of records to search through
 */
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

  bool found = false;
  for (int i = 0; i < desiredCount; i++) {
    if (!strcmp(allMovies[i].title, query)) {
      printf("MATCH FOUND\n");
      printMovie(allMovies[i]);
      movie[*count] = allMovies[i];
      (*count)++;
      found = true;
    }
  }

  // resize movie to the max it needs
  movie = realloc(movie, sizeof(Movie) * *count);

  if (found)
    return movie;
  else
    return NULL;
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

// replaces movie that currently has id `id` with `movie`
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

// remove a movie from the database
void deleteMovie(int id) {
  Movie allMovies[MAX_MOVIES];
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

// gets all currently stored movies and points `allMovies` to them
void retrieveMovies(Movie *allMovies, int *movieCount) {
  FILE *file = fopen(MOVIE_DB_FILENAME, "rb");

  if (file == NULL) {
    printf("Could not locate file, aborting\n");

    return;
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

// store a movie in the db
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

  // start ids from 100, then increase sequentially
  // until a unique id is found
  movie.id = 100;
  bool validID = true;
  do {
    for (int i = 0; i < count; i++) {
      if (currentMovies[i].id == movie.id) {

        validID = false;
        movie.id++;
      } else {
        validID = true;
        break;
      }
    }
  } while (!validID);

  fwrite(&movie, sizeof(Movie), 1, file);

  fclose(file);
}

// RETURNS 0 FOR FAILURE, 1 FOR SUCCESS
int assignMovieToCustomer(int movieID, int customerID, CopyType type, int duration) {

  // generate a rent struct
  Rent newRental = (Rent){
      .movieID = movieID,
      .customerID = customerID,
      .type = type,
      .rentDuration = duration,
      .status = Active};

  // store the rent struct in the database

  int status = saveNewRental(newRental);

  if (status == 0) {
    printf("=> Writing to file failed, exiting\n");
    return 1;
  }

  // alter the amount of copies of the movie accordingly

  Movie *movie = searchMoviesByID(movieID);
  if (!movie) {
    printf("Movie does not exist, exiting\n");
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

  // update the db
  alterMovie(movieID, *movie);

  free(movie);

  // increase the rentCount of the user
  Customer *customer = searchCustomersByID(customerID);

  if (!customer) {
    printf("=> Could not find customer, aborting");
    return 0;
  }

  customer->rentCount++;
  alterCustomer(customer->ID, *customer);

  free(customer);

  return 1;
}
