

/* ****    Wireless Joystick 
**** Working on 2.4GHz with nRF24L01 Radio Module
*  /////       Transmitter Code

**** By: Gal Arbel
**** 2022
*/

# include "GalJoystic.h"
GalJoystic myjoystic(100);

void setup() {
//Serial.begin(115200);
//Serial.println("test");
myjoystic.begin(115200);
}



void loop() {

  myjoystic.joyrun();
//Serial.println("test");

}
