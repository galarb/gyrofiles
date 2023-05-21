#ifndef SIMPLE_DEVICE_CONTROL_H
#define SIMPLE_DEVICE_CONTROL_H

#include <Arduino.h>
#include <BetterOTA.h>

void onCommand(const String &command, String args[]);

inline void print(const String &str) {
  Serial.println(str);
  OTATerminal.println(str);
}

template <typename... T>
void print(const String& str, T... args) {
  Serial.printf((str + "\n").c_str(), args...);
  OTATerminal.printlnf(str, args...);
}

class SimpleDeviceControlClass {
private:
    SimpleDeviceControlClass() = default;

public:
    static SimpleDeviceControlClass getInstance();

    void begin();
};

extern SimpleDeviceControlClass SimpleDeviceControl;

#endif //SIMPLE_DEVICE_CONTROL_H
