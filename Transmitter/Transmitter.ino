#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 9); // CE, CSN         

#define joybtn1 0  // Joystick button 1
#define joybtn2 1  // Joystick button 2
#define tgl1 7   // Toggle switch 1
#define tgl2 4   // Toggle switch 1
#define btn1 8   // Button 1
#define btn2 9   // Button 2
#define btn3 2   // Button 3
#define btn4 3   // Button 4
#define buzz 10  //Buzzer

const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte joy2_X;
  byte joy2_Y;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;


};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(115200);

  // radio communication
  if(!radio.begin()){
    while(1);
  }
    radio.printPrettyDetails();

  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  //radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  //Serial.println(radio.isChipConnected());
  Serial.println("started");
  delay(2000);

 /* pinMode(joybtn1, INPUT_PULLUP);
  pinMode(joybtn2, INPUT_PULLUP);
  pinMode(tgl1, INPUT_PULLUP);
  pinMode(tgl2, INPUT_PULLUP);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz,LOW);
*/
}


void loop() {
  Serial.print("IsChipConnected = ");
  Serial.println(radio.isChipConnected());
  // Read all analog inputs and map them to one Byte value
//  data.joy1_Y = map(analogRead(A0), 0, 1023, 0, 255);
  data.joy1_X = 4; // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
 // data.joy2_X = map(analogRead(A2), 0, 1023, 0, 255);
    data.joy2_X = 12;
    data.joy1_Y = 13;

  data.joy2_Y = map(analogRead(A3), 0, 1023, 0, 255);
  data.pot1 = map(analogRead(A7), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A6), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = 1; //digitalRead(joybtn1);
  data.j2Button = digitalRead(joybtn2);
  data.tSwitch2 = digitalRead(tgl2);
  data.tSwitch1 = digitalRead(tgl1);
  data.button1 = digitalRead(btn1);
  data.button2 = digitalRead(btn2);
  data.button3 = digitalRead(btn3);
  data.button4 = digitalRead(btn4);

  
  Serial.println(radio.writeFast(&data, sizeof(Data_Package))); // Send the whole data from the structure to the receiver
 delay(1000);
}
