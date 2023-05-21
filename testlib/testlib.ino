#include "testlib.h"

testlib mytest;  

void setup() { 
  
  mytest.begin();

 }

void loop() { 
  mytest.action(255,255);
 }