#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

// console for manager to use (ability to add, alter and delete movies)
// charges are currently not implemented
void managementConsole() {

  int choice = 6;

  do {
    divider();
    printCenter("Welcome to the");
    printCenter("Management Console!");
    divider();

    Movie allMovies[MAX_MOVIES];
    int existingMovies;

    retrieveMovies(allMovies, &existingMovies);

    char *options[] = {
        "To add a new movie",
        "To delete a movie",
        "To alter a movie",
        "To list all movies",
        "To alter charges",
        "To exit"};

    choice = chooseFromOptions(6, options);

    divider();

    switch (choice) {

    case 1: // Add new movie
      {
        Movie newMovie;

        divider();
        divider();

        printf("=> Enter the title of the movie: --< ");
        scanf("%[^\n]", newMovie.title);
        getchar();

        printf("=> Enter the genre of the movie:\n");
        newMovie.genre = pickGenre();

        printf("=> Enter the names of the actors or \"q\" to finish\n");

        for (newMovie.nActors = 0; newMovie.nActors < MAX_ACTORS; newMovie.nActors++) {

          char input[MAX_ACTOR_NAME_LENGTH] = "";

          printf("=> Enter actor %u: --< ", newMovie.nActors + 1);
          scanf("%[^\n]", input);
          getchar();

          // type "q" to exit
          if (!strcmp(input, "q")) {
            break;
          }

          // set the actors name in the struct to the input
          strncpy(newMovie.actors[newMovie.nActors], input, MAX_ACTOR_NAME_LENGTH);
        }

        printf("=> Enter the number of VHS copies: ------< ");
        scanf("%u", &newMovie.copies.vhs);
        getchar();

        printf("=> Enter the number of dvd copies: ------< ");
        scanf("%u", &newMovie.copies.dvd);
        getchar();

        printf("=> Enter the number of BlueRay copies: --< ");
        scanf("%u", &newMovie.copies.blueRay);
        getchar();

        // save to the db
        saveNewMovie(newMovie);
        break;
      };

    case 2: // Delete
      {
        char title[MAX_MOVIE_NAME_LENGTH];

        printf("=> Enter the name of the movie which you would like to delete: --< ");
        scanf("%[^\n]", title);
        getchar();

        if (strcmp(title, "q") == 0)
          break;

        int count = MAX_MOVIES;
        Movie *movieResults = searchMoviesByTitle(title, &count);

        if (movieResults) {
          if (count == 1) { // only one result found

            printf("\n=> Delete this movie [y/n]: --< ");
            char answer[10] = "";
            scanf("%s", answer);
            if (strcmp(answer, "y") == 0)
              deleteMovie(movieResults->id);
            break;
          } else if (count > 1) { // many results

            printf("=> %u results found: \n", count);
            divider();
            int id;
            printf("=> Enter the id of the movie you would like to delete: --< ");
            scanf("%u", &id);
            deleteMovie(id);
            break;
          } else if (count == 0) { // No results

            printf("=> No results found, please type the title again: --< ");
          } else { // UNKNOWN ERROR: results < 0

            fprintf(stderr, "=> ERROR: Search failed returning %i\n", count);
            break;
          }
          free(movieResults);
        }

        divider();
        break;
      }

    case 3:
      { // Alter a movie

        divider();
        printf("=> Enter the title of the movie you would like to alter: --< ");
        char query[MAX_MOVIE_NAME_LENGTH];
        scanf("%[^\n]", query);

        int count = MAX_MOVIES;

        int idToAlter = -1;
        // WARNING: DO NOT USE THIS VARIABLE IF SEARCH FAILS
        Movie *results = searchMoviesByTitle(query, &count);
        if (results) {
          if (count < 0) {
            fprintf(stderr, "=> ERROR: Unknown search error\n");
            return;
          } else if (count == 1) {

            idToAlter = results->id;
          } else { // Multiple results, select one

            printf("=> Multiple films found:\n");
            for (int i = 0; i < count; i++) {
              printMovie(results[i]);
              printf("\n");
            }
            printf("=> Enter the id of the movie you would like to alter: --< ");
            int id;
            scanf("%u", &id);
            printf("\n");

            idToAlter = id;
          }

          Movie *moviePtr = searchMoviesByID(idToAlter);

          // check if there was a result
          if (!moviePtr) {
            printf("=> Movie with id %u could not be found\n", idToAlter);
            return;
          }

          Movie movie = *moviePtr;

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
              scanf("%[^\n]", movie.title);
              break;
            }

          case 2: // actors
            {
              printf("=> Enter the names of the actors or \"q\" to finish\n");

              for (movie.nActors = 0; movie.nActors < MAX_ACTORS; movie.nActors++) {

                char input[MAX_ACTOR_NAME_LENGTH] = "";

                printf("=> Enter actor %u: --< ", movie.nActors + 1);
                scanf("%[^\n]", input);
                getchar();

                // "q" to exit
                if (!strcmp(input, "q")) {
                  break;
                }

                strncpy(movie.actors[movie.nActors], input, MAX_ACTOR_NAME_LENGTH);
              }

              break;
            }

          case 3: // copies
            {
              printf("=> Enter the number of VHS copies: ------< ");
              scanf("%u", &movie.copies.vhs);
              getchar();

              printf("=> Enter the number of dvd copies: ------< ");
              scanf("%u", &movie.copies.dvd);
              getchar();

              printf("=> Enter the number of BlueRay copies: --< ");
              scanf("%u", &movie.copies.blueRay);
              getchar();
              break;
            }

          case 4: // genre
            {
              movie.genre = pickGenre();
              break;
            }
          }

          // update the db
          alterMovie(idToAlter, movie);

          free(results);
          free(moviePtr);
          break;
        }
      }
    case 4: // list all movies
      {
        listAllMovies();
        break;
      }
    case 5: // altering charges
      // TODO: IMPLEMENT
      printf("=> Altering charges is not yet implemented");
      break;
    case 6:
      printf("=> Exiting management console");
      return;
    }
  } while (choice != 6);
}
