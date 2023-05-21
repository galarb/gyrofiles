 //Include Libraries
#include "Arduino.h"
#include <SoftwareSerial.h>
SoftwareSerial softSerial(8, 9); // RX, TX

void setup() 
{
  uint32_t baud = 115200;
  Serial.begin(baud);
  softSerial.begin(baud);
  Serial.print("SETUP!! @");
  Serial.println(baud);
  Serial.write("char ssid[] = SECRET_SSID"); 
    pinMode(LED_BUILTIN, OUTPUT);

  
}

void loop() 
{

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(1000);
  
    while(softSerial.available() > 0) 
    {
      char a = softSerial.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }
    
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      Serial.write(a);
      softSerial.write(a);
    } 
    
}
