/*/////               PWM Extender       ///////////////////////
PWM extension software using ESP32 and PCA9685
  Using I2C Bus
  Connect RGB LED 1 on ports 1,2,3
          RGB LED2 on ports 4,5,6
  Call from CLI:
    look at 'case' options
  By: Gal Arbel
  2021
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

// Creat object to represent PCA9685 at default I2C address  (0x40, more extensions up to 0x60)
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

// Define maximum and minimum number of "ticks" for the servo motors
// Range from 0 to 4095
// This determines the pulse width

#define SERVOMIN  80  // Minimum value
#define SERVOMAX  800  // Maximum value

// Define servo motor connections (expand as required)
#define SER0  0   //Servo Motor 0 on connector 0
#define SER1  12  //Servo Motor 1 on connector 12

const unsigned int MAX_MESSAGE_LENGTH = 64; 

void setup() {
  pca9685.begin();
  pca9685.setPWMFreq(100);   // Set PWM Frequency(24-1526Hz). Leds were working fine on: 800, 500, 50
  delay (100);
  Serial.begin(115200);
  Serial.println("PCA9685 Servo started");

}

void loop() {

  while (Serial.available() > 0) { 
   char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;
   char inByte = Serial.read();   //Read the next available byte in the serial receive buffer
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
     message[message_pos] = inByte;  //Add the incoming byte to our message
     message_pos++;
     }
     //Full message received...
     else
     {
       message[message_pos] = '\0';     //Add null character to string
       Serial.println(message);     //echo the message to terminal
       int command[4];
       int argindex = 0;
       char cmd;
       char delim[] = " ";
	     char tmpmsg[MAX_MESSAGE_LENGTH];
       strcpy(tmpmsg,message);
       message_pos = 0;
       message[message_pos] = '\0';     //Add null character to string

        char *ptr = strtok(tmpmsg, delim);
	      while(ptr != NULL)
	       {
		      Serial.printf("'%s'\n", ptr);
          if (argindex == 0) {
            cmd = ptr[0];
          }
          command[argindex] = atoi(ptr);   
          //Serial.println(command[argindex]);
          argindex++;  
		      ptr = strtok(NULL, delim);
	       } 

      switch (cmd) {
       case 'h': 
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],HIGH);
        Serial.printf("%s %d %s \n", "Pin", command[1],"=HIGH");
        delay(1000);
        break;
       case 'l': 
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],LOW);
        Serial.printf("%s %d %s \n", "Pin", command[1],"=LOW");
        delay(1000);
        break;
       case 'S': //[servo number, angle]
        servmov (command[1],command[2]); break;
       
       case 'R': //[ledRGB number, R, G, B]
          lightrgb(command[1], command[2], command[3], command[4]); 
          break;
      
       case 'i': // write to I2C port
          i2c (command[1], command[2]);
          break;

     message_pos = 0;     //Reset for the next message
       
      }
  }
   delay (30);
 } 
 
}

void servmov(int motornum, int position) {  // Move servo to an angle
     Serial.printf("%s%d %s%d \n", "Motor pin =",motornum, "to position =",position);
     position = map(position, 0, 180, SERVOMIN, SERVOMAX);
    pca9685.setPWM(motornum, 0, position);    // Write to PCA9685
    delay(30);
}

byte lightrgb(int lednum, int red, int green, int blue) {  // light RGB led
     Serial.printf("%s%d %s%d %s%d %s%d \n", "RGB LED",lednum, "RED=", red, "GREEN=", green, "BLUE=", blue);
 
     // my leds are COMMON_ANODE, hence 255 -
     red = map(255 - red, 0, 255, 0, 4095); // max 4096 'clicks' in a cycle
     green = map(255 - green, 0, 255, 0, 4095);
     blue = map(255 - blue, 0, 255, 0, 4095);
     pca9685.setPWM(lednum++, 0, red);    // Write to PCA9685. 
     pca9685.setPWM(lednum++, 0, green);    // 0=no clicks for Low. stay at High for "red" clicks. then back to Low
     pca9685.setPWM(lednum++, 0, blue);    // Write to PCA9685
    delay(30);
}

byte i2c (int port, int value) { //writes a value to the i2c port
     value = map(value, 0, 255, 0, 4095);
     pca9685.setPWM(port, 0, value);    // Write to PCA9685. 
}
