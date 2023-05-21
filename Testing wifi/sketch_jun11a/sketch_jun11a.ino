#include "ThingSpeak.h"
#include <SoftwareSerial.h>            // set UART communication to softserial port

#define DEBUG true
#define RX 2
#define TX 3
#define SECRET_SSID "arbs"           // replace MySSID with your WiFi network name
#define SECRET_PASS "lafamilia"      // replace MyPassword with your WiFi password
#define SECRET_CH_ID 1396813         // this is my ThingSpeak CHANNEL ID
#define SECRET_WRITE_APIKEY "VDEDADL7JFCVJFVR"  // ThingSpeak API Code

String HOST = "api.thingspeak.com";
String PORT = "80";
String PROTOCOL = "TCP";
String AP = "arbs";
String PASS = "lafamilia";

String API = "VDEDADL7JFCVJFVR";
String field = "field1";

int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 90;

SoftwareSerial esp8266(RX,TX);
void setup() {

Serial.begin(115200);
esp8266.begin(115200);

sendCommand("AT",5,"OK");
sendCommand("AT+CWMODE=3",5,"OK");  // this mode Enables TCP Single connection mode
sendCommand("AT+CWJAP=\""+ AP + "\",\""+ PASS + "\"",40,"OK");
countTrueCommand = 0;

}

void loop() {

String setData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor); // Sending Data to Thingspeak
switch(countTrueCommand) {

case 0: sendCommand("AT",5,"OK");break;
case 1: countTrueCommand++;break; //sendCommand("AT+RST",10,"invalid");break;
case 2: sendCommand("AT+CIPMUX=0",5,"OK"); break;
case 3: sendCommand("AT+CIPSTART=\"" + PROTOCOL + "\",\"" + HOST + "\"," + "80",15,"OK"); break; // connection id=0 
//AT+CIPSTART="TCP","api.thingspeak.com",80
//AT+CIPSEND		//From	now	on,	data	received	from	UART	will	be	transmitted	to	the	server	automatically.
// GET /update?api_key=VDEDADL7JFCVJFVR&field1=80 /+++to stop sending
//case 4: sendCommand("AT+CIPSTART=" + strlen(setData),4,">"); break; // sending to id=0, length of message
case 4: sendCommand("AT+CIPSEND=" +String(setData.length()+4),4,">"); break; // sending to id=0, length of message
                      
case 5: esp8266.println(setData);delay(1500);countTrueCommand++; break;
case 6: countTrueCommand++;break; //esp8266.println("+++"); break;
case 7: sendCommand("AT+CIPCLOSE",5,"OK"); break;
case 8:

Serial.println(valSensor);
Serial.print(setData);
Serial.print(",");
Serial.println(setData.length());
valSensor = 10; // random value, change with sensor value if using sensor
countTrueCommand = 0;
delay(10000);
break;

}

}
void sendCommand(String command, int maxTime, char readReplay[]) {

Serial.print(countTrueCommand);
Serial.print(". at command => ");
Serial.print(command);
Serial.print(" ");
while(countTimeCommand < (maxTime*1))

{

esp8266.println(command);//at+cipsend
if(esp8266.find(readReplay))//ok
{

found = true;
break;

}
countTimeCommand++;

}
if(found == true)
{

Serial.println("OYI");

countTrueCommand++;
countTimeCommand = 0;

}
if(found == false)
{

Serial.println("Fail");
countTrueCommand = 0;
countTimeCommand = 0;

}
found = false;

}

