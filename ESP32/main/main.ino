// main.ino is the Main Logical Layer
#include "sensors.hpp"       // Sensors is a Control Layer
#include "debug.hpp"         // Debug is a Control Layer
#include "communication.hpp" // Communication is a Control Layer

const bool debugMode = true;
const unsigned long SERIAL_BAUD_RATE = 115200;

Bsec sensorBsec;
SPIClass mySPI(VSPI);

void setup() {
    if (debugMode)
        ConfigureSerialDebug(SERIAL_BAUD_RATE);

    //ConfigureWiFi();

    if (!ConfigureBsec(&sensorBsec, &mySPI, VSPI)) {
        PackageError packageError = AnalyseAndRetryBsec(&sensorBsec, &mySPI, VSPI);
        Serial.print(packageError.getText());
    }
}

void loop() {
    //if(BsecHasNewData(&sensorBsec))
    //    PackageData = BsecData();

    unsigned long time_trigger = millis();
    if (sensorBsec.run()) { // If new data is available
        String output;
        output = String(time_trigger);
        output += ", " + String(sensorBsec.rawTemperature);
        output += ", " + String(sensorBsec.pressure);
        output += ", " + String(sensorBsec.rawHumidity);
        output += ", " + String(sensorBsec.gasResistance);
        output += ", " + String(sensorBsec.iaq);
        output += ", " + String(sensorBsec.iaqAccuracy);
        output += ", " + String(sensorBsec.temperature);
        output += ", " + String(sensorBsec.humidity);
        output += ", " + String(sensorBsec.staticIaq);
        output += ", " + String(sensorBsec.co2Equivalent);
        output += ", " + String(sensorBsec.breathVocEquivalent);
        Serial.println(output);
    } else {
      //Serial.println(checkBsecStatus(&sensorBsec));
    }
}
