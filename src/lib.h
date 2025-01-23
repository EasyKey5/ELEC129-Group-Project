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

#define MOVIE_DB_FILENAME "./data/movie.dat"
#define CUSTOMER_DB_FILENAME "./data/customer.dat"
#define TEMP_DB_FILENAME "./data/temp.dat"

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
  char actors[MAX_ACTORS][MAX_ACTOR_NAME_LENGTH];
  int nActors;
  Copies copies;
  Genre genre;

} Movie;

// TODO: Change API to use `Movie` type
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
int createCustomer();
void saveNewCustomer(Customer customer);
void displayCustomerInfo(Customer customer);
void retrieveCustomers(Customer *allCustomers, int *customerCount);
Customer searchCustomers(int id);

// Movies

// sets nMovies to number of movies and points allMovies to the first
void retrieveMovies(Movie *allMovies, int *movieCount);

/// searches for the first `count` movies
///
/// returns a pointer to the movie and sets `count` to the number of results
Movie *searchMoviesByTitle(char *query, int *count);
Movie *searchMoviesByID(int id);

void deleteMovie(int id);
int assignMovieToCustomer(int movieID, int customerID);

void printMovie(Movie movie);
Genre pickGenre();
void alterMovie(int id);
void rentMovie(Customer *customer);
void returnMovie(Customer *customer);
void saveNewMovie(Movie movie);

char *getGenreName(Genre genre);
// Interfaces
void managementConsole();
void rentalDesk();
void registrationDesk();
