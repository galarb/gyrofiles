#include <SPI.h>
#include "printf.h"
#include "RF24.h"

// instantiate an object for the nRF24L01 transceiver
RF24 radio(8, 9);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[6] = "00001";
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX node, false = RX node

// For this example, we'll be sending 32 payloads each containing
// 32 bytes of data that looks like ASCII art when printed to the serial
// monitor. The TX node and RX node needs only a single 32 byte buffer.
#define SIZE 32             // this is the maximum for this example. (minimum is 1)
char buffer[SIZE + 1];      // for the RX node
uint8_t counter = 0;        // for counting the number of received payloads


void setup() {

  buffer[SIZE] = 0;  // add a NULL terminating character (for easy printing)

  Serial.begin(115200);
   // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  radioNumber = 0;
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit
  radio.setPayloadSize(SIZE);  // default value is the maximum 32 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address);  // always uses pipe 0

  
  
    radio.stopListening();  // put radio in TX mode
  
  

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

}  

int failures = 0;
void loop() {
  
  buffer[0] = "H";
    buffer[1] = "L";

    radio.flush_tx();
    uint8_t i = 0;
    unsigned long start_timer = micros();  // start the timer
      if (!radio.writeFast(&buffer, SIZE)) {
        failures++;
        radio.reUseTX();
      }
  
      if (failures >= 100) {
        Serial.print(F("Too many failures detected. Aborting at payload "));
        Serial.println(buffer[0]);
      }
    
    unsigned long end_timer = micros();  // end the timer

    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us with "));
    Serial.print(failures);  // print failures detected
    Serial.println(F(" failures detected"));

    delay(1000);  // slow transmissions down by 1 second


    
} 


