 /* Wemos basic Software
by: Gal Arbel
2022
 */
 #include "AdafruitIO_WiFi.h"
 #include "WiFiClientSecure.h"
 #include <WiFi.h>
 
 #define AIO_SERVER      "io.adafruit.com"
 #define AIO_SERVERPORT  8883              // Using port 8883 for MQTTS
 #define AIO_USERNAME  "galarb"
 #define AIO_KEY       "aio_pAYD82KJpGasJG6voCrZ0M2fT3l0"

 #define ssid "arbs"   
 #define pass "lafamilia"
 
 WiFiClientSecure client;                  // WiFi ClientSecure for SSL/TLS support

 AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, ssid, pass);
 // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
 Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 //io.adafruit.com root CA
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


 Adafruit_MQTT_Publish testval = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Wemos_Value"); // setup the feeds

 AdafruitIO_Feed *Wemos_switch = io.feed("Wemos_Switch");


 void setup() {
 Serial.begin(115200);
 pinMode(2, OUTPUT);
 WiFi.begin(ssid, pass);
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
   Serial.print("Connecting to Adafruit IO");

 vTaskDelay(1000 / portTICK_PERIOD_MS);
 xTaskCreate(task1,"task1", 2048, NULL,1,NULL);
 xTaskCreate(task2,"task2", 2048, NULL,1,NULL); 
}
void loop() {
 vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void task1( void * parameter )
{
  
     MQTT_connect();  // Ensure the connection to the MQTT server is alive (this will make the first connection and automatically reconnect when disconnected).
    for (int i = 0; i < 5; i++){
     testval.publish (i);
     delay (2000);
     }
   vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void task2( void * parameter)
{
  while(1) {
     digitalWrite(2, HIGH);
     vTaskDelay(500 / portTICK_PERIOD_MS);
     digitalWrite(2, LOW);
     vTaskDelay(500 / portTICK_PERIOD_MS);
  } 
}
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
