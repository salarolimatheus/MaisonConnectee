#ifndef WiFiManager_h
#define WiFiManager_h

#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Preferences.h>
#include <esp_wifi.h>

struct Credentials {
    String ssid = "";
    String pass = "";
};

class WiFiManager {
    public:
        WiFiManager();

        /*
        * @brief Function to connect to a defined Access Point
        * @param apName : Access Point Name
        * @param apName : Access Point Password
        */
        void startConnection(void);
        void retryConnection(void);

        void setConnectTimeout(unsigned long seconds);
        // void setConfig(IPAddress);
    private:
        unsigned long _connectTimeout = 3000;
        unsigned long _periodReconnect = 8000;
        uint8_t _retryMaxAttempt = 166;

        int connectWifi(String ssid, String pass);
        uint8_t waitForConnectResult();
        void startAccessPoint();
        void saveCredentials(struct Credentials &credentials);
        Credentials getCredentials();
};

#endif
