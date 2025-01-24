#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

// prints given text in the center of the screen. used for headings
void printCenter(char string[]) {
  int stringLen = strlen(string);
  int leftLineLen = (SCREEN_WIDTH - 3 - stringLen) / 2;
  int rightLineLen = SCREEN_WIDTH - 3 - stringLen - leftLineLen;

  for (int i = 0; i < leftLineLen - 2; i++) {
    fputs("=", stdout);
  }

  printf(">   %s   <", string);

  for (int i = 0; i < rightLineLen - 2; i++) {
    fputs("=", stdout);
  }

  fputs("\n", stdout);
}

void divider() {
  for (int i = 0; i <= SCREEN_WIDTH; i++) {
    printf("=");
  }
  printf("\n");
}

/*
 * Provide the number of options, followed by a
 * pointer to a list of strings (char *options[]).
 * The text 'enter: <number>' will be appended to
 * the option when printed
 *
 * EXAMPLE:
 *
 * char *options[] = { "For Option 1", "For Option 2", "For Option 3" };
 *
 * int choice = chooseFromOptions(3, options);
 */
int chooseFromOptions(int n, char **options) {
  int lineLen = strlen("Enter number here: ");

  for (int i = 0; i < n; i++) {
    if (strlen(options[i]) > lineLen) {
      lineLen = strlen(options[i]) + strlen("enter: ");
    }
  }

  for (int i = 0; i < n; i++) {
    printf("=> %s enter: ", options[i]);

    for (int j = strlen(options[i]); j < lineLen; j++) {
      fputs("-", stdout);
    }

    printf("< %i\n", i + 1);
  }

  fputs("\n=> Enter number here: ", stdout);
  for (int i = 0; i < lineLen - 11; i++) {
    fputs("-", stdout);
  }
  fputs("< ", stdout);
  int choice;
  scanf("%i", &choice);
  getchar();

  return choice;
}

// returns the Genre variant, for convenience
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

// returns the character representation of the given genre variant
char *getGenreName(Genre genre) {

  switch (genre) {
  case Action:
    return "Action";
  case Romance:
    return "Romance";
  case Comedy:
    return "Comedy";
  case Horror:
    return "Horror";
  case SciFi:
    return "SciFi";
  case Musical:
    return "Musical";
  }
}
