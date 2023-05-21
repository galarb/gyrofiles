 /*  ****                Mouse movementreadign Software         *******
for ESP32 and Arduino
Software to extract Robot speed from PS/2 mouse and light RGB LED  according to the speed

By: Shimi Mahluf
Gal Arbel
Technion
2021
**************
This Software measure mouse movement on the Y axis and lights RGB led with:
BLUE = High Speed
Green = Medium Speed
Yellow = Slow Speed
Red = Stopped
/////////////////////////////////////////////*/

#include "fabgl.h"

fabgl::Mouse Mouse;
int counter = 0;
int mouseEvents[5];
int ledrgb[3];
const int redPin = 2; //servo1
const int greenPin = 9; //servo2
const int bluePin = 33; //relay1
const int yellowPin = 39; //light1

void setup()
{
  Serial.begin(115200);
  delay(500);  // avoid garbage into the UART
  Serial.write("\n\nReset\n");
  Mouse.begin(GPIO_NUM_26, GPIO_NUM_27); //CLK,DAT
 }

int abs(int n) {
  if (n < 0) {
    return (n*-1);
  }

  return n;
}
//[40,40,5,6,7] 
void addMouseEvent(int deltay) {
  for (int i=1;i<5;i++) {
    mouseEvents[i-1]=mouseEvents[i];
  }
  mouseEvents[4] = deltay;
}

void mouseMovement() {
    ledrgb[0]=0;
    ledrgb[1]=0;
    ledrgb[2]=0;
     if (Mouse.deltaAvailable()) {
      MouseDelta mouseDelta;
      Mouse.getNextDelta(&mouseDelta);
      int deltaY = abs(mouseDelta.deltaY);
      addMouseEvent(deltaY);
      
     //  Serial.printf("%d: deltaX = %d  deltaY = %d  deltaZ = %d  leftBtn = %d  midBtn = %d  rightBtn = %d\n",
       //     counter,mouseDelta.deltaX, mouseDelta.deltaY, mouseDelta.deltaZ,
         //    mouseDelta.buttons.left, mouseDelta.buttons.middle, mouseDelta.buttons.right);

    }
    else {
      addMouseEvent(0);
    }
      if (mouseEvents[4] > 80)  {
            ledrgb[0]=80;
            ledrgb[1]=0;
            ledrgb[2]=80;
      } //fast - PURPLE
      if (mouseEvents[4] < 80 && mouseEvents[4] > 50) { 
            ledrgb[0]=0;
            ledrgb[1]=255;
            ledrgb[2]=0;
      } //medium - GREEN
      if (mouseEvents[4] < 50 && mouseEvents[4] > 4) { 
            ledrgb[0]=255;
            ledrgb[1]=255;
            ledrgb[2]=0;
      } //slow - YELLOW
      if (mouseEvents[4] < 4) { 
            ledrgb[0]=255;
            ledrgb[1]=0;
            ledrgb[2]=0;
      } //stopped - RED
}

void loop()
{
  mouseMovement();
  setColor(ledrgb[0],ledrgb[1],ledrgb[2]);
  Serial.printf("%d %d %d \n",ledrgb[0],ledrgb[1],ledrgb[2]);
  delay(100);
  /*digitalWrite (redPin,LOW);
  digitalWrite (greenPin,LOW);
  digitalWrite (bluePin,LOW);
  digitalWrite (yellowPin,LOW);
  delay(500);
*/
   
}
void setColor(int red, int green, int blue)
{
 /* #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
*/  
  if (red == 255 && green == 0 && blue == 0) {digitalWrite(redPin, HIGH);}
  if (red == 255 && green == 255 && blue == 0) {digitalWrite(yellowPin, HIGH);}
  if (red == 0 && green == 255 && blue == 0) {digitalWrite(greenPin, HIGH);}
  if (red == 80 && green == 0 && blue == 80) {digitalWrite(bluePin, HIGH);}


}
