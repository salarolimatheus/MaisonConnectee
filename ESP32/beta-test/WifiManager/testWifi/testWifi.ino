#include "WiFiManager.h" // Communication is a Control Layer

void setup() {
    Serial.begin(115200);
    while(!Serial);
    WiFiManager wifi;
    wifi.startConnection();
}

void loop() {
   if(WiFi.status() != WL_CONNECTED) {
       WiFiManager wifi;
       wifi.retryConnection();
   }
}
