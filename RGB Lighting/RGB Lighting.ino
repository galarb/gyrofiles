/* /////                RGB Lighting Software       ///////////////////////
             RGB Strip software using Arduino


By: Gal Arbel
2022
*/
 
 #include <Adafruit_NeoPixel.h>
 #include <IRremote.h>
 //#define MESSAGE_WAIT_SEC (15 * 60)
 const unsigned int MAX_MESSAGE_LENGTH = 64;
 const int LEDs = 7; // pins on Arduino
 const int REDPin = 2;
 const int GREENPin = 3;
 const int BLUEPin = 12;
 const int IRPin = 4; 
 const int LED_NUM = 55;
 int  colorindex = 0;

 int RED = 0;
 int GREEN = 0;
 int BLUE = 0;
 bool increase = true;
 Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LEDs, NEO_GRB + NEO_KHZ800); // GRB is how this neopixels works
 IRrecv irrecv(IRPin);     
 decode_results results; 

void setup() {
delay(100);
  Serial.begin(115200); 
  pinMode(LEDs,OUTPUT);
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear();
  neopixels(80,0,80);
  pinMode(REDPin,INPUT_PULLUP);
  pinMode(GREENPin,INPUT_PULLUP);
  pinMode(BLUEPin,INPUT_PULLUP);
  irrecv.enableIRIn();     


}


void loop() {
/*
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
		      //Serial.printf("'%s'\n", ptr);
          if (argindex == 0) {
            cmd = ptr[0];
          }
          command[argindex] = atoi(ptr);   
          //Serial.println(command[argindex]);
          argindex++;  
		      ptr = strtok(NULL, delim);
	       } 

      switch (cmd) {
       case 'h': //Set port to HIGH
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],HIGH);
        //Serial.printf("%s %d %s \n", "Pin", command[1],"=HIGH");   
        delay(1000);
        break;
       case '0': // Set port to LOW
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],LOW);
        //Serial.printf("%s %d %s \n", "Pin", command[1],"=LOW");
        delay(1000);
        break;
       
       case 's': // turn on A single Strip led (LEDnumber, R, G, B)
        stripled (command[1], command[2], command[3], command[4]);
        break;
       case 'n': // turn on all strip leds (R, G, B)
        neopixels (command[1], command[2], command[3]);
        break;
       case 'b': //Leds brightness
        strip.setBrightness(command[1]); //(up to 255)
        //Serial.printf("%s, %d \n", "Setting  Brightness to:",command[1]); //
        break;
       case 'c': //clear the strip
        strip.clear();
        Serial.println("Clearing Strip"); //
        break;
            
       
       message_pos = 0;     //Reset for the next message
      }
   }
  while (increase == true) {
    if (digitalRead(REDPin) == 0 and RED < 249) {
    RED = RED + 10;
    Serial.print("RED = ");
    Serial.println(RED);   
    neopixels(RED,GREEN,BLUE);
    }
    if (digitalRead(GREENPin) == 0 and GREEN < 249) {
    GREEN = GREEN + 10;
    Serial.print("GREEN = ");
    Serial.println(GREEN);   
    neopixels(RED,GREEN,BLUE);
    }
    if (digitalRead(BLUEPin) == 0 and BLUE < 249) {
    BLUE = BLUE + 10;
    Serial.print("BLUE = ");
    Serial.println(BLUE);   
    neopixels(RED,GREEN,BLUE);
    }
   if (GREEN > 249 and BLUE > 249 and RED > 249) {
      Serial.println("Changing Direction");
      delay (1000);
      increase = false;
     }
   delay(10);
  }
  while (increase == false) {
   if (digitalRead(REDPin) == 0 and RED > 0) {
    RED = RED - 10;
    Serial.print("RED = ");
    Serial.println(RED);   
    neopixels(RED,GREEN,BLUE);
    }
   if (digitalRead(GREENPin) == 0 and GREEN > 0) {
    GREEN = GREEN - 10;
    Serial.print("GREEN = ");
    Serial.println(GREEN);   
    neopixels(RED,GREEN,BLUE);
    }
    if (digitalRead(BLUEPin) == 0 and BLUE > 0) {
    BLUE = BLUE - 10;
    Serial.print("BLUE = ");
    Serial.println(BLUE);   
    neopixels(RED,GREEN,BLUE);
    }
   if (GREEN < 1 and BLUE < 1 and RED < 1) {
      Serial.println("Changing Direction");
      delay (1000);
      increase = true;
     }
   delay(10);
  }
*/
  //IR Receiption
  if (irrecv.decode(&results)) {
      Serial.print("Code: ");     
      Serial.println(results.value, HEX);
      Serial.println(results.value);

      irrecv.resume(); // Receive the next value
      if((results.value) == 16724175) {
        Serial.print("Key = 1 "); 
        neopixels(255,0,0);
      }
  }
}
void stripled (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      delay(5);
}

void neopixels (int red, int green, int blue) {
  for (int i = 0; i < LED_NUM; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      delay(1);
  }
        strip.show();

}


void bluelightning(int duration){
for (int i=0; i < duration; i++) {
 neopixels(80,127,255);
 delay(12);
 strip.clear(); 
 delay(15);
 }
}


void lightning(int duration){
 for (int i=0; i < duration; i++) {
   
 neopixels(255,255,255);
 delay(20);
 strip.clear(); 
 delay(20);
 }
}


void singlelightning(int duration){
 neopixels(255,255,255);
 delay(20);
 strip.clear(); 
 delay(duration);
}

void neopattern1 (int red1, int green1, int blue1, int red2, int green2, int blue2) {
  for (int i = 0; i < LED_NUM; i++) {
      strip.setPixelColor (i, strip.Color(red1, green1, blue1));
      i++;
      strip.setPixelColor (i, strip.Color(red2, green2, blue2));
      strip.show();
      delay(15);
   }
}

