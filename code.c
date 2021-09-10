#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define MAX_NAME 100
#define MAX_TEST 10
#define FILE_CHEMICAL "chemicals.bin"
#define FILE_TEST "tests.bin"
#define FILE_REPORT "reports.bin"
typedef struct {
    char name[MAX_NAME];
    int id;
    int quantity;
}Chemical;

typedef struct {
    int id;
    char name[MAX_NAME];
    int quantity;
}Chem_req;

typedef struct {
    char name[MAX_NAME];
    int id;
    int n_o_c;    // number of chemicals: COUNT
    Chem_req test[MAX_TEST];
    int amount;
}Test;

typedef struct {
    char user_name[MAX_NAME];
    int id;
    int test_to_be_done[MAX_TEST];
    int total_amount;
}Report;

void welcome() {
    printf("Hello Welcome!!! To ....LABMATE.....");
}

int validate_chem(char name[MAX_NAME], int quantity){
    Chemical temp_chem;
    int found = 0;
    FILE *fp = NULL;
    fp = fopen(FILE_CHEMICAL, "rb"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    while (fread(&temp_chem, sizeof(temp_chem), 1, fp))
    {
        if(strcmp(temp_chem.name,name) == 0){
            found = temp_chem.quantity;
            break;
        }
    }
    fclose(fp);
    return found;
}


void add_test() {
    
    fflush(stdin);
    system("CLS||CLEAR");

    Test temp_test;
    int result;
    printf("Enter the test details: \n");
    printf("Enter the Test Name:  ");
    scanf("%s",&temp_test.name);
    fflush(stdin);
    printf("Enter  Number Of Chemical (n_o_c) required:  ");
    scanf("%d", &temp_test.n_o_c);
    fflush(stdin);
    printf("Enter Name of chemicals:\n");
    for(int i = 0; i < temp_test.n_o_c; i++){
        printf("Chemical %d :  ",i+1);
        scanf("%s",&temp_test.test[i].name);
        result = validate_chem(temp_test.test[i].name,0);
        if(result == 0) {
            printf("Chemical not available in inventory!!!\n Add before continue\n");
            printf("\n\n\n\t\t\tPress any key to go to main menu.....");
            getchar();
            return ;
        } 
        fflush(stdin);
        printf("Enter the quantity required (in units) of the chemical in the test: ");
        scanf("%d",&temp_test.test[i].quantity);
        fflush(stdin);
    }
    fflush(stdin);
    printf("Enter the cost of Test: ");
    scanf("%d",&temp_test.amount);
    temp_test.id = 00;
    fflush(stdin);
    FILE *fp = NULL;
    fp = fopen(FILE_TEST, "ab+"); // test.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fwrite(&temp_test, sizeof(temp_test), 1, fp);
    fclose(fp);   // test.bin closed
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}
void add_chemical()
{   
    fflush(stdin);
    system("CLS||CLEAR");
    Chemical temp_chem;
    printf("Enter the chemical details : \n");
    printf("Enter chemical name:  ");
    scanf("%s",&temp_chem.name);
    fflush(stdin);
    printf("Enter chemical quantity: ");
    scanf("%d",&temp_chem.quantity);
    temp_chem.id = 00;
    fflush(stdin);
    FILE *fp = NULL;
    fp = fopen(FILE_CHEMICAL, "ab+"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fwrite(&temp_chem, sizeof(temp_chem), 1, fp);
    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}

int update_stock(char chem_name[MAX_NAME], int quantity)
{
    fflush(stdin);
    Chemical temp_chem;
    FILE *fp = NULL;
    fp = fopen(FILE_CHEMICAL, "rb+"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    int found = 0;

    while (fread(&temp_chem, sizeof(temp_chem), 1, fp))
    {
        // printf("name: %s\n name_temp: %s\n",chem_name,temp_chem.name);
        if (strcmp(temp_chem.name, chem_name) == 0)
        {
            temp_chem.quantity += quantity;
            found=1;
            fseek(fp,(long) -sizeof(temp_chem), SEEK_CUR);
            fwrite(&temp_chem, sizeof(temp_chem), 1, fp);
            break;
        }
    }
    fclose(fp);
    return found;
}

void check_chemical_availablelity()
{
    fflush(stdin);
    system("CLS||CLEAR");
    Chemical temp_chem;
    printf("The chemical details are : \n");
    FILE *fp = NULL;
    fp = fopen(FILE_CHEMICAL, "rb"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    while(fread(&temp_chem,sizeof(temp_chem),1,fp)) {
        printf("\n");
        printf("%s, %d, %d", temp_chem.name,temp_chem.id,temp_chem.quantity);
        printf("\n");
    }
    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}

void perform_test()
{
    fflush(stdin);
    system("CLS||CLEAR");
    FILE *fp = NULL;
    fp = fopen(FILE_TEST, "rb+"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    Test temp_test;
    char test_name[MAX_NAME];
    int count = 0;
    printf("Note: Enter the name of test to perform(in the same format as displayed below):\n");
    printf("Tests available are:");
    
    while (fread(&temp_test, sizeof(temp_test), 1, fp))
    {
        printf("\nTest no. %d : \n", count++);
        printf("Test Name:  ");
        printf("%s\n", temp_test.name);
        printf("cost of Test: ");
        printf("%d\n", temp_test.amount);
        printf("\n");
    }
    // fclose(fp);
    fseek(fp,0,SEEK_SET);
    printf("\n\nEnter the name of test You want to perform => ");
    scanf("%s",test_name);
    fflush(stdin);
    int flag = 0;
    int error=0;
    while(fread(&temp_test, sizeof(temp_test),1,fp))
    {
        if(strcmp(temp_test.name,test_name) == 0){
            for (int j = 0; j < temp_test.n_o_c; j++)
            {
                if(!validate_chem(temp_test.test[j].name,temp_test.test[j].quantity)){
                    error=1;
                    break;
                }

            }

            if(!error)
            {
                for (int j = 0; j < temp_test.n_o_c; j++)
                {
                    update_stock(temp_test.test[j].name, temp_test.test[j].quantity * -1);
                }
                flag = 1;
                printf("Test performed and chemical quantity reduced.\n");
            }
            break;
            
        }
    }
    if(flag == 0){
        printf("Test not performed!!!!\n");
    }
    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}
void check_test_availablelity()
{
    fflush(stdin);
    system("CLS||CLEAR");
    printf("The Test details are : \n");
    FILE *fp = NULL;
    fp = fopen(FILE_TEST, "rb"); // chemicals.bin opened
    if (fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    Test temp_test;
    int result;
    int count = 1;
    while (fread(&temp_test, sizeof(temp_test), 1, fp))
    {
        printf("\n Test no. %d : \n",count++ );
        printf("Test Name:  ");
        printf("%s\n", temp_test.name);
        printf("Number Of Chemical (n_o_c) required are:  ");
        printf("%d\n", temp_test.n_o_c);
        printf("\n");
        printf("Name of chemical is :\n");
        for (int i = 0; i < temp_test.n_o_c; i++)
        {
            printf("\n");
            printf("Chemical %d\n :  ", i + 1);
            printf("%s\n", temp_test.test[i].name);
            result = validate_chem(temp_test.test[i].name, 0);
            if (result == 0)
            {
                printf("Chemical not available in inventory!!!\n Add before continue\n");
                return;
            }
            printf("Quantity required (in units) of the chemical in the test are: ");
            printf("%d\n", temp_test.test[i].quantity);
            printf("\n");
        }
        printf("cost of Test: ");
        printf("%d\n", temp_test.amount);
        temp_test.id = 00;
        printf("\n");
    }
    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}
void restock()
{

    fflush(stdin);
    system("CLS||CLEAR");
    printf("Enter the test name to restock:");
    char string[MAX_NAME];
    fflush(stdin);
    scanf("%s",&string);
    fflush(stdin);
    printf("enter the quantity: ");
    int quantity;
    scanf("%d",&quantity);
    fflush(stdin);

    if(validate_chem(string,quantity))
    {
        update_stock(string, quantity);
        printf("Stock udpated");
    }
    else
    {
        printf("error in update of stock");

    }

    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}



void menu() {
    int choice = 0;
    do{
        printf("Main Menu: \n");
        printf("1. Add Test\n");
        printf("2. Add chemical\n");
        printf("3. Display Test\n");
        printf("4. Display chemicals\n");
        printf("5. Perform Test\n");
        printf("6. Restock\n");
        printf("0. Exit\n");
        printf("Enter Your Choice => ");
        scanf("%d",&choice);
        switch(choice){
            case 1: 
            add_test();
            break;
            case 2:
            add_chemical();
            break;
            case 3:
            check_test_availablelity();
            break;
            case 4:
            check_chemical_availablelity();
            break;
            case 5:
            perform_test();
            break;
            case 6:
            restock();
            break;
            case 0:
            exit(1);
            break;
            default:
            printf("Oops, U entered something wrong!!!! Plz enter a valid choice or press 0 to exit. \n ThankYou!\n");
        }
    }while(choice != 0);
}
void init(){
    printf("hello");
} 
int main() {
    init();
    welcome();
    menu();
    return 0;
}
