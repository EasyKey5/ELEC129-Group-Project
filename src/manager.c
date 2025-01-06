#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

void managementConsole() {

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
      "To alter charges"};

  int choice = chooseFromOptions(4, options);

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
      char input[MAX_ACTOR_NAME_LENGTH] = "";

      for (newMovie.nActors = 0; newMovie.nActors < MAX_ACTORS; newMovie.nActors++) {
        if (!strcmp(input, "q")) // strings are equal
          break;

        printf("=> Enter actor %u: --< ", newMovie.nActors + 1);
        scanf("%[^\n]", input);
        getchar();

        newMovie.actors[newMovie.nActors] = input;
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

      saveMovie(newMovie);
      break;
    };

  case 2: // Delete
    {
      char title[MAX_MOVIE_NAME_LENGTH];

      printf("=> Enter the name of the movie which you would like to delete: --< ");

      scanf("%[^\n]", title);

      if (strcmp(title, "q") == 0)
        break;

      int count = 5;
      Movie *movieResults = searchMoviesByTitle(title, &count);

      if (movieResults) {
        if (count == 1) { // only one result found

          printf("=> Movie Found:\n\n");
          printMovie(movieResults[0]);
          printf("\n=> Delete this movie [y/n]: --< ");
          char answer[10] = "";
          scanf("%s", answer);
          if (strcmp(answer, "y") == 0)
            deleteMovie(movieResults->id);
          break;
        } else if (count > 1) { // many results

          printf("=> %u results found: \n", count);
          for (int i = 0; i < count - 1; i++) {
            divider();
            printf("=> Movie %u:\n\n", i + 1);
            printMovie(movieResults[i]);
          }
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

    }
  }
}
