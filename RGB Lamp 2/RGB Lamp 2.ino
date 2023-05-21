/* /////                RGB Lamp Software       ///////////////////////
             RGB Lamp software using ESP32-D1 and Adafruit
  With Local Touch Button and potentiometer to control Brightness and Color       
  Featuring Dual Core task assignement

By: Gal Arbel
2022
*/
TaskHandle_t Task1;
TaskHandle_t Task2;
 #include "config.h"
 #include <WiFi.h>
 #include "WiFiClientSecure.h"
 #include <Adafruit_NeoPixel.h>
 #include "AdafruitIO_WiFi.h"
 #include "BluetoothSerial.h"

 #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
 #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
 #endif

 //#define ssid "arbs"// "iPhone 8 gal"//           
 //#define pass "lafamilia"// "arduino1" 
 #define ssid "arbs"   
 #define pass "lafamilia"

 #define AIO_USERNAME  "noamthegreat"            //personal info from adafruit.io
 #define AIO_KEY       "aio_KVys93BcLI48X7HPUcynO7VmuCT7"
 #define AIO_SERVER      "io.adafruit.com"
 #define AIO_SERVERPORT  8883              // Using port 8883 for MQTTS
 
 //#define MESSAGE_WAIT_SEC (15 * 60)
 const unsigned int MAX_MESSAGE_LENGTH = 64;
 const int LEDs = 26; // pin on esp32
 const int Local = 33; // pin on esp32
 const int LED_NUM = 25;
 const int PB = 4; // Touch Button
 int  colorindex = 0;
BluetoothSerial SerialBT; // Define the BT to Serial Component
WiFiClientSecure client;  // WiFi ClientSecure for SSL/TLS support
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LEDs, NEO_GRB + NEO_KHZ800); // GRB is how this neopixels works

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, ssid, pass);

// io.adafruit.com root CA
 const char* adafruitio_root_ca = \   
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
    "-----END CERTIFICATE-----\n";

 AdafruitIO_Feed *neo = io.feed("Neopixel Leds");
 AdafruitIO_Feed *bri = io.feed("Brightness");
 
void setup() {
  delay(100);
  Serial.begin(115200);       
  SerialBT.begin("ESP32test"); //Bluetooth device name  
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(LEDs,OUTPUT);
  pinMode(Local,INPUT);
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear();
  neopixels(80,0,80);
 
  WiFi.begin(ssid, pass);
   Serial.println("Connecting");
   if(WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print("NOT CONNECTED");
    delay(250);
   }
   Serial.println("");
   Serial.print("Connected to WiFi network with IP Address: ");
   Serial.println(WiFi.localIP());
  
  client.setCACert(adafruitio_root_ca);  // Set Adafruit IO's root CA
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  // set up a message handler for the 'color' feed.
  // the handleMessage function (defined below) will be called whenever a message is received from adafruit io. 
  neo -> onMessage(handleNeoMessage);
  neo -> get();
  bri -> onMessage(handleBrightnessMessage);
  bri -> get();
  Serial.printf("%s, %d \n", "setup() running on core =", xPortGetCoreID()); //show the core 

 //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    loop_0,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    loop_1,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
   

}

void loop_0 ( void * pvParameters ){
  Serial.print("loop_0 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){      //this is an infinite loop on Core 0
  
  if(WiFi.status() != WL_CONNECTED) {
    delay(250);
    WiFi.begin(ssid, pass);
    io.connect();
    delay(250);
   }

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
 // CLI - Messages from Terminal
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
       case 'h': //Set port to HIGH
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],HIGH);
        Serial.printf("%s %d %s \n", "Pin", command[1],"=HIGH");   
        delay(1000);
        break;
       case '0': // Set port to LOW
        pinMode(command[1],OUTPUT);
        digitalWrite(command[1],LOW);
        Serial.printf("%s %d %s \n", "Pin", command[1],"=LOW");
        delay(1000);
        break;
       
       case 's': // turn on A single Strip led (LEDnumber, R, G, B)
        stripled (command[1], command[2], command[3], command[4]);
        break;
       case 'n': // turn on all strip leds (R, G, B)
        neopixels (command[1], command[2], command[3]);
        break;
       case 'p1': // turn on pattern1
        neopattern1 (command[1], command[2], command[3], command[4], command[5], command[6]);
        break;
       case 'p2': // turn on pattern1
        neopattern2 (command[1], command[2], command[3], command[4], command[5], command[6]);
        break;
       case 'p3': // turn on pattern1
        neopattern2 (command[1], command[2], command[3], command[4], command[5], command[6]);
        break;
       case 'b': //Leds brightness
        strip.setBrightness(command[1]); //(up to 255)
        break;
       case 'c': //clear the strip
        strip.clear();
        Serial.println("Clearing Strip"); //
        break;

       message_pos = 0;     //Reset for the next message
      }
   }
   delay (60);
   

 } 
  
  delay(100);
  } 
}

void loop_1 ( void * pvParameters ){
  Serial.print("loop_1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){      //this is an infinite loop on Core 1
    //Local Brightness Reading
    int brightness = analogRead(Local);
    if (brightness < (analogRead(Local) -2) or brightness > (analogRead(Local) +2)) {
     Serial.printf("%s, %d \n", "analog reading Brigthness vaiable =", brightness);
     Serial.printf("%s, %d \n", "analog reading =", analogRead(Local));      
     brightness = map(analogRead(Local), 0, 4095, 0, 255);
     strip.setBrightness(brightness); //(up to 255)
     strip.show();
    
     delay(2000);
     }
  
  /*
  int hr = hallRead();
   // if (reading < (analogRead(Local) - 9) or reading > (analogRead(Local) + 9) ) {
  // reading = map(reading, 0, 4095, 0, 255); 
 //  strip.setBrightness(reading); //(up to 255)
  // strip.show();
   Serial.printf("%s, %d \n", "Hall Effect Value =", hr);
  //}
  */

  int touch = touchRead(PB);
  //Serial.printf("%s, %d \n", "Touch Button Value =", touch);
  delay(500);
  if (touchRead(PB) < 9) { //measure value normally 62
     if (colorindex == 0) {
       Serial.println("Color Cycle Restarted "); 
       neopixels(255,255,255); 
       Serial.println("Status = WHITE");
       }
     if (colorindex == 1) {neopixels(0,255,0); Serial.println("Status = Green");}
     if (colorindex == 2) {neopixels(0,0,255); Serial.println("Status = Blue");}
     if (colorindex == 3) {neopixels(255,0,0); Serial.println("Status = RED");}
     if (colorindex == 4) {neopattern1(255,255,255,230,248,0); Serial.println("Neopattern1");}
     if (colorindex == 5) {neopattern2(255,0,0,0,255,255); Serial.println("Neopattern2");}
     if (colorindex == 6) {neopattern3(0,255,0,0,0,255); Serial.println("Neopattern3");}
     if (colorindex == 7) {neopattern3(0,0,0,0,0,0); Serial.println("Off");}

     colorindex++;
     Serial.printf("%s, %d \n", "colorindex =", colorindex);
     if (colorindex > 7) {colorindex = 0;}

     delay(500);    
  } 
 //Serial.println(strip.getBrightness());
 
 
 }
}

void loop() {
//BlueTooth Terminal Connectivity
 if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(20);
}

void stripclear () {strip.clear();}

void stripled (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      delay(5);
}

void neopixels (int red, int green, int blue) {
  for (int i = 0; i < LED_NUM; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      strip.show();
      delay(25);
  }
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

void neopattern2 (int red1, int green1, int blue1, int red2, int green2, int blue2) {
  for (int i = 0; i < 5; i++) {
      strip.setPixelColor (i, strip.Color(red1, green1, blue1));
      strip.setPixelColor (i+5, strip.Color(red2, green2, blue2));
      strip.setPixelColor (i+10, strip.Color(red1, green1, blue1));
      strip.setPixelColor (i+15, strip.Color(red2, green2, blue2));
      strip.setPixelColor (i+20, strip.Color(red1, green1, blue1));
      delay(15);
   }
  strip.show();

}

void neopattern3 (int red1, int green1, int blue1, int red2, int green2, int blue2) {
  for (int i = 1; i < 24; i++) {
      strip.setPixelColor (i, strip.Color(red1, green1, blue1));
      strip.show();
      delay(15);
   }
  
  strip.setPixelColor (0, strip.Color(red2, green2, blue2));
  strip.setPixelColor (4, strip.Color(red2, green2, blue2));
  strip.setPixelColor (12, strip.Color(red2, green2, blue2));
  strip.setPixelColor (20, strip.Color(red2, green2, blue2));
  strip.setPixelColor (24, strip.Color(red2, green2, blue2));
  strip.show();

}



void handleNeoMessage(AdafruitIO_Data *data) {
  Serial.println("Received NeoPixels Strip Color:");
  Serial.print("  - R: ");
  Serial.println(data->toRed());
  Serial.print("  - G: ");
  Serial.println(data->toGreen());
  Serial.print("  - B: ");
  Serial.println(data->toBlue());
  Serial.print("  - HEX: ");
  Serial.println(data->value());
  neopixels(data->toRed(), data->toGreen(),data->toBlue());
}

void handleBrightnessMessage(AdafruitIO_Data *data) {
  Serial.println("Received Brightness:");
  
  // convert the data to integer
  int brightness = data->toInt();
  brightness = map(brightness,0,100,0,255);
  Serial.println(brightness);
  strip.setBrightness(brightness); //(up to 255)
  strip.show();

}