#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

// printf("=================================>   Welcome back to the   <==================================\n");
// printf("=============================>   Snazzy Video Rental Customer   <=============================\n");
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

  fputs("=> Enter number here: ", stdout);
  for (int i = 0; i < lineLen - 11; i++) {
    fputs("-", stdout);
  }
  fputs("< ", stdout);
  int choice;
  scanf("%i", &choice);
  getchar();

  return choice;
}

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
