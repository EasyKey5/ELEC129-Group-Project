#pragma once

#define MAX_MOVIES 20
#define SCREEN_WIDTH 100
#define MAX_CUSTOMERS 50
#define MAX_ACTORS 15
#define MAX_MOVIE_NAME_LENGTH 75
#define MAX_ACTOR_NAME_LENGTH 50
#define MAX_CUSTOMER_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 150
#define MAX_MOBILE_LENGTH 20
#define MAX_RENTALS 10

typedef struct Copies {
  int vhs;
  int dvd;
  int blueRay;
} Copies;

typedef enum Genre {
  Action,
  Comedy,
  Horror,
  Musical,
  Romance,
  SciFi
} Genre;

typedef struct Movie {
  int id;
  char title[MAX_MOVIE_NAME_LENGTH];
  char *actors[MAX_ACTORS];
  int nActors;
  Copies copies;
  Genre genre;

} Movie;

typedef struct Rent {
  char Movie[MAX_MOVIE_NAME_LENGTH];
  int rentTime;
} Rent;

typedef struct Customer {
  int ID;
  char name[MAX_CUSTOMER_NAME_LENGTH];
  char phone[MAX_MOBILE_LENGTH];
  char address[MAX_ADDRESS_LENGTH];
  float pendingCharges;
  Rent rentHistory[MAX_RENTALS];
  int rentNo;
} Customer;

// Utils
void printCenter(char string[]);
void divider();
int chooseFromOptions(int n, char **options);

// Customers
void askID(int *id);
void addCustomer(Customer *customers, int *CustomerNo);
void displayCustomerInfo(Customer *customer);
void listCustomers(Customer *customers, int CustomerNo);
Customer searchCustomers(int id);
char *getGenreName(Genre genre);

// Movies

// sets nMovies to number of movies and points allMovies to the first
void retrieveMovies(Movie *allMovies, int *nMovies);

/// searches for the first `count` movies
///
/// returns a pointer to the movie and sets `count` to the number of results
Movie *searchMoviesByTitle(char *query, int *count);
Movie *searchMoviesByID(int id);

void deleteMovie(int id);

void printMovie(Movie movie);
Genre pickGenre();
void rentMovie(Customer *customer);
void returnMovie(Customer *customer);
void saveMovie(Movie movie);

// Interfaces
void managementConsole();
void rentalDesk();
void registrationDesk();
