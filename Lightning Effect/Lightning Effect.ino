/* /////                Lighning decoration Software       ///////////////////////
             RGB Strip software using Arduino


By: Gal Arbel
2022
*/
 
 #include <Adafruit_NeoPixel.h>

 //#define MESSAGE_WAIT_SEC (15 * 60)
 const unsigned int MAX_MESSAGE_LENGTH = 64;
 const int LEDs = 7; // pin on Arduino
 const int LED_NUM = 15;
 int  colorindex = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LEDs, NEO_GRB + NEO_KHZ800); // GRB is how this neopixels works

void setup() {
delay(100);
  Serial.begin(115200); 
  pinMode(LEDs,OUTPUT);
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear();
  neopixels(80,0,80);

}


void loop() {

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
   delay (60);
   singlelightning(4);
   lightning(5);
   bluelightning(3);
}

void stripled (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      delay(5);
}

void neopixels (int red, int green, int blue) {
  for (int i = 0; i < LED_NUM; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      delay(25);
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

