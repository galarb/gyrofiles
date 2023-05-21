/* /////                Roboghost Software       ///////////////////////
             Robot Car software using Arduino

By: Gal Arbel
Oct 2022
*/
#include "Roboghost.h"
#include "gyroturn.h"
bool flag = true; // a flag to make this procedure only happen once.

gyroturn myturn(2, 4, 7, 8, 3, 6);  //in1, in2, in3, in4, enA, enB in L298N. 
//make sure enA nd enB are PWM pins


Roboghost roboghost; //create an Object. Init Roboghost!

/*
//create an RF24 object
RF24 radio(4, 0);  // CE, CSN Default SPI Pins: CLK, MISO, MOSI = 13, 12, 11

//address through which two modules communicate.
const byte address[6] = "00001";
int speedright = 0;
int speedleft = 0;
bool btn1 = 0;
bool btn2 = 0;
int control[2]; 
char receivedData[32] = "";
*/

void setup() {
  myturn.begin(3, 1, -20); //Kp, Ki, Kd. it also starts serial monitor at 115200
  roboghost.begin();  
  roboghost.neopixels(255, 0, 0);

/*
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
*/
}


void loop() {
/*if (flag){
   myturn.gotoang(-45, 100); //go left. try correcting 100 times.
   delay(2000);  
   myturn.gotoang(45, 100); //go left. try correcting 100 times.
   flag = false; 
  }
   */
   
  

}






