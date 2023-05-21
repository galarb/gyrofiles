#include "Adafruit_ST77xx.h"
#include "Adafruit_SPITFT.h"
#include "esp32-hal-adc.h"
#include "ND.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>



Adafruit_ST7789 tft = Adafruit_ST7789(3, 26, 17);

ND::ND(int sensorPin) {
 
_sensorPin = sensorPin;
  
  pinMode(_sensorPin, INPUT); 
  

}

void ND::begin(double bdrate) {
  delay(30);
  Serial.begin(bdrate);      
  Serial.println("Started");
  Serial.print("Sennsor Pin = ");  
  Serial.println(_sensorPin);
  tft.init(240, 240, SPI_MODE2); 
 // tft.fillScreen(ST77XX_RED);
  tft.setRotation(2);     //to 90 deg
  tft.setTextSize(2); //1 is default 6x8, 2 is 12x16, 3 is 18x24

  delay(30);
}
int ND::measurenoise(){
  int measurement = map(analogRead(_sensorPin), 50, 200, 1, 5);
  Serial.println(measurement);
  return(measurement);
}
void ND::checklevel(){
  switch(measurenoise()) {
    case(1):
     tft.fillScreen(ST77XX_GREEN);

     break;
    case(2):
     tft.fillScreen(ST77XX_CYAN);

     break;
    case(3):
     tft.fillScreen(ST77XX_YELLOW);

     break;
    case(4):
     tft.fillScreen(ST77XX_MAGENTA);

     break;
    case(5):
     tft.fillScreen(ST77XX_RED);

     break;
  }
}
