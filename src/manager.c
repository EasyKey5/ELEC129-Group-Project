#include <stdio.h>
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

  int choice;
  do {
    char *options[] = {
        "To add a new movie",
        "To delete a movie",
        "To alter a movie",
        "To alter charges"};

    chooseFromOptions(&choice, 4, options);

    switch (choice) {

    case 1: // Add new movie
      {
        Movie newMovie;

        divider();
        divider();

        printf("=> Enter the title of the movie: --< ");
        scanf("%[^\n]", newMovie.title);

        printf("=> Enter the genre of the movie:\n");
        newMovie.genre = pickGenre();

        printf("=> Enter the names of the actors or \"q\" to finish\n");
        char input[MAX_ACTOR_NAME_LENGTH];

        for (newMovie.nActors = 0; newMovie.nActors < MAX_ACTORS; newMovie.nActors++) {
          if (!strcmp(input, "q")) // strings are equal
            break;

          printf("=> Enter actor %u: --< ", newMovie.nActors + 1);
          scanf("%[^\n]", input);
          getchar();

          newMovie.actors[newMovie.nActors] = input;
        }

        printf("=> Enter the number of VHS copies: --< ");
        scanf("%u", &newMovie.copies.vhs);

        printf("=> Enter the number of dvd copies: --< ");
        scanf("%u", &newMovie.copies.dvd);

        printf("=> Enter the number of BlueRay copies: --< ");
        scanf("%u", &newMovie.copies.blueRay);

        saveMovie(newMovie);
      }
    }
  } while (choice > 0 && choice <= 4);
}
