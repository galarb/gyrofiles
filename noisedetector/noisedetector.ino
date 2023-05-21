#include "nd.h"
#include "clicli.h"

ND mynd(33); //sensorPin)
clicli mycli(mynd);

void setup() {
  mynd.begin(115200);  
  
}

void loop() {
 mycli.run();
 mynd.checklevel();
 delay(1000);
}
