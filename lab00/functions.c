// lab00, lab procedure, tasks 1 & 3

// source file

/* 
 * this file contains the implementation of the functions relevant for
 * lab00.
 * The declarations of the functions are in the corresponding header 
 * file (functions.h).
*/

// the header file needs to be included
#include "functions.h"

/* task 1
 * The function print_bits() accepts an input of type uint16_t 
 * (arg_word) and has no return value (void).
 * The function simply writes the binary and hexadecimal number of the
 * input to the terminal.
*/
void print_bits(uint16_t arg_word)
{
	int i;
	uint16_t intg_part = arg_word;
	uint8_t length_bin = 21; //16 bits + 4 spaces + \0
	char bin[length_bin]; // create string 
	bin[length_bin-1] = '\0'; // end of string

	for(i = length_bin-2; i >= 0; i--)
	{
		if(i%5 == 0)
			bin[i] = ' '; // space every 4 binary digits
		else
		{
			if (intg_part%2 & 1) // remainder value is 1
				bin[i] = '1';
			else 
				bin[i] = '0'; // remainder value is 0
			intg_part = intg_part/2;
		}
	}
	printf("hex= 0x%x, bin: %s \n", arg_word, bin); // write to terminal
}

/* task 3
 * The function bit_merge() accepts two uint16_t as inputs (lsb and msb) 
 * and combines them to a uint32_t number by merging them.
 * The return value is a uint32_t number.
 */
 uint32_t bit_merge(uint16_t lsb, uint16_t msb)
 {
	uint32_t temp = msb << 16;
	return temp|lsb;
 }
