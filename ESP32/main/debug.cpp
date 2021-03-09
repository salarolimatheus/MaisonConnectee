#include "debug.hpp"

void ConfigureSerialDebug(const unsigned long baud) {
    Serial.begin(baud);
    while(!Serial);
    delay(100);
    Serial.println("Serial Started");
}
