// Make sure these libraries are installed
// If not the case, use Tools > Manage Libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// declaring variables
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver();

int servo_num = 0;
int servo_freq = 50;
int MinPulse = 550, MaxPulse = 2400;  // minimum (0°) & maximum (180°) pulse length of the servo in µs 
int pos = MaxPulse;  // current position of the servo as integer,            
float w_e = 180.0/150.0;          // angular velocity of servo motor at 5 V
float secondStep = (w_e/180.) * (MaxPulse - MinPulse);  // unrounded pulse length increase in µs that corresponds to an increase of 1 second
char serialVariable;          // Character received through the serial communication
int pw_add; // pulse width to add in us as integer
int counter = 1;


void setup() {
  Serial.begin(115200);                 // open serial communication

  //_________________Begin - Setup_______________
  servo.begin();
  servo.setPWMFreq(servo_freq);  // Analog servos run at ~50 Hz updates
  delay(10); // delay to setup
  //_________________End - Setup_______________

}

void loop() {
  
  // Only run the loop if the serial communication is available
  if( Serial.available())
  {
    
    // read the incoming character and save it in "serialVariable"
    serialVariable = Serial.read();

    // If "a" is received reset the servo to its starting position.
    
    //_________________Begin - Reset_______________
    if (serialVariable == 'a')
    {
      servo.writeMicroseconds(servo_num,MaxPulse); // reset servo to initial position
      pos = MaxPulse;  
      
    }
 
    //_________________End - Reset_______________


    // If "b" is received move the motor by one step.
    
    //_________________Begin - Advance_______________
    else if (serialVariable == 'b')
    {
      if (counter % 3 == 0)
      {
        pw_add = int(secondStep+1);  //round up every third iteration
        counter = 0; // reinitialize counter to zero
      }
      else
        pw_add = int(secondStep); // cast to integer (round down)

      pos -= pw_add; // substract pulse length corresponding to 1 second to current position (pulse length)
      servo.writeMicroseconds(servo_num, pos); // update servo position (increase by 1 second)
      counter++;
    }
    //_________________End - Advance_______________
 
  }
}
