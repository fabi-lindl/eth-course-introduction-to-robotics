// declaring variables
int dl, mlt, incoming, dl_c, mlt_c;
// Variable to check the state of the current switch position. 
// check == 1: blinking sequence, check == 0: number addition
int check = 1;

// define an array to access the LED Pins
int led_pins[10] = {12, 27, 33, 15, 32, 14, 22, 23};

void setup() {
   
   // setting some initial values
   dl = 10;
   mlt = 50;
   incoming = 0;

  // =================== Q1 ===================
   // Set up the pins, i.e. define I/O
   // Output
   pinMode(23,  OUTPUT);
   pinMode(22,  OUTPUT);
   pinMode(14,  OUTPUT);
   pinMode(32,  OUTPUT);
   pinMode(15,  OUTPUT);
   pinMode(33,  OUTPUT);
   pinMode(27,  OUTPUT);
   pinMode(12,  OUTPUT);
   // Input
   pinMode(21, INPUT); // Switch
   // =================== Q1 ===================   


   //_________________Begin - Part A_______________
   Serial.begin(115200);
   //_________________End - Part A_________________
}

void loop() {
   // in C, the for-loop variables need to be initialized before the loop
   // for(int i = 0; i < 9; i++) is not accepted
   int i=0, j=0, vbit=0;
   
   //_________________Begin - Part B_______________
   if( Serial.available() )
   {
     // read into variable incoming
     Serial.readBytes(((char*)&incoming),1);
     Serial.println(incoming);
     // Store the least sign. four bits of incoming in dl. 
     dl = incoming >> 4;
     // Store the most sign. four bits of incoming in mlt. 
     mlt = incoming % (1<<4);
   }
   //_________________End - Part B_________________
   
   
   //_________________Begin - Part C_______________
   // loop over all output (LED) pins and set state to HIGH/LOW
   // e.g. digitalWrite(1,LOW) means you set the digital pin 1 to LOW.
   
    if (digitalRead(21) == 1)
    {
      // Blinking sequence. 
      check = 1;
      dl = 10;
      mlt = 50;
      for (j = 0; j < 8; j++)
      {
        for (i=0;i<8;i++)
          digitalWrite(led_pins[i],LOW);
        digitalWrite(led_pins[j],HIGH);
        delay(dl*mlt);
      }   
      delay(dl*mlt*3);
      //_________________End - Part C_________________
    }
    else
    {
      //_________________Begin - Part D_______________
      // 2 number addition. 
      
      // Check if the last loop iteration dealt with the blinking 
      // sequence. If yes, set dl and mlt to zero so that the nothing
      // is displayed when the switch goes from the blinking sequence
      // position to the addition. 
      if (check == 1) 
      {
        check = 0;
        dl = 0;
        mlt = 0;
      }
      // Set output pins according to the bits of variable incoming. 
      for (i=0; i < 8; i++) 
      {
        if (i < 4) {
          vbit = mlt & 1;
          if (vbit)
            digitalWrite(led_pins[i], HIGH);
          else
            digitalWrite(led_pins[i], LOW);
          mlt = mlt>>1;
        } else {
          vbit = dl & 1;
          if (vbit)
            digitalWrite(led_pins[i], HIGH);
          else
            digitalWrite(led_pins[i], LOW);
          dl = dl>>1;
        }
      }
    // Set a high delay to witness the result of the computation.     
    delay(3000);
    }
    //_________________End - Part D_________________
   
 }

 
