#include "SimpleDeviceControl.h"

SimpleDeviceControlClass SimpleDeviceControl = SimpleDeviceControlClass::getInstance();

SimpleDeviceControlClass SimpleDeviceControlClass::getInstance() {
    static SimpleDeviceControlClass instance;
    return instance;
}

TaskHandle_t BackLoopTask;

void handleMessage(const String& message) {
    String command = "";
    String args[16];

    String currentArgString = "";
    int currentArgIndex = -1;

    for (char c : message) {

        if (c == ' ') {
            if (currentArgIndex != -1) args[currentArgIndex] = currentArgString;

            currentArgIndex++;
            currentArgString = "";

        } else if (currentArgIndex == -1) {
            command += c;
        } else {
            currentArgString += c;
        }
    }

    onCommand(command, args);
}

void backLoop(void * param) {
    BetterOTA.handle();

    if (Serial.available() > 0) handleMessage(Serial.readString());
}

void SimpleDeviceControlClass::begin() {
    Serial.begin(115200);
    WiFi.softAP("ESP32");
    OTACodeUploader.begin();
    OTATerminal.begin();

    OTATerminal.setHandler(handleMessage);

    xTaskCreatePinnedToCore(backLoop, "BackLoopTask", 10000, nullptr, 1, &BackLoopTask, 0);
}
