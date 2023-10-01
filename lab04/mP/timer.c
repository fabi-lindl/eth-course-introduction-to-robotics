#include "feather_serial.h"

/* These are system level includes */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

void delay(int number_of_seconds)
{
    int micro_seconds = 1000000 * number_of_seconds;
  
    clock_t start_time = clock();
  
    while (clock() < start_time + micro_seconds);
}


int main()
{   // Intialize serial port and variables
    int fd = serialport_init( "/dev/ttyUSB0", 115200);
    
    int correct_input_value = 0; // "boolean" value that switches to 1 when a suitable input has been received
    int run_time;               // Amount of seconds the timer is supposed to run
    int i;                       // Iteration variable
    char* reset = "a";           // Character to be sent through the serial port to reset the servo to its 0° position.
    char* increase = "b";        // Character to be sent through the serial port to tell the Arduino to advance by 1 second.
    char normal_message[] = "How long do you want the watch the run (enter a number between 1 and 150 seconds) or type 0 to quit \n";
    char* output_message = normal_message; 
    char error_message[] = "Value must be in range [0, 150], please retry. \nType in a number between 1 and 150 seconds or type 0 to quit \n";
    // loop until the user wants to quit the timer program
    while (1) {
        
        
        /*_________________Begin - Input Section_________________*/
        while(1)
        {
            printf("%s", output_message);
            scanf("%d", &run_time);
            if(run_time < 151 && run_time > -1)
            {
                output_message = normal_message;
                break;
            }
            else
                output_message = error_message;

        }
        printf("run time %d \n", run_time);
        if (!run_time)
            break; // quit the program

        /*_________________End - Input Section_________________*/

		
        
        
        /*_________________Begin - Reset Section_________________*/
        
        if (serialport_write(fd, reset)==-1)
        {
            printf("Serial port not available");
            break;
        }
        /*_________________End - Reset Section_________________*/
        
        
        
		delay(1); // Wait a second before starting the timer
        printf("%d seconds will be timed, starting now! \n", run_time);
        
        
        /*_________________Begin - Advance Section_________________*/
        for (i = run_time; i > 0; i--)
        {
          
            printf("seconds elapsed = %d \n", run_time -i+1);
            if (serialport_write(fd, increase)==-1)
            {
                printf("Serial port not available");
            }
            delay(1); 
        }

        /*_________________End - Advance Section_________________*/
    }
    
    // Close the serial port
	serialport_close(fd);
    
	return 0;
    
}
