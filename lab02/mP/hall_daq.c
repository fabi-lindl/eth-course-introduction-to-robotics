#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

// Include header files. 
#include "feather_serial.h"
#include "hall_sensor.h"

int main(int argc, char* argv[])
{
    // Initialize all parameters. 
    int i, j, n, lim, recv, checkRecv, waitInput, fileNum;
    // Sensor values. 
    int const quiescentVoltage = 1.65;
    float static voltage;
    float sumVoltage = 0;
    float mfield;
    int distance = 40; // Init. sensor distance from from the magnet.
    // Char buffer variables for read/write from/to featherboard.
    char buf[4], recvBuf[4];
    char* fname = (char *)malloc(sizeof(char)*200);    
    if (fname == NULL)
    {
      printf("Call to malloc failed.\n");
      exit(1);
    }

    // initialize the serial port on the port /dev/ttyUSB0, with baud rate 115200
    int fd = serialport_init("/dev/ttyUSB0", 115200);
    
    // write to the serial port to get a value
    printf("Type in 1 to read the sensor value:\n");
    scanf("%s", buf);

    // Ask user which file he wants to write to.
    printf("Type in file no.\n");
    scanf("%d", &fileNum);
    if (fileNum == 0) {
      // File for voltage values at the same position. 
      fname = "voltage_data.txt";
    }
    else if (fileNum == 1) {
      // File for averaged (20 samples values).
      fname = "voltage_data_avg_20_samples_per_position.txt";
    }
    else if (fileNum == 2) {
      // Fle for averaged (200 sample values).
      fname = "voltage_data_avg_200_samples_per_position.txt";
    }
    else if (fileNum == 3) {
      fname = "voltage_magnetic_field_strength_distance.txt";
    }
    // Create a file pointer to a new file. 
    FILE *f = fopen(fname, "w");
    // Check file opening. 
    if (f == NULL) {
      printf("Error opening file.\n");
      exit(1);
    }

    // Create 50 measurements. 
    for (i = 0; i < 50; i++)
    {
        // Let the user know if you were able to write to the port
        n = serialport_write(fd, buf);
        if (n == -1)
          printf("Was not able to write.");
        else
        {
          // Read the sensor value from the serial port into the buffer. 
          checkRecv = serialport_read(fd, recvBuf, 4, 3000); // Timer matters (1500 did not work)
          if (checkRecv == 0)
            printf("Reading failed.\n"); // Let the user know if you were able to read from the port
          else
            recv = atoi(recvBuf);

          // Convert the sensor value to a voltage level. 
          voltage = recv * (3.3/4095);
          // Convert the voltage value to magnetic field
          mfield = hall_sensor_get_field(voltage, quiescentVoltage);

          // Write voltage/distance data to the file. 
          fprintf(f, "%f  %f  %d\n", voltage, mfield, distance);

          // Write data to the terminal.
          printf("Digital value = %d\n", recv);
          printf("Voltage = %f V\n", voltage);
          printf("MField = %f mT\n", mfield);
          printf("distance = %d mm\n", distance);
          printf("-------------- %d ---------------\n", i);
          
          // Wait for user input (shift the sensor by a distace of 2mm). 
          while (1) {
            printf("Next measurement, type 1.\n");
            scanf("%d", &waitInput);
            if (waitInput == 1)
              break;
          }
          // Go further away from the sensor at step-sizes of 2 mm. 
          distance -= 2;
          if (distance < 10)
            break;
        }
        
    }
    
    // Close the file.
    fclose(f);
    // Free the memory.
    free(fname);
    // Close the serial port
    serialport_close(fd);

	  return 0;
}
