#include "util.h"

int initBallBalancingRobot(int fd) {
  
  printf("\e[1;1H\e[2J"); // Clear screen
  printf("#######################\n");
  printf("Hardware Initialization\n");
  printf("#######################\n");

  // Let feather reboot
  usleep(200);

  // Check pixy readings
  int pixy_return, flag, x, y;
  pixy_return = readFromPixy(fd, &flag, &x, &y);
  while (!pixy_return) {
    // retry until connection established
    pixy_return = readFromPixy(fd, &flag, &x, &y);
  }
  
  printf("%s, x = %d, y = %d \n", "INIT: Pixy Coordinates received", x, y);

  // Do some inverse kinematics to check for errors
  double position[] = {0, 0, 130};
  double plate_angles[] = {0, 0};
  double servo_angles[] = {0, 0, 0};
  inverseKinematics(plate_angles, servo_angles);
  printf("INIT: Inverse Kinematics initialized\n");
  printf("INIT: Finished\n");
  // printf("INIT: A: %.2f, B: %.2f, C: %.2f
  // \n",servo_angles[0],servo_angles[1],servo_angles[2]);

  tcflush(fd, TCIFLUSH);

  // Send motor commands
  // servoCommand(fd,servo_angles);

  return 1;
}

int inverseKinematics(const double* plate_angles, double* servo_angles) {
    // Load parameters R, L_1, L_2, P_z etc. from parameters file. Example: double R = bbs.R_plate_joint;
    // Then implement inverse kinematics similar to prelab
    
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */

    printf("CHECK STRUCT %f\n", bbs.l1);
    // Custom variables. 
    float phi, theta;                  // Input angles in rad. 
    float dz_a, dz_b, dz_c;            // z-offsets of grippers. 
    float beta_a, beta_b, beta_c;      // Beta servo angles. 
    // Constants. 
    float ll = pow(bbs.l1, 2) - pow(bbs.l2, 2);
    float c = 2 * bbs.l1;
    float cv = M_PI/180.0;
    
    /*--------------------------------------------------------------------------
    
    Transform plate angles from degrees to radians for calculation

    --------------------------------------------------------------------------*/
    phi = plate_angles[0]*cv;
    theta = plate_angles[1]*cv;
    /*--------------------------------------------------------------------------
    
    // Calculate the z-offsets of each anchor point according to the formulas
    
    --------------------------------------------------------------------------*/
    dz_a = bbs.R_plate_joint*sin(phi);
    dz_b = -0.5*bbs.R_plate_joint*sin(phi) + sqrt(3)/2*bbs.R_plate_joint*sin(theta);
    dz_c = -0.5*bbs.R_plate_joint*sin(phi) - sqrt(3)/2*bbs.R_plate_joint*sin(theta);
    /*--------------------------------------------------------------------------
    
    // Calculate the beta-servo angles according to the law of cosines
    
    --------------------------------------------------------------------------*/
    beta_a = acos((pow(bbs.plate_height + dz_a, 2) + ll) / (c * (bbs.plate_height+dz_a)));
    beta_b = acos((pow(bbs.plate_height + dz_b, 2) + ll) / (c * (bbs.plate_height+dz_b)));
    beta_c = acos((pow(bbs.plate_height + dz_c, 2) + ll) / (c * (bbs.plate_height+dz_c)));
    /*-------------------------------------------------------------------------- 
    
    // Return -1 if any of the angles is not defined
    
    --------------------------------------------------------------------------*/ 
    if (isnan(beta_a) || isnan(beta_b) || isnan(beta_c))
      return -1;
    /*-------------------------------------------------------------------------- 
    
    // Calculate the alpha servo angles and transform them back to degrees
    
    --------------------------------------------------------------------------*/
    servo_angles[0] = (M_PI/2-beta_a)/cv;
    servo_angles[1] = (M_PI/2-beta_b)/cv;
    servo_angles[2] = (M_PI/2-beta_c)/cv;

    //return -1; // if invalid input angle
    return 0; // if ok
};


int cameraCalibration(const int x_in, const int y_in, double* x_out, double* y_out){

    // implement camera calibration code
    // make sure to multiply the raw pixy2 coordinates with the scaling factor (ratio between
    // image fed to matlab for calibration and pixy2 resolution): bbs.calibration_image_scale.
    
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */
    double u, v, u_tild, v_tild, r, z_k;

    // Upscaled and normalized coordianates. 
    u_tild = (x_in * bbs.calibration_image_scale - bbs.distortion_center[0]) / bbs.focal_length;
    v_tild = (y_in * bbs.calibration_image_scale - bbs.distortion_center[1]) / bbs.focal_length;

    // Normalized radius.
    r = sqrt(pow(u_tild, 2) + pow(v_tild, 2));

    // Undistorted normalized coordinates.
    u_tild /= 1 + bbs.radial_distortion_coeff[0]*pow(r, 2) + bbs.radial_distortion_coeff[1]*pow(r, 4);
    v_tild /= 1 + bbs.radial_distortion_coeff[0]*pow(r, 2) + bbs.radial_distortion_coeff[1]*pow(r, 4);

    // Rescaling. 
    u = u_tild * bbs.focal_length;
    v = v_tild * bbs.focal_length;

    // Camera frame z coordinate. 
    z_k = abs(bbs.cam_offset[2]) + bbs.plate_height + bbs.ball_radius;

    // Camera position cameras. 
    // Camera coordinate system.
    *x_out = u * z_k / bbs.focal_length;
    *y_out = v * z_k / bbs.focal_length;
    // World coordinate system. 
    *x_out = - *x_out + bbs.cam_offset[0];
    *y_out = - *y_out + bbs.cam_offset[1];

    return 0;
};




/* Sends servo angles to serial port */
int servoCommand(int fd, double* servo_angles) {
  // check serial
  int writeval;

  // assign values
  double angleA = servo_angles[0] + servo.bias_A;
  double angleB = servo_angles[1] + servo.bias_B;
  double angleC = servo_angles[2] + servo.bias_C;

  int min = servo.min_angle;
  int max = servo.max_angle;

  // check if values are valid
  int condition = (angleA < max && angleA > min) &&
                  (angleB < max && angleB > min) &&
                  (angleC < max && angleC > min);

  if (condition != 1) {
    printf("ERROR: Servo angles out of bounds.\n");
    return -1;
  }

  // assemble command
  char command[50];
  sprintf(command, "C %.2f %.2f %.2f\n", angleA, angleB, angleC);

  // Flush serial port output
  tcflush(fd, TCOFLUSH);
  // send command
  writeval = write(fd, command, strlen(command));

  printf("SERVO COMMAND VALID\n");

  return 0;
}

/* Reads pixel coordinates from Pixycam. Also returns a flag whether an object
 * was detected or not */
int readFromPixy(int fd, int* flag, int* x, int* y) {
  char buff[20];
  const char command[] = "P\n";
  int writeval;
  char* token;
  const char delim[] = " ";

  // Flush serial port input
  tcflush(fd, TCIFLUSH);
  tcflush(fd, TCOFLUSH);

  // Write command to pixy
  writeval = serialport_write(fd, command);
  usleep(10 * 1000);

  // Read until until no more bytes available
  // If not data is availabe, retry until success
  int readval = 0;
  while(readval != 1){
    readval = serialport_read_until(fd, buff, sizeof(buff), '\n', 100);
  }
  
  // printf("readFromPixy: after read \n");
  // printf("writeval = %d, readval = %d", writeval,readval);

  // Catch read write errors
  if (!readval) {
    //printf("SERIAl READ FAILED with %d \n",readval);
    return -1;
  }

  // Add terminating 0 to make string
  buff[sizeof(buff) - 1] = 0;

  // extract values using strtok
  token = strtok(buff, delim);

  // Verify initial character
  if (token[0] != 'A') {
    //printf("SERIAL HEADER ERROR: %.20s\n",buff);
    return -1;
  }

  token = strtok(NULL, delim);
  *flag = atoi(token);
  token = strtok(NULL, delim);
  token = strtok(NULL, delim);
  *x = atoi(token);
  token = strtok(NULL, delim);
  token = strtok(NULL, delim);
  *y = atoi(token);

  return 1;
}

