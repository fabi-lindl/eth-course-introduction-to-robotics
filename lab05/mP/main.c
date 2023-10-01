#include <stdio.h>
#include "util.h"

int main(){
  // Variables. 
  int icheck, servoCheck, camCheck;
  int repeat;
  int x_k, y_k, flag;
  double x_w, y_w;

  //////////////////////////////
  /////// Initialization ///////
  //////////////////////////////

  //Print Welcome Message
  printf("\e[1;1H\e[2J"); // Clear screen
  printf("#####################\n");
  printf("Ball and Plate System\n");
  printf("#####################\n");
  printf("\n");
  printf("Opening serial port...\n");

  // Initialize the serial port
    const char* port= "/dev/ttyUSB0";//vm: "/dev/ttyUSB0", mac: "/dev/cu.SLAB_USBtoUART"
  int fd = serialport_init(port, 115200);
  if (fd == -1){
      printf("Could not open the port.\n");
      printf(" - Is the Arduino IDE terminal opened?\n");
      printf(" - Is the device connected to the VM?\n");
      return -1;    
  }

  // Initialize robot and check
  // if messages are received
  initBallBalancingRobot(fd);

  // Make sure that serial port is relaxed
  usleep(20*1000);

  // Parameter loading functions
  load_parameters();
  load_servo();

  //////////////////////////////
  //////// Task Selection //////
  //////////////////////////////
  int task_selection = 0;
  printf("Select Task: ");
  scanf("%d", &task_selection);

  //////////////////////////////
  /////////// Task 1 ///////////
  //////////////////////////////

  if(task_selection == 1){
    /* Test inverse kinematics via
    terminal */
      
    //initalize variables:
    double plate_angles[] = {0,0};
    double servo_angles[] = {0,0,0};
     
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */


    // User input. 
    while (1) {
      printf("Type in phi_x:\n");
      scanf("%lf", &plate_angles[0]);
      printf("Type in theta_y\n");
      scanf("%lf", &plate_angles[1]);
      
      // Run inverse kinematics. 
      // Compute the servo angles in degrees. 
      icheck = inverseKinematics(plate_angles, servo_angles);
      
      if (icheck == -1)
      {
        printf("Angles incorrect.\n");
        printf("Try again...\n");
      } else {
        printf("Inputs granted.\n");

        // Send angles to the servo motors. 
        servoCheck = servoCommand(fd, servo_angles);
        if (servoCheck == -1) 
          continue;

        // Print Euler angles. 
        printf("phi_x = %f\ntheta_y = %f\n", plate_angles[0], plate_angles[1]);
        printf("-----------------\n");
        // Print servo angles. 
        printf("alpha_a = %f\nalpha_b = %f\nalpha_c = %f\n", servo_angles[0], servo_angles[1], servo_angles[2]);
        
        // Ask user for input to continue with the proces.
        printf("Do you want to continue? Type 1 if so.\n");
        scanf("%d", &repeat);
        if (repeat != 1)
          break;
        

      }
    }

  }

  //////////////////////////////
  /////////// Task 2 ///////////
  //////////////////////////////
  /*Test camera calibration*/
  if(task_selection == 2){
      
    //initalize variables:
      
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */

    while (1)
    {
      // Read data from pixy.
      if (readFromPixy(fd, &flag, &x_k, &y_k) == -1)
        printf("readFromPixy error\n");
      else
      {
        if (!flag)
        {
          printf("No ball detected.\n");
          continue;
        }
        else
          printf("Camera coordinates:\nx_k = %d\ny_k = %d\n", x_k, y_k);
      }
      
      // Compute calibrated camera coordinates in the world coordinate system. 
      if (cameraCalibration(x_k, y_k, &x_w, &y_w) != 0)
        printf("Camera calibration error.\n");
      else
        printf("World coordinates:\nx_w = %lf\ny_w = %lf\n", x_w, y_w);
        printf("Radial position = %lf\n", sqrt(pow(x_w, 2) + pow(y_w, 2)));

      printf("Do you want to obtain new ball coordinates, type in 1?\n");
      scanf("%d", &camCheck);
      if (camCheck != 1)
        break;
    }

  }

  return 0;
}
