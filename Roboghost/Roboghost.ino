/* /////               Roboghost       ///////////////////////
CAR Robot software using Adafruit.io
By: Gal Arbel
*/
 #include <WiFi.h>
 #include "WiFiClientSecure.h"
 #include "Adafruit_MQTT.h"
 #include "Adafruit_MQTT_Client.h"
 #include "DHT.h"
 #include <Wire.h>
 //#include "FaBoPWM_PCA9685.h"
 #include <Adafruit_PWMServoDriver.h>

 #define ssid "arbs"// "iPhone 8 gal"//           
 #define pass "lafamilia"// "arduino1"//"arduino1"  

 #define AIO_USERNAME  "galarb"            //personal info from adafruit.io
 #define AIO_KEY       "aio_pAYD82KJpGasJG6voCrZ0M2fT3l0"
 #define AIO_SERVER      "io.adafruit.com"
 #define AIO_SERVERPORT  8883              // Using port 8883 for MQTTS
//INPUT pins definitions, as per Board Labeling
 const int LightAnalog1 = 39;               // Analog Light Meter (TEMT6000)
 const int LightAnalog2 = 36;               // Analog Light Meter (TEMT6000)
 const int SoilAnalog1 = 35;                // Analog hygrometer Capacitive
 const int DHTpin = 19;
 //OUTPUT pins definitions
 const int IRObsL = 18;                       // Misakenily named "Soil2_Analog", while this is a Digital IO
 const int IRObsR = 14;
 const int Echo = 23;
 const int Relay1 = 33;
 const int DAT = 26;
 const int Servo2 = 9;
 const int Trig = 5;
 const int IO16 = 16;       
 const int IO17 = 17;       
 const int IO12 = 12;       
 const int IO32 = 32;     
 const int IO25 = 25;
 const int IO27 = 27; 
 // Setting PWM properties
 const int freq = 30000;    // PWM Frequency
 const int pwmChannel1 = 0; // PWM pin IO0
 const int pwmChannel2 = 4; // PWM pin IO4
 //const int pwmChannel3 = 2; // PWM pin IO2 / "servo1"
 const int resolution = 8;
 const int Rpin = 9;
 const unsigned int MAX_MESSAGE_LENGTH = 64; 
DHT dht(DHTpin, DHT11);                   // setup DHT sensor 
byte temperature = 0;
byte humidity = 0;                                      
 
WiFiClientSecure client;                  // WiFi ClientSecure for SSL/TLS support
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// io.adafruit.com root CA
const char* adafruitio_root_ca = \   
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
    "-----END CERTIFICATE-----\n";


 Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity"); // setup the feeds
 Adafruit_MQTT_Publish Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temp");
 Adafruit_MQTT_Publish Light1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light1");
 Adafruit_MQTT_Publish Light2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light2");
 Adafruit_MQTT_Publish LightI2C1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/LightI2C1");
 Adafruit_MQTT_Publish LightI2C2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/LightI2C2");
 Adafruit_MQTT_Publish SM1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/SM1"); // Soil Moisture Sensor 1

 Adafruit_MQTT_Subscribe Relay1Button = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1"); // Relay1 Button 
 Adafruit_MQTT_Subscribe RMSlider = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/RIGHT Motor"); // Slider 
 Adafruit_MQTT_Subscribe LMSlider = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LEFT Motor"); // Slider

void setup() {
  
  Serial.begin(115200);                   
  //stop();
  pinMode (Relay1,OUTPUT);
  pinMode (2,OUTPUT); //PWM IO2 
  pinMode (0,OUTPUT); //PWM IO0 (ENA R)
  pinMode (4,OUTPUT); //PWM IO4 (ENA L)
 
  // configure LED PWM functionalitites - no "analogWrite" command in ESP32  for PWM pins.
  ledcSetup(pwmChannel1, freq, resolution); // a workaround command to affect another pin
  ledcAttachPin(Relay1, pwmChannel1); //setting IO16 as the affected EN-A pin
  ledcSetup(pwmChannel2, freq, resolution);
  ledcAttachPin(27, pwmChannel2); //setting IO27 as the affected EN-B pin
 // ledcSetup(pwmChannel3, freq, resolution);
 //  ledcAttachPin(Rpin, pwmChannel3); //setting IO34 as the affected RED RGB LED Pin

  //pinMode (Servo2,OUTPUT); // not working?
  pinMode (Trig,OUTPUT); 
  pinMode (IO16,OUTPUT);    //ENA L (EN-B)
  pinMode (IO17,OUTPUT);    //L Motor pin1
  pinMode (IO12,OUTPUT);    //L Motor pin2
  pinMode (IO32,OUTPUT);    //R Motor pin1
  pinMode (IO25,OUTPUT);    //R Motor pin2
  pinMode (IO27,OUTPUT);    // ENA R (EN-A)
  pinMode (Rpin,OUTPUT);    // used as PWM for GRB LED
 // pinMode (SoilAnalog1,INPUT);  
  //pinMode (LightAnalog1,INPUT);    
  // pinMode (LightAnalog2,INPUT);    
  pinMode (IRObsR,INPUT);     
  pinMode (IRObsL,INPUT);    
  pinMode (Echo,INPUT); 
 // dht.begin();              // starting the DHT Sensor

 // Frontlights (100, 100, 100); //  Color

 
 /*WiFi.begin(ssid, pass);
   Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
     delay(250);
     Serial.print(".");
     delay(250);
   }
   Serial.println("");
   Serial.print("Connected to WiFi network with IP Address: ");
   Serial.println(WiFi.localIP());
  
  client.setCACert(adafruitio_root_ca);  // Set Adafruit IO's root CA
   mqtt.subscribe(&Relay1Button);       // Subscribe the Relay with Adafruit
   mqtt.subscribe(&RMSlider);           // Subscribe Motor Speed Slider
   mqtt.subscribe(&LMSlider);
*/

}

void loop() {
  /*

 /* MQTT_connect();   // Ensure the connection to the MQTT server is alive (this will make the first connection and automatically reconnect when disconnected). 
   Adafruit_MQTT_Subscribe *subscription;
   while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &Relay1Button) {
      Serial.println(F("Got Relay1 activation: "));
      Serial.println((char *)Relay1Button.lastread);
      if (int (strcmp ((char *)Relay1Button.lastread,"N")) == 0) {relay1 ("ON");} else relay1 ("OFF");
      if (int (strcmp ((char *)Relay1Button.lastread,"F")) == 0) {relay1 ("OFF");} 
    }
     if (subscription == &RMSlider) {
      Serial.println(F("Got Right Motor Slider activation: ")); 
      Serial.println((char *)RMSlider.lastread);
      char  buffer [3];     // in order to put the char array into an int - speed
      buffer [0]= RMSlider.lastread[0];
      buffer [1]= RMSlider.lastread[1];
      buffer [2]= RMSlider.lastread[2];
      int speed = 0;
      speed=atoi(buffer);  // convertin the ASCII stored in the buffer to Decimal
      rmfwd (speed);
    }
    if (subscription == &LMSlider) {
      Serial.println(F("Got Left Motor Slider activation: ")); 
      Serial.println((char *)LMSlider.lastread);
      char  buffer [3];
      buffer [0]= LMSlider.lastread[0];
      buffer [1]= LMSlider.lastread[1];
      buffer [2]= LMSlider.lastread[2];
      int speed = 0;
      speed=atoi(buffer);  // convertin the ASCII stored in the buffer to Decimal
      lmfwd (speed);
    }
  }
 */
 //if (distance() <= 10) { stop();}
 delay(1000);
 if (digitalRead (IRObsR)==LOW) {
   Serial.println ("Right Obstacle!");
   // lmfwd (100); //slower speed on the opposite motor
   }
 if (digitalRead (IRObsL)==LOW) {
   Serial.println ("Left Obstacle!");
   // rmfwd (100);
  }
 digitalWrite(2, LOW);
 //Frontlights (120, 120, 120); // White Color
 //delay (5000);
 digitalWrite(2, HIGH);
 //Messages from Terminal
 
 while (Serial.available() > 0) {
   char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;
   char inByte = Serial.read();   //Read the next available byte in the serial receive buffer
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
     {
     message[message_pos] = inByte;  //Add the incoming byte to our message
     message_pos++;
     }
     //Full message received...
     else
     {
      message[message_pos] = '\0';     //Add null character to string
      Serial.println(message);     //echo the message to terminal
      int command[4];
      int argindex = 0;
      char cmd;
      char delim[] = " ";
	     char tmpmsg[MAX_MESSAGE_LENGTH];
       strcpy(tmpmsg,message);
       message_pos = 0;
       message[message_pos] = '\0';     //Add null character to string

        char *ptr = strtok(tmpmsg, delim);
	      while(ptr != NULL)
	       {
		      Serial.printf("'%s'\n", ptr);
          if (argindex == 0) {
            cmd = ptr[0];
          }
          command[argindex] = atoi(ptr);   
          //Serial.println(command[argindex]);
          argindex++;  
		      ptr = strtok(NULL, delim);
	       } 

      switch (cmd) {
       case 'r': rmfwd (command[1]);  break;
       case 'l': lmfwd (command[1]); break;
       case 'd': distance(); break;
       case 't': dhtread (); break;     
       case 'R': 
          Frontlights(command[1], command[2], command[3]); 
          break;
       message_pos = 0;     //Reset for the next message
      }
   }
   delay (10);
 } 
 
}

byte distance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);

  const unsigned long duration= pulseIn(Echo, HIGH);
 int x = duration/29/2;
 if(duration==0){
   Serial.println("Warning: no pulse from sensor");
   } 
  else{
      Serial.print("distance to nearest object:");
      Serial.println(x);
      Serial.println(" cm");
  }
 delay(10);
 return (x);
}


void stop () {
 digitalWrite(IO17, LOW);
 digitalWrite(IO12, LOW);
 digitalWrite(IO32, LOW);
 digitalWrite(IO25, LOW);
 ledcWrite(pwmChannel1, 0); 
 ledcWrite(pwmChannel2, 0); 
}

char* fwd (int speedR, int speedL) {// Move DC motors forward with "Duty Cycle" speed 0-255

   digitalWrite(IO17, HIGH);
   digitalWrite(IO12, LOW);    
   ledcWrite(pwmChannel1, speedL); 
   Serial.print("RIGHT Forward at: ");
   Serial.println(speedR); 
  
   digitalWrite(IO32, HIGH);
   digitalWrite(IO25, LOW);
   ledcWrite(pwmChannel2, speedR);
   Serial.print("LEFT Forward at: ");
   Serial.println(speedL); 

  delay (100); 
  return ("Forward");
}
void rmfwd (int rspeed) {
   digitalWrite(IO32, LOW);
   digitalWrite(IO25, HIGH);   
   rspeed = map (rspeed, 0, 255, 130, 255); // measurments showed that the motor only started moving at around "160"
   ledcWrite(pwmChannel2, rspeed); 
   Serial.print("RIGHT Forward at: ");
   Serial.println(rspeed); 
     
}
void lmfwd (int lspeed) {
  digitalWrite(IO17, LOW);
   digitalWrite(IO12, HIGH);    
   lspeed = map (lspeed, 0, 255, 130, 255);

   ledcWrite(pwmChannel1, lspeed); 
   Serial.print("LEFT Forward at: ");
   Serial.println(lspeed); 
     
}

void Frontlights(int red, int green, int blue)
{
 /*
 #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
*/
  // ledcWrite(pwmChannel3, red); 
   //Serial.write (red);
   Serial.printf("%d %d %d \n" ,red, green, blue);
}

void dhtread (){
 Serial.print(F("\nSending measurement. temp= "));
 Serial.print(measuretemp ());
 Serial.print(F("\nSending measurement. humidity= "));
 Serial.println(measurehumidity());
 //Humidity.publish(measurehumidity());        // publish data to adafruit
 //Temp.publish (measuretemp ());              // publish Temp data
}  
byte measuretemp () {                         // function to measure Temp
 temperature = dht.readTemperature();
 return temperature;
}
byte measurehumidity () {                     // function to measure Humidity
 humidity = dht.readHumidity();
 return humidity; 
}

void relay1 (char* Val) {
 if (Val == "ON") {
   Serial.println("Relay1 Activated!");
   digitalWrite(Relay1,HIGH); //Activate relay
   delay (1000); 
  }
if (Val == "OFF") {
   Serial.println("Relay1 De-Activated!");
   digitalWrite(Relay1,LOW); //de-Activate relay
   delay (1000); 
   }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}

