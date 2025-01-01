#pragma once

#define MAX_MOVIE_NAME_LENGTH 50
#define MAX_CUSTOMER_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 150
#define MAX_MOBILE_LENGTH 20
#define MAX_RENTALS 10

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

void askID(int *id);
void addCustomer(Customer *customers, int *CustomerNo);
void displayCustomerInfo(Customer *customer);
void listCustomers(Customer *customers, int CustomerNo);
Customer searchCustomers(int id);

void rentMovie(Customer *customer);
void returnMovie(Customer *customer);
