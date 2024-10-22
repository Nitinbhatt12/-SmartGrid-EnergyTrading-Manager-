#include<stdio.h>
#include<string.h>
#define TXNID_len 15
#define BuyerID_len 15
#define SellerID_len 15
#define DB_size 700

// Struct for time (date, month, year, hour, minute, second)
struct time {
    unsigned int date;
    char month[10];
    unsigned int year;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
};



struct transaction_record{
    char TXN_ID[TXNID_len];
    char BUYER_ID[BuyerID_len];
    char SELLER_ID[SellerID_len];
    float Amt_Energy;
    float Prc_kwh;
    struct time time_TXN;  
};


// Struct to store seller revenue
struct seller_revenue {
    char SELLER_ID[SellerID_len];
    float total_revenue;
};


//struct to store energy bought by buyer
struct buyer_energy{
    char BUYER_ID[BuyerID_len];
    float total_energy;
};


// Structure to store the seller/buyer pair and their transaction count , revenue generated
struct seller_buyer_pair {
    char SELLER_ID[SellerID_len];
    char BUYER_ID[BuyerID_len];
    int transaction_count;
    float rev_generated;
};


//intialize database
void Intialize_DB(struct transaction_record transaction_DB[],int size){
    for(int i=0;i<size;i++){
        transaction_DB[i].TXN_ID[0]='\0';
        transaction_DB[i].BUYER_ID[0]='\0';
        transaction_DB[i].SELLER_ID[0]='\0';
        transaction_DB[i].Amt_Energy=0;
        transaction_DB[i].Prc_kwh=0;
        transaction_DB[i].time_TXN.date=0;
        transaction_DB[i].time_TXN.month[0]='\0';
        transaction_DB[i].time_TXN.year=0;
    }
}


//intialize seller revenue database
void Intialize_rev_DB(struct seller_revenue seller_revenue_DB[],int size){
    for(int i=0;i<size;i++){
        seller_revenue_DB[i].SELLER_ID[0]='\0';
        seller_revenue_DB[i].total_revenue = 0;
    }
}

//intialize buyer energy database
void Intialize_egy_DB(struct buyer_energy buyer_energy_DB[],int size){
    for(int i=0;i<size;i++){
        buyer_energy_DB[i].BUYER_ID[0]='\0';
        buyer_energy_DB[i].total_energy=0;
    }
}

//Intialize seller buyer pair
void Intialize_seller_buyer_pair(struct seller_buyer_pair pair_DB[],int size){
    for(int i=0;i<size;i++){
        pair_DB[i].BUYER_ID[0]='\0';
        pair_DB[i].rev_generated=0;
        pair_DB[i].SELLER_ID[0]='\0';
        pair_DB[i].transaction_count=0;
    }
}


//adding new transactions
int Insert(struct transaction_record transaction_DB[],char txn_ID[],char buyer_ID[],char seller_ID[],float amt,float pricekw,struct time time_ok,int pos){
    int freeloc=pos;
    if(freeloc<DB_size){
    strcpy(transaction_DB[freeloc].TXN_ID,txn_ID);
    strcpy(transaction_DB[freeloc].BUYER_ID,buyer_ID);
    strcpy(transaction_DB[freeloc].SELLER_ID,seller_ID);
    transaction_DB[freeloc].Amt_Energy=amt;
    transaction_DB[freeloc].Prc_kwh=pricekw;
    transaction_DB[freeloc].time_TXN.date=time_ok.date;
    strcpy(transaction_DB[freeloc].time_TXN.month,time_ok.month);
    transaction_DB[freeloc].time_TXN.year=time_ok.year;
    //energy_DB[freeloc]=transaction_DB[freeloc];
    freeloc=pos+1;
    }
    else{
        printf("No Space\n");
    }
    return freeloc;
}


//printing all transactions
void print_TXN(struct transaction_record transaction_DB[], int size) {
    printf("\n========= Transaction Records =========\n");
    for(int i = 0; i < size; i++) {
        printf("Transaction #%d\n", i + 1);
        printf("------------------------------\n");
        printf("Transaction ID   : %s\n", transaction_DB[i].TXN_ID);
        printf("Buyer ID         : %s\n", transaction_DB[i].BUYER_ID);
        printf("Seller ID        : %s\n", transaction_DB[i].SELLER_ID);
        printf("Amount of Energy : %.2f kWh\n", transaction_DB[i].Amt_Energy);
        printf("Price per kWh    : $%.2f\n", transaction_DB[i].Prc_kwh);
        printf("Transaction Date : %u %s %u\n", transaction_DB[i].time_TXN.date, 
                                              transaction_DB[i].time_TXN.month, 
                                              transaction_DB[i].time_TXN.year);
        printf("Transaction Time : %02u:%02u:%02u\n", transaction_DB[i].time_TXN.hour, 
                                                  transaction_DB[i].time_TXN.minute, 
                                                  transaction_DB[i].time_TXN.second);
        printf("------------------------------\n\n");
    }
    printf("Operation Success.\n");
}


// Function to create a list of transactions according to the seller and optionally print transactions per seller
int list_by_seller(struct transaction_record transaction_DB[], 
                   struct transaction_record list_seller_DB[], 
                   struct seller_revenue seller_revenue_DB[], 
                   int size, int choice) {

    char unique_sellers[DB_size][SellerID_len];  // Array to store unique seller IDs
    int unique_seller_count = 0;  // Counter for unique sellers

    // Collect unique seller IDs
    for (int i = 0; i < size; i++) {
        int is_unique = 1;
        // Check if the seller ID is already in the unique_sellers array
        for (int j = 0; j < unique_seller_count && is_unique; j++) {
            if (strcmp(transaction_DB[i].SELLER_ID, unique_sellers[j]) == 0) {
                is_unique = 0;  // Mark as non-unique if already found
            }
        }
        // If unique, add the seller ID to the unique_sellers array
        if (is_unique) {
            strcpy(unique_sellers[unique_seller_count], transaction_DB[i].SELLER_ID);
            strcpy(seller_revenue_DB[unique_seller_count].SELLER_ID, transaction_DB[i].SELLER_ID);  // Add to seller revenue DB
            unique_seller_count++;
        }
    }

    int k = 0;  // Counter to track the index for list_seller_DB

    // If choice == 3, print transactions per seller
    if (choice == 3) {
        for (int i = 0; i < unique_seller_count; i++) {
            printf("\n========== Transactions for Seller ID: %s ==========\n", unique_sellers[i]);

            for (int j = 0; j < size; j++) {
                // If transaction belongs to current seller, print the transaction details
                if (strcmp(transaction_DB[j].SELLER_ID, unique_sellers[i]) == 0) {
                    printf("Transaction ID: %s, Buyer ID: %s\n", transaction_DB[j].TXN_ID, transaction_DB[j].BUYER_ID);
                    printf("Amount of Energy: %.2f kWh, Price per kWh: $%.2f\n", transaction_DB[j].Amt_Energy, transaction_DB[j].Prc_kwh);
                    printf("Date: %u %s %u, Time: %02u:%02u:%02u\n", 
                           transaction_DB[j].time_TXN.date, 
                           transaction_DB[j].time_TXN.month, 
                           transaction_DB[j].time_TXN.year,
                           transaction_DB[j].time_TXN.hour, 
                           transaction_DB[j].time_TXN.minute, 
                           transaction_DB[j].time_TXN.second);
                    printf("---------------------------------------------\n");

                    // Add the transaction to the seller list
                    list_seller_DB[k] = transaction_DB[j];
                    k++;
                }
            }
        }
        printf("Operation Success.\n");
    }
    // If choice is not 3, just create the seller's transaction list
    else {
        for (int i = 0; i < unique_seller_count; i++) {
            for (int j = 0; j < size; j++) {
                if (strcmp(transaction_DB[j].SELLER_ID, unique_sellers[i]) == 0) {
                    list_seller_DB[k] = transaction_DB[j];
                    k++;
                }
            }
        }
    }

    return unique_seller_count;  // Return the count of unique sellers
}




// Function to create a list of transactions according to the buyer and optionally print transactions per buyer
int list_by_buyer(struct transaction_record transaction_DB[], 
                  struct transaction_record list_buyer_DB[], 
                  struct buyer_energy buyer_energy_DB[], 
                  int size, int choice) {
    
    char unique_buyers[DB_size][BuyerID_len];  // Array to store unique buyer IDs
    int unique_buyer_count = 0;  // Counter for unique buyers

    // Collect unique buyer IDs
    for (int i = 0; i < size; i++) {
        int is_unique = 1;

        // Check if the buyer ID is already in the unique_buyers array
        for (int j = 0; j < unique_buyer_count && is_unique; j++) {
            if (strcmp(transaction_DB[i].BUYER_ID, unique_buyers[j]) == 0) {
                is_unique = 0;  // Mark as non-unique if already found
            }
        }

        // If unique, add the buyer ID to the unique_buyers array
        if (is_unique) {
            strcpy(unique_buyers[unique_buyer_count], transaction_DB[i].BUYER_ID);
            strcpy(buyer_energy_DB[unique_buyer_count].BUYER_ID, transaction_DB[i].BUYER_ID);  // Add to buyer energy DB
            unique_buyer_count++;
        }
    }

    int k = 0;  // Counter to track the index for list_buyer_DB

    // If choice == 4, print transactions per buyer
    if (choice == 4) {
        for (int i = 0; i < unique_buyer_count; i++) {
            printf("\n========== Transactions for Buyer ID: %s ==========\n", unique_buyers[i]);

            for (int j = 0; j < size; j++) {
                // If transaction belongs to current buyer, print the transaction details
                if (strcmp(transaction_DB[j].BUYER_ID, unique_buyers[i]) == 0) {
                    printf("Transaction ID   : %s\n", transaction_DB[j].TXN_ID);
                    printf("Amount of Energy : %.2f kWh, Price per kWh : $%.2f\n", transaction_DB[j].Amt_Energy, transaction_DB[j].Prc_kwh);
                    printf("Date             : %u %s %u\n", transaction_DB[j].time_TXN.date, 
                                                         transaction_DB[j].time_TXN.month, 
                                                         transaction_DB[j].time_TXN.year);
                    printf("Time             : %02u:%02u:%02u\n", transaction_DB[j].time_TXN.hour, 
                                                               transaction_DB[j].time_TXN.minute, 
                                                               transaction_DB[j].time_TXN.second);
                    printf("---------------------------------------------\n");

                    // Add the transaction to the buyer list
                    list_buyer_DB[k] = transaction_DB[j];
                    k++;
                }
            }
        }
        printf("Operation Success.\n");
    }
    // If choice is not 4, just create the buyer's transaction list
    else {
        for (int i = 0; i < unique_buyer_count; i++) {
            for (int j = 0; j < size; j++) {
                if (strcmp(transaction_DB[j].BUYER_ID, unique_buyers[i]) == 0) {
                    list_buyer_DB[k] = transaction_DB[j];
                    k++;
                }
            }
        }
    }

    return unique_buyer_count;  // Return the count of unique buyers
}



// Function to compare two dates
// Returns 1 if time1 is after time2, -1 if time1 is before time2, and 0 if they are equal
int compare_dates(struct time time1, struct time time2) {
    if (time1.year > time2.year) return 1;
    if (time1.year < time2.year) return -1;

    // Convert month strings to integer
    char months[12][10] = {"January", "February", "March", "April", "May", "June", 
                           "July", "August", "September", "October", "November", "December"};
    int month1 = -1, month2 = -1;
    for (int i = 0; i < 12; i++) {
        if (strcmp(time1.month, months[i]) == 0) month1 = i + 1;
        if (strcmp(time2.month, months[i]) == 0) month2 = i + 1;
    }

    if (month1 > month2) return 1;
    if (month1 < month2) return -1;

    // Compare dates
    if (time1.date > time2.date) return 1;
    if (time1.date < time2.date) return -1;

    // If dates are the same, compare times (hour, minute, second)
    if (time1.hour > time2.hour) return 1;
    if (time1.hour < time2.hour) return -1;
    if (time1.minute > time2.minute) return 1;
    if (time1.minute < time2.minute) return -1;
    if (time1.second > time2.second) return 1;
    if (time1.second < time2.second) return -1;

    return 0; // If all are the same, dates are equal
}

//transactions that takes place in between the dates
// Function to list transactions within a given time period
void list_transactions_by_period(struct transaction_record transaction_DB[], int size, 
                                 struct time start_date, struct time end_date) {

    printf("Listing transactions from %u %s %u to %u %s %u:\n", 
           start_date.date, start_date.month, start_date.year, 
           end_date.date, end_date.month, end_date.year);

    int transactions_found = 0;  // Flag to track if any transactions are found

    for (int i = 0; i < size; i++) {
        // Check if the transaction date is within the given period
        if (compare_dates(transaction_DB[i].time_TXN, start_date) >= 0 && 
            compare_dates(transaction_DB[i].time_TXN, end_date) <= 0) {

            // Print transaction details
            printf("Transaction ID   : %s\n", transaction_DB[i].TXN_ID);
            printf("Buyer ID         : %s\n", transaction_DB[i].BUYER_ID);
            printf("Seller ID        : %s\n", transaction_DB[i].SELLER_ID);
            printf("Amount of Energy : %.2f kWh\n", transaction_DB[i].Amt_Energy);
            printf("Price per kWh    : $%.2f\n", transaction_DB[i].Prc_kwh);
            printf("Date             : %u %s %u\n", transaction_DB[i].time_TXN.date, 
                                                       transaction_DB[i].time_TXN.month, 
                                                       transaction_DB[i].time_TXN.year);
            printf("Time             : %02u:%02u:%02u\n", transaction_DB[i].time_TXN.hour, 
                                                          transaction_DB[i].time_TXN.minute, 
                                                          transaction_DB[i].time_TXN.second);
            printf("---------------------------------------------\n");

            transactions_found = 1;  // Mark that a transaction was found
        }
    }

    // If no transactions are found within the given period
    if (!transactions_found) {
        printf("No transactions found in the specified period.\n");
    }

    printf("Operation Success!\n");
}


//month with most transactions
 int month_most_transaction(struct transaction_record transaction_DB[],int size){
    char months[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    int cnt_trans_Month = 0;
    //helps in remembering the index of month
    int idx=-1;
    int cnt = 0;
    for(int i=0;i<12;i++){
        cnt=0;
        for(int j=0;j<size;j++){
            if(strcmp(months[i],transaction_DB[j].time_TXN.month)==0){
                cnt++;
            }
        }
        if(cnt>cnt_trans_Month){
            cnt_trans_Month=cnt;
            idx=i;
        }
    }
    return idx;
}

//revenue per seller

float revenue_seller(struct transaction_record list_seller_DB[],int size,char seller_ID[]){
    float revenue = 0;
    int found=0;
    for(int i=0;i<size && !found;i++){
        if(strcmp(list_seller_DB[i].SELLER_ID,seller_ID)==0){
            found=1;
            while(strcmp(list_seller_DB[i].SELLER_ID,seller_ID)==0){
                revenue = revenue + list_seller_DB[i].Amt_Energy * list_seller_DB[i].Prc_kwh;
                i++;
            }
        }
    }
    if(!found){
        printf("No record found !");
    }
    return revenue;
}


//sorting based on their revenue
void i_revenue_sort(struct seller_revenue seller_revenue_DB[],int size){
    char temp_seller_ID[SellerID_len];
    float min_revenue = 0;
    int idx=-1;
    strcpy(temp_seller_ID,seller_revenue_DB[0].SELLER_ID);
    min_revenue = seller_revenue_DB[0].total_revenue;
    for(int i=1;i<size;i++){
        if(min_revenue > seller_revenue_DB[i].total_revenue){
            min_revenue = seller_revenue_DB[i].total_revenue;
            idx=i;
        }
    }
    strcpy(seller_revenue_DB[0].SELLER_ID,seller_revenue_DB[idx].SELLER_ID);
    seller_revenue_DB[idx].total_revenue=seller_revenue_DB[0].total_revenue;
    strcpy(seller_revenue_DB[idx].SELLER_ID,temp_seller_ID);
    seller_revenue_DB[0].total_revenue=min_revenue;

    for(int i=2;i<size;i++){
        strcpy(temp_seller_ID,seller_revenue_DB[i].SELLER_ID);
        float x = seller_revenue_DB[i].total_revenue;
        int j=i-1;
        while(seller_revenue_DB[j].total_revenue > x){
            seller_revenue_DB[j+1].total_revenue=seller_revenue_DB[j].total_revenue;
            strcpy(seller_revenue_DB[j+1].SELLER_ID,seller_revenue_DB[j].SELLER_ID);
            j--;
        }
        seller_revenue_DB[j+1].total_revenue=x;
        strcpy(seller_revenue_DB[j+1].SELLER_ID,temp_seller_ID);
    }
    for(int i=0;i<size;i++){
        printf("\n Seller ID: %s with revenue : %.2f\n",seller_revenue_DB[i].SELLER_ID,seller_revenue_DB[i].total_revenue);
    }
    printf("\n Operation Success.\n");
}


//sorting by energy && largest energy trading event
void sort_by_energy(struct transaction_record energy_DB[],int size){
    int flag=1;
    for(int i=0;i<size-1 && flag;i++){
        flag=0;
        for(int j=0;j<size-i-1;j++){
            if(energy_DB[j].Amt_Energy > energy_DB[j+1].Amt_Energy){
                struct transaction_record temp = energy_DB[j];
                energy_DB[j]=energy_DB[j+1];
                energy_DB[j+1]=temp;
                flag=1;
            }
        }
    }
    for(int i=0;i<size;i++){
        printf("\n %s:Energy: %.2f with price per Kw %.2f in between %s and %s on %u:%s:%u ,time %u::%u::%u \n",energy_DB[i].TXN_ID,energy_DB[i].Amt_Energy,
                                        energy_DB[i].Prc_kwh,energy_DB[i].SELLER_ID,energy_DB[i].BUYER_ID,energy_DB[i].time_TXN.date,
                                        energy_DB[i].time_TXN.month,energy_DB[i].time_TXN.year,energy_DB[i].time_TXN.hour,energy_DB[i].time_TXN.minute,
                                        energy_DB[i].time_TXN.second);
    }
    printf("\n Operation Success.\n");
}

//energy per buyer
float energy_bought_buyer(struct transaction_record list_buyer_DB[],int size,char buyer_ID[]){
    float energy = 0;
    int found=0;
    for(int i=0;i<size && !found;i++){
        if(strcmp(list_buyer_DB[i].BUYER_ID,buyer_ID)==0){
            found=1;
            while(strcmp(list_buyer_DB[i].BUYER_ID,buyer_ID)==0){
                energy = energy + list_buyer_DB[i].Amt_Energy ;
                i++;
            }
        }
    }
    if(!found){
        printf("No record found !");
    }
    return energy;
}


// Sorting buyers based on total energy purchased
void i_buyertotalenergy_sort(struct buyer_energy buyer_energy_DB[], int size) {
    char temp_buyer_ID[BuyerID_len];
    
    // Insertion sort for sorting based on total energy
    for (int i = 1; i < size; i++) {
        strcpy(temp_buyer_ID, buyer_energy_DB[i].BUYER_ID); // Save buyer ID
        float x = buyer_energy_DB[i].total_energy; // Save total energy for current buyer
        int j = i - 1;
        
        // Shift elements of array that are greater than x
        while (j >= 0 && buyer_energy_DB[j].total_energy > x) {
            buyer_energy_DB[j + 1].total_energy = buyer_energy_DB[j].total_energy;
            strcpy(buyer_energy_DB[j + 1].BUYER_ID, buyer_energy_DB[j].BUYER_ID);
            j--;
        }
        
        // Place the saved buyer at the correct position
        buyer_energy_DB[j + 1].total_energy = x;
        strcpy(buyer_energy_DB[j + 1].BUYER_ID, temp_buyer_ID);
    }
    
    // Display sorted results
    for (int i = 0; i < size; i++) {
        printf("\n Buyer ID: %s bought total energy equal to %.2f \n", 
               buyer_energy_DB[i].BUYER_ID, buyer_energy_DB[i].total_energy);
    }
    printf("\n Operation Success.\n");
}



// Function to find the seller/buyer pair with the maximum number of transactions
void find_max_transactions_pair_sort(struct transaction_record transaction_DB[],struct seller_buyer_pair pair_DB[], int size,int n) {
    
    int pair_count = 0;
    int max_transactions , max_index ;

    // Iterate through all transactions and count the occurrences of each seller/buyer pair
    for (int i = 0; i < size; i++) {
        int found = 0;
        // Check if the (seller, buyer) pair already exists
        for (int j = 0; j < pair_count && !found; j++) {
            if (strcmp(pair_DB[j].SELLER_ID, transaction_DB[i].SELLER_ID) == 0 &&
                strcmp(pair_DB[j].BUYER_ID, transaction_DB[i].BUYER_ID) == 0) {
                pair_DB[j].transaction_count++;
                pair_DB[j].rev_generated= pair_DB[j].rev_generated + transaction_DB[i].Amt_Energy * transaction_DB[i].Prc_kwh;
                found = 1;
            }
        }
        // If the pair doesn't exist, add a new entry
        if (!found) {
            strcpy(pair_DB[pair_count].SELLER_ID, transaction_DB[i].SELLER_ID);
            strcpy(pair_DB[pair_count].BUYER_ID, transaction_DB[i].BUYER_ID);
            pair_DB[pair_count].transaction_count = 1;
            pair_DB[pair_count].rev_generated= pair_DB[pair_count].rev_generated + transaction_DB[i].Amt_Energy * transaction_DB[i].Prc_kwh;
            pair_count++;
        }
    }
    switch(n){
        case 1:

    // Find the pair with the maximum transaction count
         max_transactions = 0;
         max_index = -1;
        for (int i = 0; i < pair_count; i++) {
               if (pair_DB[i].transaction_count > max_transactions) {
              max_transactions = pair_DB[i].transaction_count;
              max_index = i;
              }
         }

    // Print the seller/buyer pair involved in the maximum number of transactions
        if (max_index != -1) {
             printf("\n Seller/Buyer pair with maximum transactions:\n");
             printf("\n Seller ID: %s, Buyer ID: %s, Transactions: %d\n", 
                pair_DB[max_index].SELLER_ID, 
                pair_DB[max_index].BUYER_ID, 
                pair_DB[max_index].transaction_count);
            } else {
            printf("\n No transactions found.\n");
        }
        printf("\n Operation Success.\n");
        break;

        case 2:
            for (int i = 0; i < pair_count - 1; i++) {
              for (int j = 0; j < pair_count - i - 1; j++) {
                if (pair_DB[j].transaction_count < pair_DB[j + 1].transaction_count) {
                   // Swap the pairs 
                   struct seller_buyer_pair temp = pair_DB[j];
                   pair_DB[j] = pair_DB[j + 1];
                   pair_DB[j + 1] = temp;
                 }
              }
            }

    // Print the sorted pairs
            printf("\n Seller/Buyer pairs sorted by number of transactions :\n");
            for (int i = 0; i < pair_count; i++) {
                 printf("Seller: %s, Buyer: %s, Transactions: %d\n", 
                 pair_DB[i].SELLER_ID, 
                 pair_DB[i].BUYER_ID, 
                 pair_DB[i].transaction_count);
            }
            printf("\n Operation Success.\n");
        break;

        case 3://  the seller/buyer pairs by transaction count (in decreasing order)
            for (int i = 0; i < pair_count - 1; i++) {
                 for (int j = 0; j < pair_count - i - 1; j++) {
                     if (pair_DB[j].rev_generated < pair_DB[j + 1].rev_generated) {
                     // Swap the pairs 
                    struct seller_buyer_pair temp = pair_DB[j];
                    pair_DB[j] = pair_DB[j + 1];
                    pair_DB[j + 1] = temp;
                    }
                }
            }

            // Print the sorted pairs
            printf("\n Seller/Buyer pairs sorted by revenue generated:\n");
            for (int i = 0; i < pair_count; i++) {
                printf("\n Seller: %s, Buyer: %s, revenue: %.2f \n", 
                pair_DB[i].SELLER_ID, 
                pair_DB[i].BUYER_ID, 
                pair_DB[i].rev_generated);
            }
            printf("\n Operation Success.\n");
    
        break;
    }
}


int main(){
    int i,flag=1,unique_seller_count,unique_buyer_count,index;
    char txn_ID[TXNID_len];
    char buyer_ID[BuyerID_len];
    char seller_ID[SellerID_len];
    float amt_egy;
    float pce_kwh;
    float revenue;
    float energy;
    struct time time_ok;
    int newpos=0;
    struct transaction_record transaction_DB[DB_size];
    struct transaction_record list_seller_DB[DB_size];
    struct transaction_record list_buyer_DB[DB_size];
    struct transaction_record energy_DB[DB_size];
    struct time start_date;
    struct time end_date;
    struct seller_revenue seller_revenue_DB[DB_size];
    struct buyer_energy buyer_energy_DB[DB_size];
    struct seller_buyer_pair pair_DB[DB_size];
    char month[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    Intialize_seller_buyer_pair(pair_DB,DB_size);
    Intialize_rev_DB(seller_revenue_DB,DB_size);
    Intialize_egy_DB(buyer_energy_DB,DB_size);
    Intialize_DB(transaction_DB,DB_size);
    /*
    printf("How many records");
    scanf("%d",&num_rec);
    */
    struct transaction_record hardcoded_transactions[] = {
    {"TXN001", "Buy001", "Sel001", 100, 0.20, {1, "January", 2024, 9, 30, 15}},
    {"TXN002", "Buy001", "Sel002", 150, 0.22, {2, "February", 2024, 14, 45, 0}},
    {"TXN003", "Buy003", "Sel003", 200, 0.25, {3, "March", 2024, 18, 15, 45}},
    {"TXN004", "Buy004", "Sel004", 250, 0.30, {4, "April", 2024, 12, 10, 10}},
    {"TXN005", "Buy004", "Sel005", 300, 0.28, {5, "May", 2024, 16, 55, 20}},
    {"TXN006", "Buy001", "Sel006", 350, 0.35, {6, "June", 2024, 19, 5, 5}},
    {"TXN007", "Buy001", "Sel006", 400, 0.32, {7, "July", 2024, 10, 30, 30}},
    {"TXN008", "Buy001", "Sel009", 87, 0.25, {14, "January", 2024, 10, 30, 0}},
    {"TXN009", "Buy008", "Sel010", 500, 0.40, {8, "August", 2024, 11, 50, 50}},
    {"TXN010", "Buy009", "Sel011", 550, 0.45, {9, "September", 2024, 12, 0, 0}},
    {"TXN011", "Buy010", "Sel012", 600, 0.50, {10, "October", 2024, 13, 15, 15}},
    {"TXN012", "Buy011", "Sel013", 650, 0.55, {11, "November", 2024, 14, 25, 25}},
    {"TXN013", "Buy012", "Sel014", 700, 0.60, {12, "December", 2024, 15, 35, 35}},
    {"TXN014", "Buy013", "Sel001", 750, 0.65, {13, "January", 2024, 16, 45, 45}},
    {"TXN015", "Buy014", "Sel001", 800, 0.70, {14, "February", 2024, 17, 55, 55}},
    {"TXN016", "Buy015", "Sel017", 850, 0.75, {15, "March", 2024, 18, 5, 5}},
    {"TXN017", "Buy006", "Sel018", 900, 0.80, {16, "April", 2024, 19, 15, 15}},
    {"TXN018", "Buy003", "Sel003", 950, 0.85, {17, "May", 2024, 20, 25, 25}},
    {"TXN019", "Buy008", "Sel009", 1000, 0.90, {18, "June", 2024, 21, 35, 35}},
    {"TXN020", "Buy008", "Sel008", 1050, 0.95, {19, "July", 2024, 22, 45, 45}},
    {"TXN021", "Buy020", "Sel008", 1100, 1.00, {20, "August", 2024, 23, 55, 55}},
    {"TXN022", "Buy021", "Sel023", 1150, 1.05, {21, "September", 2024, 0, 5, 5}},
    {"TXN023", "Buy008", "Sel009", 1200, 1.10, {22, "October", 2024, 1, 15, 15}},
    {"TXN024", "Buy008", "Sel008", 1250, 1.15, {23, "November", 2024, 2, 25, 25}},
    {"TXN025", "Buy002", "Sel026", 1300, 1.20, {24, "December", 2024, 3, 35, 35}},
    {"TXN026", "Buy025", "Sel027", 1350, 1.25, {25, "January", 2024, 4, 45, 45}},
    {"TXN027", "Buy026", "Sel028", 1400, 1.30, {26, "February", 2024, 5, 55, 55}},
    {"TXN028", "Buy005", "Sel029", 1450, 1.35, {27, "March", 2024, 6, 5, 5}},
    {"TXN029", "Buy028", "Sel030", 1500, 1.40, {28, "April", 2024, 7, 15, 15}},
    {"TXN030", "Buy028", "Sel030", 1550, 1.45, {29, "May", 2024, 8, 25, 25}}
    };

    // Initialize transaction_DB with hardcoded transactions
    for (i = 0; i < 30; i++) {
        transaction_DB[i] = hardcoded_transactions[i];
        //energy_DB[i]=hardcoded_transactions[i];
    }
    newpos = 30;  // Update new position after hardcoded transactions
while (flag && newpos < DB_size) {
    // Display menu options
    printf("\nEnter your choice:\n");
    printf(" 1. Add new transaction\n");
    printf(" 2. Display all transactions\n");
    printf(" 3. List transactions by seller\n");
    printf(" 4. List transactions by buyer\n");
    printf(" 5. List transactions within a specific time period\n");
    printf(" 6. Find the month with the most transactions\n");
    printf(" 7. Query total revenue generated by a specific seller\n");
    printf(" 8. Sort sellers by revenue\n");
    printf(" 9. Display the largest energy trading event\n");
    printf("10. Sort buyers by total energy bought\n");
    printf("11. Find the seller/buyer pair with the most transactions\n");
    printf("12. Sort seller/buyer pairs by transaction count\n");
    printf("13. Sort seller/buyer pairs by total revenue exchanged\n");
    printf(" 0. Quit\n\n");
    
    // Get user input for the choice
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            // Add new transaction
            printf("\nEnter the following details:\n");
            printf("TXN_ID, BUYER_ID, SELLER_ID, AMOUNT_ENERGY, PRICE_KWH, DATE (dd mm yyyy), TIME (hr min sec)\n");
            scanf("%s %s %s %f %f %u %s %u %u %u %u", txn_ID, buyer_ID, seller_ID, &amt_egy, &pce_kwh, 
                  &time_ok.date, time_ok.month, &time_ok.year, &time_ok.hour, &time_ok.minute, &time_ok.second);
            
            newpos = Insert(transaction_DB, txn_ID, buyer_ID, seller_ID, amt_egy, pce_kwh, time_ok, newpos);
            break;
            
        case 2:
            // Display all transactions
            print_TXN(transaction_DB, newpos);
            break;
            
        case 3:
            // List transactions by seller
            Intialize_DB(list_seller_DB, DB_size);
            unique_seller_count = list_by_seller(transaction_DB, list_seller_DB, seller_revenue_DB, newpos, 3);
            break;
            
        case 4:
            // List transactions by buyer
            Intialize_DB(list_buyer_DB, DB_size);
            unique_buyer_count = list_by_buyer(transaction_DB, list_buyer_DB, buyer_energy_DB, newpos, 4);
            break;
            
        case 5:
            // List transactions within a specific time period
            printf("Enter start date (dd mm yyyy) and time (hr min sec): ");
            scanf("%u %s %u %u %u %u", &start_date.date, start_date.month, &start_date.year, 
                  &start_date.hour, &start_date.minute, &start_date.second);
            printf("Enter end date (dd mm yyyy) and time (hr min sec): ");
            scanf("%u %s %u %u %u %u", &end_date.date, end_date.month, &end_date.year, 
                  &end_date.hour, &end_date.minute, &end_date.second);
            
            list_transactions_by_period(transaction_DB, newpos, start_date, end_date);
            break;
            
        case 6:
            // Find the month with the most transactions
            index = month_most_transaction(transaction_DB, newpos);
            printf("Month with the most transactions: %s\n", month[index]);
            break;
            
        case 7:
            // Query total revenue by a specific seller
            printf("Enter seller ID: ");
            scanf("%s", seller_ID);
            unique_seller_count = list_by_seller(transaction_DB, list_seller_DB, seller_revenue_DB, newpos, 7);
            revenue = revenue_seller(list_seller_DB, newpos, seller_ID);
            printf("Total revenue generated by seller ID %s: %.2f\n", seller_ID, revenue);
            break;
            
        case 8:
            // Sort sellers by revenue
            unique_seller_count = list_by_seller(transaction_DB, list_seller_DB, seller_revenue_DB, newpos, 8);
            for (i = 0; i < unique_seller_count; i++) {
                revenue = revenue_seller(list_seller_DB, newpos, seller_revenue_DB[i].SELLER_ID);
                seller_revenue_DB[i].total_revenue = revenue;
            }
            i_revenue_sort(seller_revenue_DB, unique_seller_count);
            break;
            
        case 9:
            // Display the largest energy trading event
            for (i = 0; i < newpos; i++) {
                energy_DB[i] = transaction_DB[i];
            }
            sort_by_energy(energy_DB, newpos);
            int largest_index = newpos - 1;
            printf("Largest energy trading event:\n");
            printf("Transaction ID: %s | Seller ID: %s | Buyer ID: %s | Amount of Energy: %.2f KWH | Price per KWH: %.2f | Date: %u:%s:%u | Time: %u:%u:%u\n",
                   transaction_DB[largest_index].TXN_ID, transaction_DB[largest_index].SELLER_ID, transaction_DB[largest_index].BUYER_ID,
                   transaction_DB[largest_index].Amt_Energy, transaction_DB[largest_index].Prc_kwh, transaction_DB[largest_index].time_TXN.date,
                   transaction_DB[largest_index].time_TXN.month, transaction_DB[largest_index].time_TXN.year,
                   transaction_DB[largest_index].time_TXN.hour, transaction_DB[largest_index].time_TXN.minute, transaction_DB[largest_index].time_TXN.second);
            break;
            
        case 10:
            // Sort buyers by total energy bought
            unique_buyer_count = list_by_buyer(transaction_DB, list_buyer_DB, buyer_energy_DB, newpos, 10);
            for (i = 0; i < unique_buyer_count; i++) {
                energy = energy_bought_buyer(list_buyer_DB, newpos, buyer_energy_DB[i].BUYER_ID);
                buyer_energy_DB[i].total_energy = energy;
            }
            i_buyertotalenergy_sort(buyer_energy_DB, unique_buyer_count);
            break;
            
        case 11:
            // Find the seller/buyer pair with the most transactions
            find_max_transactions_pair_sort(transaction_DB, pair_DB, newpos, 1);
            break;
            
        case 12:
            // Sort seller/buyer pairs by number of transactions
            find_max_transactions_pair_sort(transaction_DB, pair_DB, newpos, 2);
            break;
            
        case 13:
            // Sort seller/buyer pairs by total revenue exchanged
            find_max_transactions_pair_sort(transaction_DB, pair_DB, newpos, 3);
            break;
            
        case 0:
            // Quit the program
            flag = 0;
            break;
            
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
    }
}

    return 0;
}