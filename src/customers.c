#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

int alterCustomer(int id, Customer newCustomer) {

  // get all the customers
  Customer allCustomers[MAX_CUSTOMERS];
  int customerCount;
  retrieveCustomers(allCustomers, &customerCount);

  FILE *temp = fopen(TEMP_DB_FILENAME, "wb");

  if (!temp) {
    printf("=> Reading from %s failed, aborting.\n", TEMP_DB_FILENAME);
    return 0;
  }

  // overwrite the customer with this id
  for (int i = 0; i < customerCount; i++) {
    if (allCustomers[i].ID == id) {
      fwrite(&newCustomer, sizeof(Customer), 1, temp);
    } else {
      fwrite(&allCustomers[i], sizeof(Customer), 1, temp);
    }
  }

  fclose(temp);
  remove(CUSTOMER_DB_FILENAME);
  rename(TEMP_DB_FILENAME, CUSTOMER_DB_FILENAME);
  return 1;
}

/// SAFETY: ALWAYS CHECK FOR NULL & REMEMBER TO free()
Customer *searchCustomersByID(int id) {
  Customer *customer = malloc(sizeof(Customer));

  Customer allCustomers[MAX_CUSTOMERS];
  int customerCount;
  retrieveCustomers(allCustomers, &customerCount);

  for (int i = 0; i < customerCount; i++) {
    if (allCustomers[i].ID == id) {
      *customer = allCustomers[i];
      return customer;
    }
  }

  // could not find customer with matching id
  return NULL;
}

/// Sets *allCustomers to the first customer
/// SAFETEY: IF customerCount IS SET TO 0,
/// DO NOT USE allCustomers AS IT IS HANGING
void retrieveCustomers(Customer *allCustomers, int *customerCount) {
  FILE *file = fopen(CUSTOMER_DB_FILENAME, "rb");

  if (file == NULL) {
    printf("Could not locate file %s, attempting to create it\n", CUSTOMER_DB_FILENAME);
    file = fopen(CUSTOMER_DB_FILENAME, "wb");
    if (file) {
      printf("=> File created successfully\n");
      return;
    } else {
      printf("=> Something went wrong, aborting.\n");
      exit(-1);
    }
  }

  *customerCount = 0;
  Customer currentCustomer;
  while (fread(&currentCustomer, sizeof(Customer), 1, file) == 1) {

    allCustomers[*customerCount] = currentCustomer;
    (*customerCount)++;
  }
  fclose(file);
}

/// Append a customer to the db
int saveNewCustomer(Customer customer) {
  FILE *db = fopen(CUSTOMER_DB_FILENAME, "ab");

  // error opening file
  if (!db) {
    printf("=> An error occurred while opening %s, aborting\n", CUSTOMER_DB_FILENAME);
    return 0;
  }

  fwrite(&customer, sizeof(Customer), 1, db);

  fclose(db);
  return 1;
}

void askID(int *id) {
  printf("=> Enter the customers ID number: ----------< ");
  scanf("%d", id);
  getchar();
}

/// Lists every customer in the system
void listCustomers() {
  Customer allCustomers[MAX_CUSTOMERS];
  int count;

  retrieveCustomers(allCustomers, &count);
  for (int i = 0; i < count; i++) {
    displayCustomerInfo(allCustomers[i]);
  }
}

/// Prints customers in a friendly format
void displayCustomerInfo(Customer customer) {

  divider();
  printf("=> Customer information: \n");
  printf("=> ID Number: ------------< %d\n", customer.ID);
  printf("=> Name: -----------------< %s\n", customer.name);
  printf("=> Mobile phone number: --< %s\n", customer.phone);
  printf("=> Address: --------------< %s\n", customer.address);
  printf("=> Pending Charges: ------< $%.2f\n", customer.pendingCharges);
  divider();
  if (customer.rentCount > 0) {

    // get rentals associated with this customer
    int count = 5;
    Rent *rentals = getRentalsByCustomerID(customer.ID, &count);

    // rentCount > 0 yet there are no rentals ??
    if (!rentals) {
      printf("=> An error occurred while reading the customer rented movies");
      customer.rentCount = 0;
      return;
    }

    printf("=> Customer Rental History:\n");

    for (int i = 0; i < customer.rentCount; i++) {
      // TODO: SAFETY

      Movie *movie = searchMoviesByID(rentals[i].movieID);
      if (!movie) {

        // something is wrong, number of rentals != rentCount
        printf("=> Error: rent count is incorrect\n");

        // sanity check:
        // if i = 0 and it fails, there are 0 movies.
        // if i = 1 and it fails, we have already parsed
        // 1 movie, so rentCount should be 1;
        customer.rentCount = i;
        alterCustomer(customer.ID, customer);
        break;
      }

      switch (rentals[i].status) {
      case Active:
        {
          printf("=> -----------------------< %d. '%s' - Active (%d days remaining)\n",
                 i + 1, movie->title, rentals[i].rentDuration);
          break;
        }
      case Returned:
        printf("=> -----------------------< %d. '%s - Returned'\n",
               i + 1, movie->title);
        break;
      case Overdue:
        printf("=> -----------------------< %d. '%s' - OVERDUE\n",
               i + 1, movie->title);
        break;
      }
      free(movie);
    }
    free(rentals);

  } else {
    printf("=> Customer has not rented any movies yet.\n");
  }
  divider();
  fputs("\n\n", stdout);
}

/// Interactively generate a customer, then save to the db
int createCustomer() {

  Customer newCustomer;
  divider();
  printf("=> Create the customers ID number: ----------< ");
  scanf("%d", &newCustomer.ID);
  getchar();
  divider();
  printf("=> What is their full name?: ----------------< ");
  fgets(newCustomer.name, sizeof(newCustomer.name), stdin);
  newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;
  divider();
  printf("=> What is their mobile phone number?: ------< ");
  fgets(newCustomer.phone, sizeof(newCustomer.phone), stdin);
  newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;
  divider();
  printf("=> What is their address?: ------------------< ");
  fgets(newCustomer.address, sizeof(newCustomer.address), stdin);
  newCustomer.address[strcspn(newCustomer.address, "\n")] = 0;
  newCustomer.pendingCharges = 0.0;
  newCustomer.rentCount = 0;

  // ensure that there isn't a customer already with this id
  int customerCount;
  Customer customers[MAX_CUSTOMERS];

  retrieveCustomers(customers, &customerCount);

  bool validID = false;

  while (!validID) {
    validID = true;
    for (int i = 0; i < customerCount; i++) {
      if (customers[i].ID == newCustomer.ID) {
        validID = false;
        break;
      }
    }

    if (!validID) {
      printf("=> This id is already taken, please choose another: ");
      scanf("%d", &newCustomer.ID);
      getchar();
      divider();
    }
  };

  // save to db
  int exitCode = saveNewCustomer(newCustomer);
  if (exitCode == 1) {

    printf("=> All complete, the customer has been added to the Database!\n");
    divider();
    fputs("\n\n", stdout);
    return 1;
  } else {
    printf("Could not create user, exited with status %i", exitCode);
    return exitCode;
  }
}
