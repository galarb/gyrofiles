# SimpleDeviceControl

### Example Usage
```cpp
#include <SimpleDeviceControl.h>

void setup() {
    SimpleDeviceControl.begin();

    pinMode(16, OUTPUT);
}

void onCommand(const String& command, String args[]) {
    if (command == "high") {
        pinMode(args[1].toInt(), OUTPUT);
        digitalWrite(args[1].toInt(), HIGH);
        print("set pin %d to HIGH", args[1].toInt());
    }

    if (command == "low") {
        pinMode(args[1].toInt(), OUTPUT);
        digitalWrite(args[1].toInt(), LOW);
        print("set pin %d to LOW", args[1].toInt());
    }
}

void loop() {
    digitalWrite(16, HIGH);
    delay(1000);
    digitalWrite(16, LOW);
    delay(1000);
}
```
