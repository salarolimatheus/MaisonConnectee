#include "debug.hpp"

void ConfigureSerialDebug(const unsigned long baud) {
    Serial.begin(baud);
    while(!Serial) {delay(50);}
    Serial.println("Serial Started");
}

void PrintBsecData(PackageDataBsec* packageDataBsec) {
    Serial.println(packageDataBsec->toString());
}
