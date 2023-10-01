/*
 * This program allows to convert dec, bin, and hex values into
 * the other formats.
 * Furthermore, it is possible to compute basic arithmetic ops
 * on dec, bin, and hex formats. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculator.h"

int add(int a, int b) 
{
    /* Adds to integer values together. */
    int c = a + b;
    return c;
}

int subtract(int a, int b) 
{
    /* Subtract one integer number from another. */
    int c = a - b;
    return c;
}

int multiply(int a, int b) 
{
    /* Computes the product of two integer values. */
    int c = a * b;
    return c;
}

int divide(int a, int b) 
{
    /* Divides one integer by another. */
    int c = a / b;
    return c;
}

int power_of(int a, int b) 
{
    /* Cumptes the bth power of an integer a. */
    int c = 1;
    while (b > 0) {
        c *= a;
        b--;
    }
    return c;
}

int gen_decimal(char *value, char *format)
{
    /* Generate decimal format from input character string. */
    int c;
    // Check input formats. 
    if (!strcmp(format, "b"))
        c = bin_to_dec(value);
    else if (!strcmp(format, "x"))
        c = hex_to_dec(value);
    else
        c = atol(value);
    return c;
}

int bin_to_dec(char *value)
{
    /* Converts string bit sequence into an integer value. */
    int c = 0;
    while (*value != '\0') {
        c = 2 * c + (*value++ - '0');
    }
    return c;
}

int hex_to_dec(char *value)
{
    /* Converts string hex sequence into an integer value. */
    int a, b;
    int c = 0;
    while (*value != '\0') {
        a = (*value - '0');
        if (a < 10) {
            c = 16 * c + (*value++ - '0');
        }
        else {
            switch(a)
            {
                case 49:
                    b = 10;
                    break;
                case 50:
                    b = 11;
                    break;
                case 51:
                    b = 12;
                    break;
                case 52:
                    b = 13;
                    break;
                case 53:
                    b = 14;
                    break;
                case 54:
                    b = 15;
                    break;
            }
            c = 16 * c + b;
            value++;
        }
    }
    return c;
}

void dec_to_hex(int a)
{
    /* Prints a hex version of the input argument. */
    int mod;
    int cnt = 0;
    char s[100], temp[2];
    strcpy(s, "x");
    // Create a character string. 
    while (a > 0) 
    {
        if (cnt == 4)
        {
            strcat(s, " ");
            cnt = 0;
        }
        mod = a%16;
        if (mod > 9) {
            switch(mod)
            {
                case 10:
                    strcat(s, "a");
                    break;
                case 11:
                    strcat(s, "b");
                    break;
                case 12:
                    strcat(s, "c");
                    break;
                case 13:
                    strcat(s, "d");
                    break;
                case 14:
                    strcat(s, "e");
                    break;
                case 15:
                    strcat(s, "f");
                    break;
            }
        }
        else
        {
            sprintf(temp, "%d", mod);
            // temp = atol(mod, temp, 10);
            strcat(s, temp);
        }
        a = a/16;
        cnt++;
    }
    // Print string in reverse order. 
    int start = strlen(s)-1;
    for (int i = start; i > 0; i--)
        printf("%c", s[i]);
    printf("\n");
}

void dec_to_bin(int a, char *c)
{
    /* Prints a bin version of the input argument. */
    // Create character string.
    int cnt = 0; 
    char s[100];
    strcpy(s, "b");
    while (a > 0)
    {
        if (cnt == 4)
        {
            strcat(s, " ");
            cnt = 0;
        }
        if (a%2)
            strcat(s, "1");
        else
            strcat(s, "0");
        a = a/2;
        cnt++;
    }
    // Print string in reverse order. 
    int start = strlen(s)-1;
    for (int i = start; i > 0; i--)
        printf("%c", s[i]);
    printf("\n");
}

void print_results(int r, char *c)
{
    /* Print the result in dec, bin, and hex format. */
    printf("Dec: %d\n", r);
    printf("Bin: ");
    dec_to_bin(r, c);
    printf("Hex: ");
    dec_to_hex(r);
    printf("------------------------------------\n");
}


int main(int argc, char* argv[]) 
{
    // Start calculator.
    printf("Welcome to this amazing calculator.\n");
    printf("It helps you to compute numbers and switch between different number formats.\n");
    
    char *c = (char *)malloc(64*sizeof(char));
    char *task = (char *)malloc(2*sizeof(char));
    char *op = (char *)malloc(2*sizeof(char));
    char *a = (char *)malloc(64*sizeof(char));
    char *b = (char *)malloc(64*sizeof(char));
    char *f1 = (char *)malloc(2*sizeof(char));
    char *f2 = (char *)malloc(2*sizeof(char));
    char *yn = (char *)malloc(1*sizeof(char));
    // char task[2], op[2], a[64], b[64], f1[2], f2[2], yn[1];
    int x, y, r;
    int check = 1;
    
    while (check)
    {
        // Ask the user what to do next. 
        printf("Choose your next operation.\n");
        printf("Convert number [0] or calculate numbers [1]: ");
        scanf("%s", task);

        if (!strcmp(task, "0"))
        {
            // Ask user for the input format. 
            printf("Provide input format of value (dec (d), bin (b), hex (x)): ");
            scanf("%s", f1);
            printf("Which value would you like to show in the different formats: ");
            scanf("%s", a);
            // Display results. 
            x = gen_decimal(a, f1);
            print_results(x, c);
        }
        else {
            // Ask user for target operation. 
            printf("Type in +, -, *, /, ^ for the corresponding operation: ");
            scanf("%s", op);

            // Ask user for the two input formats. 
            printf("Provide input format first value (dec (d), bin (b), hex (x)): ");
            scanf("%s", f1);
            printf("Provide input format second value (dec (d), bin (b), hex (x)): ");
            scanf("%s", f2);
            printf("------------\n");

            // Ask user for the input arguments.
            if (!strcmp(op, "/"))
            {
                printf("Which value would you like to divide: ");
                scanf("%s", a);
                printf("By which value would you like to divide: ");
                scanf("%s", b);
            }
            else if (!strcmp(op, "^"))
            {
                printf("Type in your base value: ");
                scanf("%s", a);
                printf("Type in your exponent: ");
                scanf("%s", b);
            }
            else {
                printf("First input value: ");
                scanf("%s", a);
                printf("Second input value: ");
                scanf("%s", b);
            }

            // Create a decimal format. 
            x = gen_decimal(a, f1);
            y = gen_decimal(b, f2);
        
            // Perform operation and print result to the terminal. 
            if (!strcmp(op, "+"))
                r = add(x, y);
            else if (!strcmp(op, "-"))
                r = subtract(x, y);
            else if (!strcmp(op, "*"))
                r = multiply(x, y);
            else if (!strcmp(op, "/"))
                r = divide(x, y);
            else
                r = power_of(x, y);

            // Print results to the console. 
            print_results(r, c);
        }

        // Exit program. 
        printf("Continue ([y], [n])? ");
        scanf("%s", yn);
        if (!strcmp(yn, "n"))
        {
            printf("Thanks for working with me.\nHave a great day ahead.\n");
            check = 0;
        }

    }

    // Free memory again. 
    free(c);
    free(task);
    free(op);
    free(a);
    free(b);
    free(f1);
    free(f2);
    free(yn);

    return 0;
}
