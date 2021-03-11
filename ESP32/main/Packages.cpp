#include "Packages.h"

// PackageError ------------------------------------------------------
PackageError::PackageError(String topic) {
    packageTopic = topic;
}

void PackageError::setText(String text) {
    packageContent = text;
}

void PackageError::addContent(String label, String value) {
    packageContent += label +":" + value + ",";
}

String PackageError::getText() {
    return packageContent;
}
// PackageDataBsec ---------------------------------------------------

PackageDataBsec::PackageDataBsec(String topic) {
    packageTopic = topic;
}

void PackageDataBsec::getData(float staticIaq, uint8_t iaqAccuracy, float temperature, float humidity) {
    _staticIaq = staticIaq;
    _iaqAccuracy = iaqAccuracy;
    _temperature = temperature;
    _humidity = humidity;
}

String PackageDataBsec::toString(void) {
    String output = String(_staticIaq);
    output += ", " + String(_iaqAccuracy);
    output += ", " + String(_temperature);
    output += ", " + String(_humidity);
    return output;
}
