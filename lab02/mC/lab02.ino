/// Lab 02 - Analog Signal Acquisition
// Sample the analog output of the hall sensor sensor
// and print the value to the serial port

int i, in, sensorVal;
int sumSensorVal = 0;
int lim = 20; // Set to 1, 20 or 200 according to the tasks.

void setup() {
  // setup the serial connection
  Serial.begin(115200);
}

void loop() {
  // check if the serial port is available and if something is received from the serial port
  if(Serial.available())  
  {
    // Read the the hall sensor voltage
    Serial.readBytes((char *)&in, 4);
    
    if (in == '1')
    {
      for (i = 0; i < lim; i++) 
      {
        // Read the voltage as a digital value.
        sensorVal = analogRead(A0);
        // Sum digital voltage levels. 
        sumSensorVal += sensorVal;
      }
      sensorVal = sumSensorVal / lim;
      sumSensorVal = 0;
      Serial.print(sensorVal);
    }
  }
  delay(500);
}
