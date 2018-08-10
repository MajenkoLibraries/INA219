#include <Wire.h>
#include <INA219.h>

INA219 current(Wire);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    current.begin();
}

void loop() {
    Serial.print("Bus voltage: ");
    Serial.print(current.readVoltage(), 2);
    Serial.println("V");
    Serial.print("Shunt current: ");
    Serial.print(current.readCurrent(), 4);
    Serial.println("A");
    delay(100);
}
