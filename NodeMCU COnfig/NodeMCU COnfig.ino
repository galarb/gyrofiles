/* /////               ACFD Automatic Cat Food Discepnser       ///////////////////////
NodeMCU Development Board 8266 software
By: Shimi Mahluf, Gal Arbel
*/

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define SECRET_SSID "arbs"           // replace MySSID with your WiFi network name
#define SECRET_PASS "lafamilia"      // replace MyPassword with your WiFi password
#define SECRET_CH_ID 1396813         // this is my ThingSpeak CHANNEL ID
#define SECRET_WRITE_APIKEY "VDEDADL7JFCVJFVR"  // ThingSpeak API Code

char ssid[] = SECRET_SSID;           // network SSID (name)
char pass[] = SECRET_PASS;           // network password
WiFiClient client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

int TRYWIFI = 4;                        // D2 Pin - "connecting to Wi-Fi" Yellow LED
int WIFIONLED = 5;                      //D1 Pin - "Connected to Wi-Fi" Blue Led

String myStatus = "";                   // Thingspeak string value

byte uartin;                            // UART incoming message Variable
char UARTarray[20] = "";                // Array to store the message characters
int i = 0;                              // messages counter
bool Receiving = LOW;                   // messages Flag, receiving data on UART

void setup() {
  
  Serial.begin(115200);                 // Baud rate: 115200, but you can define any baud rate you want
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);             // Initialize ThingSpeak
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
       digitalWrite(TRYWIFI, HIGH);
       digitalWrite (WIFIONLED, LOW);
        delay (1000);
        digitalWrite (TRYWIFI, LOW);
        delay (1000);
      Serial.print(".");
      delay (2000);
    }
    Serial.println("\nConnected.");
      digitalWrite(TRYWIFI, LOW);
      digitalWrite(WIFIONLED, HIGH);
  }

//Write to THingspeak
  ThingSpeak.setField(1,weightsensor);
  ThingSpeak.setField(2,"1" );
  ThingSpeak.setField(3, "2" );
  ThingSpeak.setField(4, "10");
 // set the status line in THingspeak
  myStatus=String("its working...");
  ThingSpeak.setStatus(myStatus);

    // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 //weightCheck();
 // delay(1000);
 
 ////// Serial Reading Programming ///////
 /////// Message format: < begin message. > End message   //////
   if (Serial.available() > 0)
  {
    if (Receiving == HIGH)                     //"<" received
    {
      uartin = Serial.read();                  // reading , waiting for ">"
      if (uartin != '>')
      {
        Serial.print("current input is:");     // 
        Serial.println((char)uartin);          // Printing single char
        UARTarray[i] = uartin;                 // storing into the Array
        i++;
      }
      else
      {
        Serial.print("full input is:");        // Printing the full array

        Serial.println(UARTarray);             // Printing the full array
      }
    }
    else
    {
      if (Serial.read() == '<')
      {
        Receiving = HIGH;                     // Set the receive Flag
      }
    }
  }                              
}

void actuation(bool value) {            // Function called when message arrives to your specified asset. "value" is the actual value received.
 if (value == true) {
    digitalWrite(TRYWIFI, HIGH);         // Set pin "TRYWIFI" to HIGH (ON)
    delay(1000);
    Serial.println("FEED NOW Cycle initiated");    // Prints to serial port
  } else {
    digitalWrite(TRYWIFI, LOW);          // Sets pin "TRYWIFI" to LOW (OFF)
    Serial.println("LED Turned OFF");   // Prints to serial port
 }
}


void weightCheck() {                    // function that checks the value of analog port
  weightsensor = analogRead(analogpin);           // Reads the analog port A0 and saves it to weightsensor variable
  Serial.print("Current weight Value: ");  // Prints to Serial port
  Serial.println(weightsensor);            // Prints to Serial port
  
         
}

