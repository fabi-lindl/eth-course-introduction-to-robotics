#include "util.h"

int inverseKinematics(const double* plate_angles, double* servo_angles) {
  //TODO: copy + paste your previous work
    
   // Load parameters R, L_1, L_2, P_z etc. from parameters file. Example: double R = bbs.R_plate_joint;
    // Then implement inverse kinematics similar to prelab

    // Custom variables. 
    float phi, theta;                  // Input angles in rad. 
    float dz_a, dz_b, dz_c;            // z-offsets of grippers. 
    float beta_a, beta_b, beta_c;      // Beta servo angles. 
    // Constants. 
    float ll = pow(bbs.l1, 2) - pow(bbs.l2, 2);
    float L1 = 2 * bbs.l1;
    float deg2rad = M_PI/180.0;
    float R = bbs.R_plate_joint;
    float height = bbs.plate_height;
    
    /*--------------------------------------------------------------------------
    
    Transform plate angles from degrees to radians for calculation

    --------------------------------------------------------------------------*/
    phi = plate_angles[0]*deg2rad;
    theta = plate_angles[1]*deg2rad;
    /*--------------------------------------------------------------------------
    
    // Calculate the z-offsets of each anchor point according to the formulas
    
    --------------------------------------------------------------------------*/
    dz_a = R*sin(phi);
    dz_b = -0.5*R*sin(phi) + sqrt(3)*0.5*R*sin(theta);
    dz_c = -0.5*R*sin(phi) - sqrt(3)*0.5*R*sin(theta);
    /*--------------------------------------------------------------------------
    
    // Calculate the beta-servo angles according to the law of cosines
    
    --------------------------------------------------------------------------*/
    beta_a = acos((pow(height + dz_a, 2) + ll) / (L1 * (height+dz_a)));
    beta_b = acos((pow(height + dz_b, 2) + ll) / (L1 * (height+dz_b)));
    beta_c = acos((pow(height + dz_c, 2) + ll) / (L1 * (height+dz_c)));
    /*-------------------------------------------------------------------------- 
    
    // Return -1 if any of the angles is not defined
    
    --------------------------------------------------------------------------*/ 
    if (isnan(beta_a) || isnan(beta_b) || isnan(beta_c))
      return -1;
    /*-------------------------------------------------------------------------- 
    
    // Calculate the alpha servo angles and transform them back to degrees
    
    --------------------------------------------------------------------------*/
    servo_angles[0] = (M_PI*0.5-beta_a)/deg2rad;
    servo_angles[1] = (M_PI*0.5-beta_b)/deg2rad;
    servo_angles[2] = (M_PI*0.5-beta_c)/deg2rad;

    return 0; // if ok
}

int cameraCalibration(const int x_in, const int y_in, double* x_out, double* y_out){
   //TODO: copy + paste your previous work
    
// implement camera calibration code
    // make sure to multiply the raw pixy2 coordinates with the scaling factor (ratio between
    // image fed to matlab for calibration and pixy2 resolution): bbs.calibration_image_scale.
    
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

double discreteDerivative(const double dt, const double * x){

  return (x[0]-x[1])/dt;
};

double movingAverage(const int n, const double * x){
  //TODO: Implement a moving average function
  double sum = 0;
  for(int i = 0; i < n; i++){
  	sum += x[i];
  }
  return sum/n; 
};

double butterWorth(const double * x, const double * y){
  //TODO: Implement this if you like bonus points (not required to reach max points)
    /* ********************* */
    /* Insert your Code here */
    /* ********************* */
  double a[] = {1, -1.6475, 0.7009};
  double b[] = {0.00134, 0.0267, 0.0134};
  double temp = -a[1]*y[1]-a[2]*y[2]+ b[0]*x[0] + b[1]*x[1] + b[2]*x[2];
  
  //return -(a[0]*y[1] + a[1]*y[2] + a[2]*y[3]+a[3]*y[4])+ b[0]*x[0] + b[1]*x[1]+b[2]*x[2] + b[3]*x[3];
  return temp/a[0];
};

int stepResponse(const double current_time, double* x_ref, double* y_ref,
                 double* vx_ref, double* vy_ref){
  
  double step_start = 5;
  double step_distance = 80;
  if (current_time < step_start){
    *x_ref = 0;
  }else{
    *x_ref = step_distance;
  }

  *y_ref = 0;
  *vx_ref = 0;
  *vy_ref = 0;

  return 0;
};

int circularTrajectory(const double current_time, double* x_ref, double* y_ref,
                 double* vx_ref, double* vy_ref){
  
  double traj_start = 3;
  double num_of_traj = 5;
  double period = 4; //seconds
  double R = 80; //radius
  double w = 2*M_PI/period;
  //TODO: Implement the circular trajectory function.
    // Hint: Use the equations for parametrizing a cirlce (and its derivative)
  *x_ref = R*cos(w*current_time);
  *y_ref = R*sin(w*current_time);

  *vx_ref = -R*sin(w*current_time)*w;
  *vy_ref = R*cos(w*current_time)*w;

  return 0;
};


// Don't change any of the below functions.

int initBallBalancingRobot(int fd) {
    // don't change this function.
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
    
    // Do some inverse kinematics
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

long getMicroseconds() {
  struct timeval the_time;
  long microseconds;
  gettimeofday(&the_time, NULL);
  microseconds = the_time.tv_sec * 1000000 + the_time.tv_usec / 1000 + the_time.tv_usec;
  return microseconds;
}

int pushBack(const double x_new, double* x_array, int array_size) {
  for (int i = array_size - 1; i > 0; i = i - 1) {
    x_array[i] = x_array[i - 1];
  }

  x_array[0] = x_new;
  return 1;
};

int startLogging(FILE* fp, int task_selection, double k_p, double k_d,
                 double k_i, int n_pos, int n_vel) {
  fprintf(fp, "#Task Selector: %d\n", task_selection);
  fprintf(fp, "#PID Parameters: kp: %.2f, kd: %.2f, ki: %.2f\n", k_p, k_d, k_i);
  fprintf(fp, "#n_pos: %d\n", n_pos);
  fprintf(fp, "#n_vel: %d\n", n_vel);
  fprintf(fp,
          "t x_ref y_ref vx_ref vy_ref x_raw y_raw x y vx_raw vy_raw vx vy\n");
  fclose(fp);

  return 0;
}

int logger(FILE * fp,
            long end_time,
            double current_time,
            double dt,
            double k_p,
            double k_d,
            double k_i,
            double x_ref,
            double y_ref,
            double vx_ref,
            double vy_ref,
            double x_raw,
            double y_raw,
            double x,
            double y,
            double vx_raw,
            double vy_raw,
            double vx,
            double vy,
            double * plate_angles,
            double * servo_angles,
            double x_integrator,
            double y_integrator
            ){

  printf("\e[1;1H\e[2J");
  printf("##################################\n");
  printf("############# PID LOOP ###########\n");
  printf("##################################\n");
  printf("Timing\n");
  printf("Elapsed Time: %.2f s\n", current_time);
  printf("Step Time: %.4f s\n", dt);
  printf("Frequency: %.1f Hz\n\n", 1/dt);
  printf("PID Parameters\n");
  printf("k_p: %.2f, k_d: %.2f, k_i: %.2f\n\n", k_p, k_d, k_i);
  printf("ReferencePosition\n");
  printf("x_ref: %.f \t y_ref: %.f\n\n", x_ref, y_ref);
  printf("Position\n");
  printf("x: %.f \t y: %.f\n\n", x,y);
  printf("Reference Velocity\n");
  printf("vx_ref: %.f \t vy_ref: %.f\n\n", vx_ref, vy_ref);
  printf("Velocity\n");
  printf("vx: %.f \t vy: %.f\n\n", vx,vy);
  printf("Plate Angles\n");
  printf("pitch: %.f deg, roll: %.f deg \n\n", plate_angles[1], plate_angles[0]);
  printf("Servo Angles\n");
  printf("A: %.f deg, B: %.f deg, C: %.f deg\n\n", servo_angles[0], servo_angles[1], servo_angles[2]);
  printf("Integrators\n");
  printf("x_integ: %.2f, y_integ: %.2f\n", x_integrator, y_integrator);

  fprintf(
      fp,
      "%lu %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
      end_time, x_ref, y_ref, vx_ref, vy_ref, x_raw, y_raw, x, y, vx_raw,
      vy_raw, vx, vy);
  fclose(fp);

  return 0;
};
