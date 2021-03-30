// main.ino is the Main Logical Layer
#include "sensors.hpp"       // Sensors is a Control Layer
#include "debug.hpp"         // Debug is a Control Layer
#include "communication.hpp" // Communication is a Control Layer

const bool debugMode = true;
const unsigned long SERIAL_BAUD_RATE = 115200;
const String SUBSCRIPTION_TOPIC_BSEC = "bsec";

Bsec sensorBsec;
SPIClass mySPI(VSPI);

void setup() {
    if (debugMode)
        ConfigureSerialDebug(SERIAL_BAUD_RATE);

    ConfigureWiFi();

    if (!ConfigureBsec(&sensorBsec, &mySPI, VSPI)) {
        PackageError packageError = AnalyseAndRetryBsec(&sensorBsec, &mySPI, VSPI);
        Serial.print(packageError.getText());
    }
}

void loop() {
    static PackageDataBsec packageDataBsec = PackageDataBsec(SUBSCRIPTION_TOPIC_BSEC);
    SensorStatus status = BsecHasNewData(&sensorBsec, &packageDataBsec);
    if (status == NEW_DATA)
        PrintBsecData(&packageDataBsec);
    else if (status == SENSOR_ERROR)
        PackageError packageError = AnalyseAndRetryBsec(&sensorBsec, &mySPI, VSPI);

    checkWiFi();
}
