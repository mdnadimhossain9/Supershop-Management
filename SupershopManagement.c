// Irfanul Karim, [8/19/2024 11:46 PM]
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PRODUCTS 100
#define MAX_CUSTOMERS 50
#define MAX_BILLS 100
#define DATE_STRING_LENGTH 20
#define MAX_REVIEW_LENGTH 255

typedef struct
{
    int product_id;
    char product_name[50];
    float price;
    int quantity;
} Product;
typedef struct
{
    int customer_id;
    char customer_name[50];
    char contact_number[15];
} Customer;
typedef struct
{
    int bill_id;
    Product product_list[MAX_PRODUCTS];
    int product_count;
    float total_amount;
    Customer customer;
    char date[DATE_STRING_LENGTH];
} Bill;

Product inventory[MAX_PRODUCTS];
Customer customers[MAX_CUSTOMERS];
Bill bills[MAX_BILLS];
int productCount = 0;
int customerCount = 0;
int billCount = 0;
float discountRate = 0.0;
float totalSalesAmount = 0.0;

void addProduct();
void updateProduct();
void deleteProduct();
void viewProducts();
void deleteProduct();
void sellProduct();
void searchProduct();
void addCustomer();
void viewCustomers();
void deleteCustomer();
void generateBill(Product products[], int product_count, float total_amount, Customer customer);
void getCurrentDate(char *date);
void loadProducts();
void saveProducts();
void loadCustomers();
void saveCustomers();
void clearScreen();
void saveBill(Bill bill);
void loadBills();
void saveTotalSalesAmount();
void loadTotalSalesAmount();
void generateEndOfDayReport();
int isRegularCustomer(Customer customer);
float applyDiscount(float total_amount, int product_count, Customer customer);

int main()
{
    loadProducts();
    loadCustomers();
    loadBills();
    loadTotalSalesAmount();
    int choice;

    while (1)
    {
        clearScreen();
        printf("\n--Welcome to IIUC SUPERSHOP--\n");
        printf("Please Select an Option:\n");
        printf("1. Add Product\n");
        printf("2. Update Product\n");
        printf("3. Delete Product\n");
        printf("4. View Product\n");
        printf("5. Sell Product\n");
        printf("6. Search Product\n");
        printf("7. Add Customer\n");
        printf("8. View Customers\n");
        printf("9. Delete Customer\n");
        printf("10. Generate End of Day Report\n");
        printf("11. Exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        clearScreen();
        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            updateProduct();
            break;
        case 3:
            deleteProduct();
            break;
        case 4:
            viewProducts();
            break;
        case 5:
            sellProduct();
            break;
        case 6:
            searchProduct();
            break;
        case 7:
            addCustomer();
            break;
        case 8:
            viewCustomers();
            break;
        case 9:
            deleteCustomer();
            break;
        case 10:
            generateEndOfDayReport();
            break;
        case 11:
            saveProducts();
            saveCustomers();
            saveTotalSalesAmount();
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
        printf("\nPress Enter to continue....");
        while (getchar() != '\n')
            ;
        getchar();
    }
    return 0;
}

void addProduct()
{
    if (productCount >= MAX_PRODUCTS)
    {
        printf("Inventory is full.\n");
        return;
    }

    Product newProduct;
    newProduct.product_id = productCount + 1;
    printf("Enter product name : ");
    scanf("%s", newProduct.product_name);
    printf("Enter price : ");
    scanf("%f", &newProduct.price);
    printf("Enter quantity : ");
    scanf("%d", &newProduct.quantity);

    inventory[productCount++] = newProduct;
    printf("Product added successfully.\n");
    saveProducts();
    printf("\n----------------------------Powered by CodeWhite\n");
}

void updateProduct()
{
    int id;
    printf("Enter product ID to update : ");
    scanf("%d", &id);

    if (id <= 0 || id > productCount)
    {
        printf("Invalid product ID.\n");
        return;
    }

    printf("Enter new product name : ");
    scanf("%s", inventory[id - 1].product_name);
    printf("Enter new price : ");
    scanf("%f", &inventory[id - 1].price);
    printf("Enter new quantity : ");
    scanf("%d", &inventory[id - 1].quantity);

    printf("Product updated successfully.\n");
    printf("\n----------------------------Powered by CodeWhite\n");
    saveProducts();
}

void deleteProduct()
{
    int id;
    printf("Enter product ID to delete : ");
    scanf("%d", &id);

    if (id <= 0 || id > productCount)
    {
        printf("Invalid product ID.\n");
        return;
    }

    for (int i = id - 1; i < productCount - 1; i++)
    {
        inventory[i] = inventory[i + 1];
    }
    productCount--;

    printf("Product deleted successfully.\n");
    printf("\n----------------------------Powered by CodeWhite\n");
    saveProducts();
}

void viewProducts()
{
    if (productCount == 0)
    {
        printf("No products in inventory.\n");
        return;
    }

    printf("\nInventory :\n");
    printf("ID\tName\t\t\t\tPrice\tQuantity\n");
    for (int i = 0; i < productCount; i++)
    {
        printf("%d\t%s\t\t\t\t%.2f\t%d\n", inventory[i].product_id, inventory[i].product_name, inventory[i].price, inventory[i].quantity);
    }
    printf("\n----------------------------Powered by CodeWhite\n");
}

void sellProduct()
{
    if (productCount == 0)
    {
        printf("No products available for sale.\n");
        return;
    }
    Customer currentCustomer;
    printf("Enter customer name : ");
    scanf("%s", currentCustomer.customer_name);
    printf("Enter contact number : ");
    scanf("%s", currentCustomer.contact_number);

    Product products[MAX_PRODUCTS];
    int product_count = 0;
    float total_amount = 0;

    while (1)
    {
        int id, quantity;
        printf("Enter product ID to sell (or 0 to finish) : ");
        scanf("%d", &id);

        if (id == 0)
            break;

        if (id <= 0 || id > productCount)
        {
            printf("Invalid product ID.\n");
            continue;
        }

        printf("Enter quantity to sell : ");
        scanf("%d", &quantity);

        if (inventory[id - 1].quantity < quantity)
        {
            printf("Insufficient quantity in stock.\n");
            continue;
        }

        inventory[id - 1].quantity -= quantity;
        products[product_count] = inventory[id - 1];
        products[product_count].quantity = quantity;
        total_amount += inventory[id - 1].price * quantity;
        product_count++;
    }

    if (product_count > 0)
    {
        total_amount = applyDiscount(total_amount, product_count, currentCustomer);
        generateBill(products, product_count, total_amount, currentCustomer);
        saveProducts();
        totalSalesAmount += total_amount;
        saveTotalSalesAmount();
    }
    else
    {
        printf("No products sold.\n");
    }
}

void searchProduct()
{
    int id;
    printf("Enter product ID to search : ");
    scanf("%d", &id);

    if (id <= 0 || id > productCount)
    {
        printf("Invalid product ID.\n");
        return;
    }

    printf("Product found :\n");
    printf("ID : %d\n", inventory[id - 1].product_id);
    printf("Name : %s\n", inventory[id - 1].product_name);
    printf("Price : %.2f\n", inventory[id - 1].price);
    printf("Quantity : %d\n", inventory[id - 1].quantity);
    printf("\n----------------------------Powered by CodeWhite\n");
}

void addCustomer()
{
    if (customerCount >= MAX_CUSTOMERS)
    {
        printf("Customer list is full.\n");
        return;
    }

    Customer newCustomer;
    newCustomer.customer_id = customerCount + 1;
    printf("Enter customer name : ");
    scanf("%s", newCustomer.customer_name);
    printf("Enter contact number : ");
    scanf("%s", newCustomer.contact_number);

    customers[customerCount++] = newCustomer;
    printf("Customer added successfully.\n");
    printf("\n----------------------------Powered by CodeWhite\n");
    saveCustomers();
}

void viewCustomers()
{
    if (customerCount == 0)
    {
        printf("No customers found.\n");
        return;
    }

    printf("\nCustomers :\n");
    printf("ID\tName\t\tContact Number\n");
    for (int i = 0; i < customerCount; i++)
    {
        printf("%d\t%s\t\t%s\n", customers[i].customer_id, customers[i].customer_name, customers[i].contact_number);
    }
    printf("\n----------------------------Powered by CodeWhite\n");
}

void deleteCustomer()
{
    int id;
    printf("Enter customer ID to delete : ");
    scanf("%d", &id);

    if (id <= 0 || id > customerCount)
    {
        printf("Invalid customer ID.\n");
        return;
    }

    for (int i = id - 1; i < customerCount - 1; i++)
    {
        customers[i] = customers[i + 1];
    }
    customerCount--;

    printf("Customer deleted successfully.\n");
    printf("\n----------------------------Powered by CodeWhite\n");
    saveCustomers();
}

void generateBill(Product products[], int product_count, float total_amount, Customer customer)
{
    Bill newBill;
    newBill.bill_id = billCount + 1;
    newBill.product_count = product_count;
    newBill.total_amount = total_amount;
    getCurrentDate(newBill.date);
    newBill.customer = customer;

    for (int i = 0; i < product_count; i++)
    {
        newBill.product_list[i] = products[i];
    }

    bills[billCount++] = newBill;
    clearScreen();

    printf("\nBill Generated :\n");
    printf("Bill ID : %d\n", newBill.bill_id);
    printf("Date : %s\n", newBill.date);
    printf("Customer Name : %s\n", newBill.customer.customer_name);
    printf("Contact Number : %s\n", newBill.customer.contact_number);
    printf("Products :\n");
    for (int i = 0; i < product_count; i++)
    {
        printf("ID : %d, Name : %s, Quantity : %d, Price : %.2f\n", products[i].product_id, products[i].product_name, products[i].quantity, products[i].price);
    }
    applyDiscount(total_amount, product_count, customer);
    printf("Total Amount : %.2f\n", total_amount);
    printf("\n---Thank You For Purchasing From Our Shop---\n");
    printf("------------Please Come Again---------------\n");
    printf("\n----------------------------Powered by CodeWhite\n");
    saveBill(newBill);
}

void getCurrentDate(char *date)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadProducts()
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
    {
        return;
    }

    productCount = 0;
    while (fscanf(file, "%d %49s %f %d", &inventory[productCount].product_id, inventory[productCount].product_name, &inventory[productCount].price, &inventory[productCount].quantity) != EOF)
    {
        productCount++;
    }

    fclose(file);
}

void saveProducts()
{
    FILE *file = fopen("products.txt", "w");
    if (file == NULL)
    {
        printf("Error saving products.\n");
        return;
    }

    for (int i = 0; i < productCount; i++)
    {
        fprintf(file, "%d %s %.2f %d\n", inventory[i].product_id, inventory[i].product_name, inventory[i].price, inventory[i].quantity);
    }

    fclose(file);
}

void loadCustomers()
{
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL)
    {
        return;
    }

    customerCount = 0;
    while (fscanf(file, "%d %49s %14s", &customers[customerCount].customer_id, customers[customerCount].customer_name, customers[customerCount].contact_number) != EOF)
    {
        customerCount++;
    }

    fclose(file);
}

void saveCustomers()
{
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL)
    {
        printf("Error saving customers.\n");
        return;
    }

    for (int i = 0; i < customerCount; i++)
    {
        fprintf(file, "%d %s %s\n", customers[i].customer_id, customers[i].customer_name, customers[i].contact_number);
    }

    fclose(file);
}

void saveBill(Bill bill)
{
    FILE *file = fopen("bills.txt", "a");
    if (file == NULL)
    {
        printf("Error saving bill.\n");
        return;
    }

    fprintf(file, "%d %s %.2f %d %s %s\n", bill.bill_id, bill.date, bill.total_amount, bill.product_count, bill.customer.customer_name, bill.customer.contact_number);
    for (int i = 0; i < bill.product_count; i++)
    {
        fprintf(file, "%d %s %.2f %d\n",
                bill.product_list[i].product_id, bill.product_list[i].product_name,
                bill.product_list[i].price, bill.product_list[i].quantity);
        applyDiscount(bill.total_amount, bill.product_count, bill.customer);
    }

    fclose(file);
}

void loadBills()
{
    FILE *file = fopen("bills.txt", "r");
    if (file == NULL)
    {
        return;
    }

    billCount = 0;
    while (fscanf(file, "%d %s %f %d %s %s", &bills[billCount].bill_id, bills[billCount].date, &bills[billCount].total_amount, &bills[billCount].product_count, bills[billCount].customer.customer_name, bills[billCount].customer.contact_number) != EOF)
    {
        for (int i = 0; i < bills[billCount].product_count; i++)
        {
            fscanf(file, "%d %s %f %d",
                   &bills[billCount].product_list[i].product_id, bills[billCount].product_list[i].product_name,
                   &bills[billCount].product_list[i].price, &bills[billCount].product_list[i].quantity);
            applyDiscount(bills[billCount].total_amount, bills[billCount].product_count, bills[billCount].customer);
        }
        billCount++;
    }

    fclose(file);
}

void saveTotalSalesAmount()
{
    FILE *file = fopen("total_sales.txt", "w");
    if (file == NULL)
    {
        printf("Error saving total sales amount.\n");
        return;
    }

    fprintf(file, "%.2f\n", totalSalesAmount);

    fclose(file);
}

void loadTotalSalesAmount()
{
    FILE *file = fopen("total_sales.txt", "r");
    if (file == NULL)
    {
        return;
    }

    fscanf(file, "%f", &totalSalesAmount);

    fclose(file);
}

float applyDiscount(float total_amount, int product_count, Customer customer)
{
    float discount = 0;
    if (product_count > 400)
    {
        discount += total_amount * 0.07;
    }
    if (isRegularCustomer(customer))
    {
        discount += total_amount * 0.05;
    }
    total_amount -= discount;
    printf("Discount applied : %.2f\n", discount);
    return total_amount;
}

int isRegularCustomer(Customer customer)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(customers[i].customer_name, customer.customer_name) == 0 &&
            strcmp(customers[i].contact_number, customer.contact_number) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void generateEndOfDayReport()
{
    printf("\nEnd-of-Day Report :\n");
    printf("Total sales amount : %.2f\n", totalSalesAmount);
    printf("Number of transactions : %d\n", billCount);
    printf("\nBill Details :\n");

    for (int i = 0; i < billCount; i++)
    {
        printf("Bill ID : %d\n", bills[i].bill_id);
        printf("Date : %s\n", bills[i].date);
        printf("Customer Name : %s\n", bills[i].customer.customer_name);
        printf("Contact Number : %s\n", bills[i].customer.contact_number);
        for (int j = 0; j < bills[i].product_count; j++)
        {
            printf("Product ID : %d, Name : %s, Quantity : %d, Price : %.2f\n",
                   bills[i].product_list[j].product_id, bills[i].product_list[j].product_name,
                   bills[i].product_list[j].quantity, bills[i].product_list[j].price);
            applyDiscount(bills[i].total_amount, bills[i].product_count, bills[i].customer);
        }
        printf("Total Amount : %.2f\n\n", bills[i].total_amount);
    }
    FILE *file = fopen("end_of_day_report.txt", "w");
    if (file != NULL)
    {
        fprintf(file, "End-of-Day Report :\n");
        fprintf(file, "Total sales amount : %.2f\n", totalSalesAmount);
        fprintf(file, "Number of transactions : %d\n", billCount);
        fprintf(file, "\nBill Details :\n");

        for (int i = 0; i < billCount; i++)
        {
            fprintf(file, "Bill ID : %d\nDate : %s\n", bills[i].bill_id, bills[i].date);
            fprintf(file, "Customer Name : %s\n", bills[i].customer.customer_name);
            fprintf(file, "Contact Number : %s\n", bills[i].customer.contact_number);
            for (int j = 0; j < bills[i].product_count; j++)
            {
                fprintf(file, "Product ID : %d, Name : %s, Quantity : %d, Price : %.2f\n",
                        bills[i].product_list[j].product_id, bills[i].product_list[j].product_name,
                        bills[i].product_list[j].quantity, bills[i].product_list[j].price);
            }
            fprintf(file, "Total Amount : %.2f\n\n", bills[i].total_amount);
        }

        fclose(file);
        printf("End-of-day report saved to 'end_of_day_report.txt'.\n");
        printf("\n----------------------------Powered by CodeWhite\n");
    }
    else
    {
        printf("Error saving end-of-day report.\n");
    }
}