
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(25, 32);  // CE, CSN Default SPI Pins: CLK, MISO, MOSI = 18, 19, 23

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
   Serial.begin(115200);
  Serial.println("Started Transmitter");
 radio.setDataRate( RF24_1MBPS );


  //radio.begin();
  Serial.println(radio.begin());
  //set the address
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);

  //Set module as transmitter
  radio.stopListening();
  radio.printDetails();
  
}
void loop()
{
   // Serial.println(radio.begin());
    Serial.println("TRASMITTING");

//Send message to receiver
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
 Serial.print("Radio SPI Bus status =");
 Serial.println(radio.isChipConnected());
  delay(1000);
}