#include "communication.hpp"

void ConfigureWiFi(void) {
    WiFiManager wifi;
    wifi.startConnection();
}

void checkWiFi(void) {
    if(WiFi.status() != WL_CONNECTED) {
        WiFiManager wifi;
        wifi.retryConnection();
    }
}
