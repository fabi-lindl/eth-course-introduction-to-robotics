#include <stdio.h>
#include <stdint.h>
#include "feather_serial.h"


// ***** print_bits() can be implemented here ***** //
void print_bits(uint8_t arg_word)
{
	int i;
	uint8_t intg_part = arg_word;
	uint8_t length_bin = 11; //8 bits + 2 spaces + \0
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
		
		
// *********************************************** //

int32_t main()
{
  int n;
  // initialization of serial communication, port and baud rate are specified
  int fd = serialport_init( "/dev/ttyUSB0", 115200);
  uint8_t arg = (uint8_t)245;
  int inp1, inp2;
  uint8_t n1, n2, sum;

  while(1)
  {
	// ***** your code goes here ***** //
	// User input values. 
	printf("Enter the first number:\n");
	scanf("%d", &inp1);
    printf("Enter the second number:\n");
	scanf("%d", &inp2);
	// Convert the input numbers.
    n1 = (uint8_t)inp1;
    n2 = (uint8_t)inp2;

	// Specific number combination to stop the input program. 
    if (n1 == 0 && n2 == 0)
      break;

    sum = n1+n2;

	// ******************************* //
	
	// send arg via serial communication to the mC
	// type casting is again needed to match type
	n = serialport_writebyte(fd, ((char*) &sum));
	if(n == -1)
		printf("Unable to write the port \n");
  }
  
  // close serial communication
  serialport_close(fd);
  return 0;
}

