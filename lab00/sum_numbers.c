// lab00, lap procedure, task 2

// source file for the program sum_numbers

/*
 * This file is used to generate an executable program that prints the 
 * sum of the two numbers in binary and hexadecimal format.
*/

// Since we will be using our own functions, we need to add the header
// file where the functions are declared.
// Keep in mind that the header file already includes standard libraries
// so they do not to be included here.
#include "functions.h"

 // main function
int main(int argc, char *argv[])
{
	uint16_t arg1 = 0xabc, arg2=0x111;
	uint16_t output = arg1+arg2; 
	print_bits(output);

	return 0;
}
