#include "WiFiManager.h"

// ********************************************************************************************************************
// PUBLIC METHODS
// ********************************************************************************************************************
WiFiManager::WiFiManager() { }
void WiFiManager::startConnection() {
    Credentials credentials = getCredentials();

    WiFi.mode(WIFI_STA);

    if (connectWifi(credentials.ssid, credentials.pass) != WL_CONNECTED) {
        startAccessPoint();
    }
}
void WiFiManager::retryConnection() {
    static uint8_t retries = 0;
    Credentials credentials = getCredentials();

    int status = connectWifi(credentials.ssid, credentials.pass);
    if ((status != WL_CONNECTED) && (retries == _retryMaxAttempt)) {
        startAccessPoint();
        retries = 0;
    } else if (status == WL_CONNECTED) retries = 0;
    else retries++;
}

void WiFiManager::setConnectTimeout(unsigned long seconds) {
    _connectTimeout = seconds * 1000;
}

// ********************************************************************************************************************
// PRIVATE METHODS
// ********************************************************************************************************************

int WiFiManager::connectWifi(String ssid, String pass) {
    if (WiFi.status() == WL_CONNECTED)
        return WL_CONNECTED;

    if (ssid != "") {
        WiFi.begin(ssid.c_str(), pass.c_str());
    } else {
        if (WiFi.SSID()) {
            esp_wifi_disconnect();
            WiFi.begin();
        }
    }

    int connRes = waitForConnectResult();
    return connRes;
}
uint8_t WiFiManager::waitForConnectResult() {
  if (_connectTimeout == 0) {
    return WiFi.waitForConnectResult();
  } else {
    unsigned long start = millis();
    boolean keepConnecting = true;
    uint8_t status;
    while (keepConnecting) {
      status = WiFi.status();
      if (millis() > start + _connectTimeout) {
        keepConnecting = false;
      }
      if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
        keepConnecting = false;
      }
      delay(100);
    }
    return status;
  }
}

void WiFiManager::startAccessPoint() {
    WiFi.mode(WIFI_AP_STA);
    uint16_t chipID = ESP.getEfuseMac();
    String chipIdString = "device-" + String(chipID, HEX);
    WiFi.softAP(chipIdString.c_str(), NULL);
    delay(100);
    WiFi.softAPConfig(IPAddress(192, 200, 1, 1), IPAddress(192, 200, 1, 1), IPAddress(255, 255, 255, 0));

    Credentials credentials;
    bool newCredentials = false;

    AsyncWebServer server(80);
    server.on("/", HTTP_GET, [&credentials, &newCredentials](AsyncWebServerRequest *request) {
        int paramsNr = request->params();

        for(int i = 0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            if (p->name() == "ssid") {
                credentials.ssid = p->value();
                newCredentials = true;
            } else if (p->name() == "pass") {
                credentials.pass = p->value();
            }
        }
        request->send(200, "text/plain", "ok");
    });
    server.begin();

    Credentials oldCredentials = getCredentials();
    uint32_t t_reconnect = millis();

    while(WiFi.status() != WL_CONNECTED) {
        if (t_reconnect < millis()) {
            connectWifi(oldCredentials.ssid, oldCredentials.pass);
            t_reconnect += _periodReconnect;
        }

        if (newCredentials) {
            if (connectWifi(credentials.ssid, credentials.pass) == WL_CONNECTED) {
                saveCredentials(credentials);
            }
            newCredentials = false;
        }
    }
    server.end();
    WiFi.mode(WIFI_STA);
}

void WiFiManager::saveCredentials(struct Credentials &credentials) {
    Preferences preferences;
    preferences.begin("credentials", false);
    preferences.clear();
    preferences.putString("ssid", credentials.ssid);
    preferences.putString("pass", credentials.pass);
    preferences.end();
}
Credentials WiFiManager::getCredentials() {
    Preferences preferences;
    Credentials credentials;
    preferences.begin("credentials", true);
    credentials.ssid = preferences.getString("ssid", "");
    credentials.pass = preferences.getString("pass", "");
    preferences.end();
    return credentials;
}
