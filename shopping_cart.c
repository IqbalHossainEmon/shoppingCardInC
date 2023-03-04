#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct product
{
    char name[30];
    int quantity;
    float price;
} pro;

void fileReader(pro cart[], int *added, int *size)
{
    FILE *data;
    data = fopen("cart.txt", "r");
    if (fgetc(data) == EOF)
    {
        fclose(data);
        return;
    }
    fclose(data);
    data = fopen("cart.txt", "r");
    if (data == NULL)
    {
        printf("Can't Open the file. Try again.");
        return;
    }
    do
    {
        char ch = ' ';
        int j = 0;
        while (ch != '*' && ch != -1)
        {
            ch = fgetc(data);
            if (ch != '*' && ch != '\n' && ch != -1)
            {
                cart[*added].name[j] = ch;
            }
            j++;
        }
        cart[*added].name[j - 1] = '\0';
        if (ch == -1)
        {
            break;
        }

        fscanf(data, "%d*%f", &cart[*added].quantity, &cart[*added].price);
        if ((*added) > 100)
        {
            cart = (pro *)realloc(cart, (*size + 100));
            (*size) = (*size) + 100;
        }
        (*added)++;
    } while (fgetc(data) != -1);
    fclose(data);
}

void display()
{
    printf("\n\n\t\tMenu\n"
           "\t----------------------\t\n\n"
           "\nEnter 1 to Add a Product."
           "\nEnter 2 to Display All Product sorted by price."
           "\nEnter 3 to Delete a product from the cart."
           "\nEnter 4 to Update a product Info."
           "\nEnter 5 to Search a product by name."
           "\nEnter 6 to Exit the program."
           "\nPlease enter a number (1-7) : ");
}

void addProduct(pro cart[], int *added, int *size)
{
    printf("\n\n\t\tAdd a New Product"
           "\n\t-------------------------------\n\n");
    fflush(stdin);
    printf("\n\nPlease Enter Product name (1-30 characters): ");
    gets(cart[*added].name);
    printf("\nPlease Enter Product's Quantity: ");
    scanf("%d", &cart[*added].quantity);
    printf("\nPlease Enter the Products Price: ");
    scanf("%f", &cart[*added].price);

    FILE *data;
    data = fopen("cart.txt", "a");
    if (data == NULL)
    {
        printf("\nSomething went wrong. Can't open the file.\n");
        return;
    }
    fprintf(data, "%s*%d*%1.2f\n", cart[*added].name, cart[*added].quantity, cart[*added].price);
    fclose(data);
    printf("\nNew Product Added Successfully.\n");
    printf("\n\n------------------------------------------------\n\n");
    if ((*added) > 100)
    {
        cart = (pro *)realloc(cart, (*size + 100));
        (*size) = (*size) + 100;
    }
    (*added)++;
}
void merge(pro cart[], int left, int mid, int right)
{
    int i, j, k = left;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    pro L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = cart[left + i];
    for (j = 0; j < n2; j++)
        R[j] = cart[mid + 1 + j];
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (L[i].price <= R[j].price)
        {
            cart[k] = L[i];
            i++;
        }
        else
        {
            cart[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        cart[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        cart[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(pro cart[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(cart, left, mid);
        mergeSort(cart, mid + 1, right);

        merge(cart, left, mid, right);
    }
}

void print_by_price(pro cart[], int *added)
{
    printf("\n\nAll Product in shopping cart sorted by PRICE."
           "\2\n-----------------------------------------------");
    printf("\n\n\n%4s\t%-20s%-10s\t%-10s\n", "No", "Name", "Quantity", "Price");
    mergeSort(cart, 0, *added - 1);
    for (int i = 0; i < *added; i++)
    {
        printf("%3d.\t%-20s%-10d\t%-10.2f\n", i + 1, cart[i].name, cart[i].quantity, cart[i].price);
    }
    printf("\n\n------------------------------------------------\n\n");
}

int linearSearch(pro cart[], int *added)
{
    int i = 0;
    char name[30];
    fflush(stdin);
    gets(name);
    while (strcmp(cart[i].name, name) && i < *added)
    {
        printf("%d\n", !strcmp(cart[i].name, name));
        i++;
    }
    if (i == *added)
    {
        printf("\nProduct not Found.\n\n");
        return -1;
    }
    return i;
}

void deleteProduct(pro cart[], int *added)
{
    printf("\nDelete Product"
           "\n--------------");
    printf("\n\nEnter the name of the product you want to delete : ");
    int i = linearSearch(cart, added);
    if (i < 0)
    {
        return;
    }
    printf("\nThis product is Deleted:\n"
           "\n%-20s \t%-10s %-5s\n"
           "\n%-20s \t%-10d %-10.2f\n",
           "Name", "Quantity", "Price", cart[i].name, cart[i].quantity, cart[i].price);

    while (i < *added - 1)
    {
        cart[i] = cart[i + 1];
        i++;
    }

    (*added)--;
    FILE *data;
    data = fopen("cart.txt", "w");
    if (data == NULL)
    {
        printf("\nSomething went wrong. Can't open the file.\n");
        return;
    }
    for (int j = 0; j < *added; j++)
    {
        fprintf(data, "%s*%d*%1.2f\n", cart[j].name, cart[j].quantity, cart[j].price);
    }

    fclose(data);
    printf("\nProduct Deleted Successfully.\n");
    printf("\n\n------------------------------------------------\n\n");
}
void updateProduct(pro cart[], int *added)
{
    printf("\nUpdate Product"
           "\n----------------");
    printf("\n\nEnter the product name you want to update : ");
    int i = linearSearch(cart, added), option;
    if (i < 0)
    {
        return;
    }
    printf("The Product name is : %-30s\n"
           "quantity is : %-10d\n"
           "price is : %-1.2f\n"
           "\nWhat do you want to update:\n"
           "1.Name\n"
           "2.Quantity\n"
           "3.Price\n"
           "4.Everything\n"
           "Option: ",
           cart[i].name, cart[i].quantity, cart[i].price);
    scanf("%d", &option);
    if (option == 1 || option == 4)
    {
        printf("\nNew name: ");
        fflush(stdin);
        gets(cart[i].name);
    }
    if (option == 2 || option == 4)
    {
        printf("New Quantity: ");
        scanf("%d", &cart[i].quantity);
    }
    if (option == 3 || option == 4)
    {
        printf("New Price: ");
        scanf("%f", &cart[i].price);
    }

    printf("\nThe updated product is:\n"
           "%4s\t%-20s%-10s\t%-10s\n"
           "%3d.\t%-20s%-10d\t%-10.2f\n",
           "No", "Name", "Quantity", "Price", i + 1, cart[i].name, cart[i].quantity, cart[i].price);
    FILE *data;
    data = fopen("cart.txt", "w");
    if (data == NULL)
    {
        printf("\nSomething went wrong. Can't open the file.\n");
        return;
    }
    for (int j = 0; j < *added; j++)
    {
        fprintf(data, "%s*%d*%1.2f\n", cart[j].name, cart[j].quantity, cart[j].price);
    }

    fclose(data);
    printf("\nProduct update Successfully.\n");
    printf("\n\n------------------------------------------------\n\n");
}
void searchProduct(pro cart[], int *added)
{
    printf("\nSearch Product."
           "\n---------------");
    printf("\n\nEnter the product name you want to search : ");
    int i = linearSearch(cart, added);
    if (i < 0)
    {
        return;
    }
    printf("Name is : %-30s\n", cart[i].name);
    printf("quantity is : %-10d\n", cart[i].quantity);
    printf("Price is : %-1.2f\n", cart[i].price);
    printf("\n\n------------------------------------------------\n\n");
}

void mainWork(pro cart[], int *added, int *size)
{
    while (1)
    {
        display();
        switch (getch())
        {
        case '1':
            printf("1\n");
            addProduct(cart, added, size);
            break;
        case '2':
            printf("2\n");
            print_by_price(cart, added);
            break;
        case '3':
            printf("3\n");
            deleteProduct(cart, added);
            break;
        case '4':
            printf("4\n");
            updateProduct(cart, added);
            break;
        case '5':
            printf("5\n");
            searchProduct(cart, added);
            break;
        case '6':
            printf("exit\n");
            exit(0);
            break;
        default:
            printf("\n\nPlease Enter a valid Number.\n\n");
            break;
        }
    }
}

int main()
{
    int size = 100;
    int added = 0;
    pro *cart;
    cart = (pro *)calloc(size, sizeof(pro));
    printf("\n\n=================================================="
           "\n\tWellcome to Shopping cart System.\n"
           "==================================================");
    fileReader(cart, &added, &size);
    mainWork(cart, &added, &size);
    free(cart);
    return 0;
}