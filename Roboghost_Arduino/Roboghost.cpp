#include "Roboghost.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

 
const int LedsPin = 1;
const int LED_NUM = 6;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LedsPin, NEO_GRB + NEO_KHZ800);

Roboghost::Roboghost() {
}

void Roboghost::begin() {
       
  Serial.println("**--  Roboghost begin  --**");
  pinMode (LedsPin,OUTPUT);
  strip.begin();
  strip.setBrightness(200); //(up to 255)
  strip.clear();  
}


void stripled (int lednum, int red, int green, int blue) {
      strip.setPixelColor(lednum, strip.Color(red, green, blue));
      strip.show();
      delay(50);
}
void Roboghost::neopixels (int red, int green, int blue) {
  for (int i = 0; i < LED_NUM; i++) {
      strip.setPixelColor (i, strip.Color(red, green, blue));
      strip.show();
      delay(50);
  }
}



