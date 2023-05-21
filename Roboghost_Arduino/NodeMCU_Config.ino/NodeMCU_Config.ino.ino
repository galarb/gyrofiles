/* ACFD Automatic Cat Food Discepnser
NodeMCU 8266 software
By: Gal Arbel
Shimi Mahluf
*/
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define SECRET_SSID "arbs"       // replace MySSID with your WiFi network name
#define SECRET_PASS "lafamilia"  // replace MyPassword with your WiFi password

#define SECRET_CH_ID 1396813                    // this is my ThingSpeak CHANNEL ID
#define SECRET_WRITE_APIKEY "VDEDADL7JFCVJFVR"  // ThingSpeak API Code

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password

WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;

int ledPin = 4;  // D2 - connecting to Wi-Fi
int PINTEST = 5; //D1 - Connected

const int analogpin = A0; //is this the right number of this pin?
int weightsensor = 0;  // Variable that will store the value of the analog port

String myStatus = "";

void setup() {
  pinMode(ledPin, OUTPUT);   // Set the pin of our LED to OUTPUT
  pinMode(PINTEST, OUTPUT);  // Set the pin of our LED to OUTPUT
  pinMode (analogpin,INPUT);
  Serial.begin(115200);  // Baud rate: 115200, but you can define any baud rate you want
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}
/*
void actuation(bool value) {            // Function called when message arrives to your specified asset. "value" is the actual value received.
 if (value == true) {
    digitalWrite(ledPin, HIGH);         // Set pin "ledPin" to HIGH (ON)
    delay(1000);
    Serial.println("FEED NOW Cycle initiated");    // Prints to serial port
  } else {
    digitalWrite(ledPin, LOW);          // Sets pin "ledPin" to LOW (OFF)
    Serial.println("LED Turned OFF");   // Prints to serial port
 }
}
*/

void weightCheck() {                    // function that checks the value of analog port
  weightsensor = analogRead(analogpin);           // Reads the analog port A0 and saves it to weightsensor variable
  Serial.print("Current weight Value: ");  // Prints to Serial port
  Serial.println(weightsensor);            // Prints to Serial port
                                           // startMillis = currentMillis;                       // Resets the startMillis by assigning it the value of currentMillis
                                           // }
}



void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
       digitalWrite(ledPin, HIGH);
       digitalWrite (PINTEST, LOW);
        delay (1000);
        digitalWrite (ledPin, LOW);
        delay (1000);
      Serial.print(".");
      delay (2000);
    }
    Serial.println("\nConnected.");
      digitalWrite(ledPin, LOW);
      digitalWrite(PINTEST, HIGH);

  }

//Write to THingspeak
  ThingSpeak.setField(1,weightsensor);
  ThingSpeak.setField(2,"1" );
  ThingSpeak.setField(3, "2" );
  ThingSpeak.setField(4, "10");
 // set the status
  myStatus=String("its working...");
  ThingSpeak.setStatus(myStatus);

    // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 weightCheck();
  delay(1000);
 
}
