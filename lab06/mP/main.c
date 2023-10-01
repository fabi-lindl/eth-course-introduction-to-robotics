#include <stdio.h>
#include "util.h"

int main(){
  //////////////////////////////
  /////// Initialization ///////
  //////////////////////////////

  //Time information
  time_t rawtime;
  struct tm *info;
  char buffer[80];
  time(&rawtime);
  int icheck, servoCheck, camCheck;
  int x_k, y_k, flag;
  double x_w, y_w;
  int repeat;
  info = localtime(&rawtime);
  printf("Formatted date & time : |%s|\n", buffer );

  //Print Welcome Message
  printf("\e[1;1H\e[2J"); // Clear screen
  printf("#####################\n");
  printf("Ball and Plate System\n");
  printf("#####################\n");
  printf("|%s|\n", buffer );
  printf("\n");
  printf("Opening serial port...\n");

  // Initialize the serial port
  const char* port= "/dev/ttyUSB0";
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


  //////////////////////////////
  /////// Task 4/5/6 ///////////
  //////////////////////////////

  if ((task_selection == 4) || (task_selection == 5) || (task_selection == 6)) {

    //Initialize PID parameters
    double factor = 8.0;
    double k_p = 0.7/factor;
    double k_i = 0.3/factor;
    double k_d = 0.3/factor;
    
     //PID default parameters
    int default_pid= 0;
    //Intialize filter window sizes
    int n_pos = 5;
    double n_vel = 15;
    double v_new_x = 0;
    double v_new_y = 0;
    double x_new;
    double y_new;
    // TODO: Ask for user input to change PID parameters
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */

    printf("Do you want to use default parameters ? Type 1 for yes, 0 for no\n");
    printf("Default is k_p = %lf, k_i = %lf, k_d = %lf \n", k_p, k_i, k_d);
    scanf("%d", &default_pid);
    if(!default_pid){
      printf("Enter the Kp gain of the PID \n"); 
      scanf("%lf", &k_p);
      printf("Enter the Ki gain of the PID \n"); 
      scanf("%lf", &k_i);
      printf("Enter the Kd gain of the PID \n"); 
      scanf("%lf", &k_d);
    }

    // Variables for Pixy2
    int flag = 0;
    int x_px = 0;
    int y_px = 0;
    double x_cal = 0;
    double y_cal = 0;

    //read pixy a couple of times to clear buffer
    for(int i = 0; i < 20; i++){
      readFromPixy(fd, &flag, &x_px, &y_px);
    }
    
    // create buffer arrays for filtered variables
    // [0] is always the current element
    // make sure buf_size is bigger than filter windows
    int buf_size = 50;
    double x_raw[buf_size];  // calibrated, unfiltered
    double y_raw[buf_size];
    double vx_raw[buf_size];  // 1st order derivative, unfiltered
    double vy_raw[buf_size];
    double x[buf_size];  // filtered
    double y[buf_size];
    double vx[buf_size];  // filtered
    double vy[buf_size];

    // initialize buffer arrays to zero
    for (int i = 0; i < buf_size; i++) {
      x_raw[i] = 0;
      y_raw[i] = 0;
      vx_raw[i] = 0;
      vy_raw[i] = 0;
      x[i] = 0;
      y[i] = 0;
      vx[i] = 0;
      vy[i] = 0;
    }

    // initialize angles
    double servo_angles[] = {0,0,0};
    double plate_angles[] = {0,0};

    // reference variables for control
    // are being set in reference functions
    double x_ref, y_ref, vx_ref, vy_ref;

    // pid variables
    double x_integ = 0; 
    double y_integ = 0;
    double u_x = 0;
    double u_y = 0;
    double error_x = 0;
    double error_y = 0;
    double prev_error_x = 0;
    double prev_error_y = 0;

    //Logfile with datetime as filename
    char datetime[80];
    strftime(datetime,80,"%Y-%m-%d_%H-%M-%S_pid_log.txt", info);
    FILE *fp = fopen(datetime, "w+");
    startLogging(fp, task_selection, k_p, k_d, k_i, n_pos, n_vel);

    //Timing variables
    long start = 0;
    long end = 16;
    long t0 = getMicroseconds();
    double dt = 0.016; //variables for timing
    double current_time = 0;
    //double previous_time = t0;
   // double elapsed_time = 0.0;

  
    while(1){
  
      //TODO: Get current sampling time dt
      dt = (end- start)/pow(10, 6);
      start = end;
      end = getMicroseconds() - t0;
      current_time += dt;
      //current_time = getMicroseconds();
      //dt = (current_time - previous_time)/pow(10, 6);
      //previous_time = current_time;
      //elapsed_time += dt;
      
      //TODO: Pixy Measurement
      // Read data from pixy.
      if (readFromPixy(fd, &flag, &x_px, &y_px) == -1)
        printf("readFromPixy error\n");
      else
      {
        if (!flag)
        {
          printf("No ball detected.\n");
          continue;
        }
        else
          printf("Camera coordinates:\nx_k = %d\ny_k = %d\n", x_px, y_px);
      }
      
      
      //If anything is detected, enter loop
      if(flag){
        //TODO: Use camera calibration
        // Compute calibrated camera coordinates in the world coordinate system. 
        if (cameraCalibration(x_px, y_px, &x_cal, &y_cal) != 0)
          printf("Camera calibration error.\n");
        else{
          printf("World coordinates:\nx_w = %lf\ny_w = %lf\n", x_cal, y_cal);
          printf("Radial position = %lf\n", sqrt(pow(x_cal, 2) + pow(y_cal, 2)));
          //TODO: Place measurements in buffer
          //Hint: There is a function called pushBack
          // in util.h that you can use here.
          pushBack(x_cal, x_raw, buf_size);
          pushBack(y_cal, y_raw, buf_size);
          

          //TODO: Compute velocity
          v_new_x = discreteDerivative(dt,x_raw);
          v_new_y = discreteDerivative(dt,y_raw);
          pushBack(v_new_x, vx_raw, buf_size);
          pushBack(v_new_y, vy_raw, buf_size);

          //TODO: Apply filter
          x_new = movingAverage(n_pos, x_raw);
          //x_new = butterWorth(x_raw, x);
          y_new = movingAverage(n_pos, y_raw);
          //y_new = butterWorth(y_raw, x);
          pushBack(x_new, x, buf_size);
          pushBack(y_new, y, buf_size);

          v_new_x = movingAverage(n_vel,vx_raw);
          //v_new_x  = butterWorth(vx_raw, vx);
          v_new_y = movingAverage(n_vel,vy_raw);
          //v_new_y  = butterWorth(vy_raw, vy);
          pushBack(v_new_x,vx, buf_size);
          pushBack(v_new_y,vy, buf_size);


          //TODO: Set reference depending
          switch(task_selection){
            case 4: /*TODO: Postlab Q4 centering task */ 
            // setpoints
              x_ref = 0;
              y_ref = 0;
              vx_ref = 0;
              vy_ref = 0;
              break;
            case 5:
             /*TODO: Postlab Q5 step response reference  --> use function in util.h */
            stepResponse(current_time, &x_ref, &y_ref, &vx_ref, &vy_ref);
             break;
            case 6: /*TODO: Postlab Q6 circular trajectory reference --> implement & use function in util.h */
            circularTrajectory(current_time, &x_ref, &y_ref, &vx_ref, &vy_ref);
             break;
          }

          // compute error
          error_x = x_ref - x[0];
          error_y = y_ref - y[0];

          //Hint: Wait 0.5s before starting to update integrator
          //TODO: Update Integrator
          if (current_time > 0.5){
          x_integ += dt*error_x;
          y_integ += dt*error_y;
          }

          //TODO: Compute PID
          u_x = k_p*error_x + k_i* x_integ + k_d * (vx_ref- vx[0]);
          u_y = k_p*error_y + k_i* y_integ + k_d * (vy_ref- vy[0]);

          //TODO: Compute servo angles and send command
          /*
          printf("ux is %lf \n", u_x);
          printf("uy is %lf\n", u_y);
          printf("vx is %lf \n", vx[0]);
          printf("vy is %lf \n", vy[0]);
          printf("x is %lf \n", x_raw[0]);
          printf("y is %lf \n", y_raw[0]);*/
          
          //controller command
          plate_angles[0] = -u_y;
          plate_angles[1]= u_x;
          inverseKinematics(plate_angles, servo_angles);
          servoCheck = servoCommand(fd, servo_angles);

          if (servoCheck == -1) 
            continue;
          //Open logging file and log everything to textfile
          fp = fopen(datetime, "a");
          logger(fp, end, current_time, dt, k_p, k_d, k_i, x_ref, y_ref, vx_ref,
                 vy_ref, x_raw[0], y_raw[0], x[0], y[0], vx_raw[0], vy_raw[0],
                 vx[0], vy[0], plate_angles, servo_angles, x_integ, y_integ);
        }
        //If everything works until here, you deserve a cookie
      }
    }
  }

  return 0;
}
