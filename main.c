#include <stdio.h>
#include <stdlib.h>

int main()
{

	return 0;

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MOVIE_NAME_LENGTH 50
#define MAX_CUSTOMER_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 150
#define MAX_MOBILE_LENGTH 20
#define MAX_RENTALS 10
typedef struct {
    char Movie[MAX_MOVIE_NAME_LENGTH];
    int rentTime;
} Rent;
typedef struct {
    int ID;                 
    char name[MAX_CUSTOMER_NAME_LENGTH];
    char phone[MAX_MOBILE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    float pendingCharges;              
    Rent rentHistory[MAX_RENTALS];
    int rentNo;                    
} Customer;
void addCustomer(Customer* customers, int* CustomerNo);
void rentmovie(Customer* customer);
void returnmovie(Customer* customer);
void DisplayCustomerInfo(Customer* customer);
void listCustomers(Customer* customers, int CustomerNo);
int main() {
    Customer customers[50];  // Array can store however many i just put 50 so change if u need
    int CustomerNo = 0;
    int choice;
    do {
		printf("==============================================================================================\n\n");
		printf("==============================================================================================\n");
		printf("=================================>   Welcome back to the   <==================================\n");
        printf("=============================>   Snazzy Video Rental Customer   <=============================\n");
        printf("===================================>      Database!      <====================================\n");
		printf("==============================================================================================\n\n");
		printf("==============================================================================================\n");
        printf("================================> What would you like to do ? <===============================\n");
		printf("==============================================================================================\n\n");  
		printf("==============================================================================================\n");
        printf("=> To create a new customer account enter: --< 1 \n");
        printf("==============================================================================================\n");
        printf("=> To rent a movie enter: -------------------< 2 \n");
        printf("==============================================================================================\n");
        printf("=> To return a movie enter: -----------------< 3 \n");
        printf("==============================================================================================\n");
        printf("=> To view customer details enter: ----------< 4 \n");
        printf("==============================================================================================\n");
        printf("=> To view a list of all customers enter: ---< 5 \n");
        printf("==============================================================================================\n");
        printf("=> To exit enter: ---------------------------< 6 \n");
        printf("==============================================================================================\n\n");
        printf("==============================================================================================\n");
        printf("=> Enter number here: -----------------------< ");
        scanf("%d", &choice);
        printf("==============================================================================================\n\n");
        getchar();
        switch (choice) {
            case 1:
                addCustomer(customers, & CustomerNo);
                break;
            case 2:
                {
                    int ID;
                    printf("==============================================================================================\n");
                    printf("=> Enter the customers ID number: -----------< "); 
                    scanf("%d", &ID);
                    getchar();
                    for (int i = 0; i < CustomerNo; i++) {
                        if (customers[i].ID == ID) {
                            rentmovie(&customers[i]);
                            break;
                        }
                    }
                    break;
                }
            case 3:
                {
                   int ID;
                    printf("==============================================================================================\n");
                    printf("=> Enter the customers ID number: -----------< "); 
                    scanf("%d", &ID);
                    getchar();
                    for (int i = 0; i < CustomerNo; i++) {
                        if (customers[i].ID == ID) {
                            returnmovie(&customers[i]);
                            break;
                        }
                    }
                    break;
                }
            case 4:
                {
                   int ID;
                    printf("==============================================================================================\n");
                    printf("=> Enter the customers ID number: -----------< "); 
                    scanf("%d", &ID);
                    getchar();
                    for (int i = 0; i < CustomerNo; i++) {
                        if (customers[i].ID == ID) {
                            DisplayCustomerInfo(&customers[i]);
                            break;
                        }
                    }
                    break;
                }
            case 5:
                listCustomers(customers, CustomerNo);
                break;
            case 6:
            	printf("==============================================================================================\n");
                printf("Exiting program...\n");
                printf("==============================================================================================\n\n\n");
                break;
            default:
            	printf("==============================================================================================\n");
                printf("Invalid choice. Please try again.\n");
                printf("==============================================================================================\n\n\n");
        }
    } while (choice != 6);

    return 0;
}
void addCustomer(Customer* customers, int* CustomerNo) {
    Customer newCustomer;
    printf("==============================================================================================\n");
    printf("=> Create the customers ID number: ----------< ");
    scanf("%d", &newCustomer.ID);
    printf("==============================================================================================\n");
    getchar();
    printf("=> What is their full name?: ----------------< ");
    fgets(newCustomer.name, sizeof(newCustomer.name), stdin);
    newCustomer.name[strcspn(newCustomer.name, "\n")] = 0;
    printf("==============================================================================================\n");
    printf("=> What is their mobile phone number?: ------< ");
    fgets(newCustomer.phone, sizeof(newCustomer.phone), stdin);
    newCustomer.phone[strcspn(newCustomer.phone, "\n")] = 0;
	printf("==============================================================================================\n");
    printf("=> What is their address?: ------------------< ");
    fgets(newCustomer.address, sizeof(newCustomer.address), stdin);
    newCustomer.address[strcspn(newCustomer.address, "\n")] = 0;
    newCustomer.pendingCharges = 0.0;
    newCustomer.rentNo = 0;

    customers[*CustomerNo] = newCustomer;
    (*CustomerNo)++;
    printf("==============================================================================================\n");
    printf("=> All complete, the customer has been added to the Database!\n");
    printf("==============================================================================================\n\n\n");
}
// 2
void rentmovie(Customer* customer) {
    if (customer->rentNo >= MAX_RENTALS) {
    	printf("==============================================================================================\n");
        printf("=> Customer has reached maximum amount of rentals permitted,\n");
		printf("=> They must return any loaned out movies before being allowed to rent more.\n");
        printf("==============================================================================================\n\n\n");
        return;
    }
	
    Rent newRental;
    printf("==============================================================================================\n");
    printf("=> What is the name of the movie?: ----------< ");
    fgets(newRental.Movie, sizeof(newRental.Movie), stdin);
    newRental.Movie[strcspn(newRental.Movie, "\n")] = 0;
    printf("==============================================================================================\n");
    printf("=> How many days will they rent the movie?:--< ");
    scanf("%d", &newRental.rentTime);
    getchar();

    customer->rentHistory[customer->rentNo] = newRental;
    customer->rentNo++;

    float rentalCharge = newRental.rentTime * 1;  // Im makinng the price a £1 per day to rnt idk how rental prices work so just change if needed
    customer->pendingCharges += rentalCharge;
	printf("==============================================================================================\n");
    printf("=> Movie has been rented successfully and added to their account!\n");
	printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
    printf("==============================================================================================\n\n\n");
}

// 3
void returnmovie(Customer* customer) {
    if (customer->rentNo == 0) {
    printf("==============================================================================================\n");
    printf("=> This customer currently is not renting any movies.\n");
    printf("==============================================================================================\n\n\n");
        return;
		
    }
	//prin("=> To exit enter: ---------------------------< 6 \n");
	printf("==============================================================================================\n");
    printf("=> Customer is currently renting the following movies:\n");
    for (int i = 0; i < customer->rentNo; i++) {
        printf("=> -------< %d. %s (Rented movie for: %d days)\n", i + 1, customer->rentHistory[i].Movie, customer->rentHistory[i].rentTime);
    }

    int rentalIndex;
    printf("==============================================================================================\n");
    printf("=> What movie are they returning?\n");
    printf("=> Enter the moivie number here: ------------<");
    scanf("%d", &rentalIndex);
    rentalIndex--;

    if (rentalIndex < 0 || rentalIndex >= customer->rentNo) {
    	printf("==============================================================================================\n");
        printf("=> Invalid selection.\n");
        printf("==============================================================================================\n\n\n");
        return;
    }
    float rentalCharge = customer->rentHistory[rentalIndex].rentTime * 1;
    customer->pendingCharges -= rentalCharge;

    // Shift the rental history to remove the returned film
    for (int i = rentalIndex; i < customer->rentNo - 1; i++) {
        customer->rentHistory[i] = customer->rentHistory[i + 1];
    }
    customer->rentNo--;
	printf("==============================================================================================\n");
    printf("=> Movie has been returned successfully and removed from their account!\n");
	printf("=> Pending charges: $%.2f\n", customer->pendingCharges);
    printf("==============================================================================================\n\n\n");
}

// 4
void DisplayCustomerInfo(Customer* customer) {

	printf("==============================================================================================\n");
    printf("=> Customer information: \n");
    printf("==============================================================================================\n");
    printf("=> ID Number: -------------------------------< %d\n", customer->ID);
    printf("==============================================================================================\n");
    printf("=> Name: ------------------------------------< %s\n", customer->name);
    printf("==============================================================================================\n");
    printf("=> Mobile phone number: ---------------------< %s\n", customer->phone);
    printf("==============================================================================================\n");
    printf("=> Address: ---------------------------------< %s\n", customer->address);
    printf("==============================================================================================\n");
    printf("=> Pending Charges: -------------------------< $%.2f\n", customer->pendingCharges);
    printf("==============================================================================================\n");
    if (customer->rentNo > 0) {
    printf("==============================================================================================\n");
    printf("=> Customer Rental History:\n");
    for (int i = 0; i < customer->rentNo; i++) {
    printf("==============================================================================================\n");
    printf("=> -----------------------< %d. %s (Rented movie(s) for: %d days)\n", i + 1, customer->rentHistory[i].Movie, customer->rentHistory[i].rentTime);
        }
    } else {printf("==============================================================================================\n");
        printf("=> Customer has not rented any movies yet.\n");
        printf("==============================================================================================\n\n\n");
    }
    printf("==============================================================================================\n\n\n");
}

// 5
void listCustomers(Customer* customers, int CustomerNo) {
    if (CustomerNo == 0) {
    	printf("==============================================================================================\n");
        printf("=> No customers recorded on the system.\n");
        printf("==============================================================================================\n\n\n");
        return;
    }
	printf("==============================================================================================\n");
    printf("=> List of all customers recorded on the system:\n");
    printf("==============================================================================================\n");
    for (int i = 0; i < CustomerNo; i++) {
        printf("=> ID Number: %d, Name: %s\n", customers[i].ID, customers[i].name);
    }
    printf("==============================================================================================\n\n\n");
}
