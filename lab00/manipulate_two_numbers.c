// lab00, lab procesure, task 4

// source file for the program manipulate_two_numbers

/*
 * This file is used to generate an executable program that reads two
 * numbers from the terminal and output the merged result in 
 * hexadecimal, and the sum in hexadecimal and binary format.
*/

// again, the header of our function library is already included 
#include "functions.h"

// main
int main(int argc, char *argv[])
{
	uint16_t  number_1, number_2;
	
	printf("Please enter two numbers in hexadecimal, lsb first \n");
	if(scanf("%hx", &number_1) && scanf("%hx", &number_2)) // format given is correct for the two numbers
	{
		printf("merging 0x%x and 0x%x results in 0x%x \n", number_1, number_2, bit_merge(number_1, number_2));
		printf("The sum is ");
		print_bits(number_1 + number_2);
	}
	else // error handling
		printf("Error: make sure to respect the format given for the two numbers\n");

	return 0;
}
