// Make sure these libraries are installed
// If not the case, use Tools > Manage Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


// create a pwm object to control the servo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// declare variables
int servo_num = 0;
int servo_freq = 50;
float poti_value;
float poti_volt;
float pwm_position;
float pwm_length;



#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


void setup() {
  
  // _______________ Begin - Setup _______________

  Serial.begin(115200); 
  // Begin the serial communication
  // Begin PWM communication and set servo frequency
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
  
  // _______________ End - Setup _______________
  
}

void loop() {
  // _______________ Begin - Loop _______________

  // Read values from the analog pin and map/scale them to the movment range of the servo.
  // Optionally display the reading from the potentiometer on the serial monitor
  // Set the servo position according to the mapped/scaled value
  if(Serial.available())
  {
      
      poti_value = analogRead(A0);
      poti_volt = poti_value*(3.3/4095.0);
      Serial.print("Magnitude of the voltage:  ");
      Serial.println(poti_volt);
      
      pwm_position = map(poti_volt, 0, 3.3, 0, 180); //map voltage to pwm position
      pwm_length = map(pwm_position, 0, 180, 550, 2400); // map to pwm length in us
      Serial.print("PWM length is:  ");
      Serial.println(round(pwm_length));
      pwm.writeMicroseconds(0,int(round(pwm_length))); // send pwm pulse to servo number 0
      delay(1000);
  }
  
  // _______________ End - Loop _______________
  
  delay(15); // wait for the servo to get there
}
