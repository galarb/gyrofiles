#include "testlib.h"
#include "Arduino.h"

testlib::testlib() { }

void testlib::begin() {
  Serial.begin(115200);       
}

void testlib::action(int speedR, int speedL){
  /* do something */
}
