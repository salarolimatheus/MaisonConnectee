#include "WiFiManager.h"

WiFiManager::WiFiManager() {
}

boolean WiFiManager::autoConnect(char const *apName, char const *apPassword) {
    // read eeprom for ssid and pass
    //String ssid = getSSID();
    //String pass = getPassword();

    // attempt to connect; should it fail, fall back to AP
    WiFi.mode(WIFI_STA);

    if (connectWifi("", "") == WL_CONNECTED) {
        //connected
        return true;
    }

    return startConfigPortal(apName, apPassword);
}
boolean  WiFiManager::startConfigPortal(char const *apName, char const *apPassword) {
    //setup AP
    WiFi.mode(WIFI_AP_STA);

    _apName = apName;
    _apPassword = apPassword;

    //notify we entered AP mode
    if ( _apcallback != NULL) {
        _apcallback(this);
    }

    connect = false;
    setupConfigPortal();

    while(1) {
        if(configPortalHasTimeout()) break; // check if timeout
        dnsServer->processNextRequest(); //DNS
        server->handleClient(); //HTTP

        if (connect) {
            connect = false;
            delay(2000);

            // using user-provided  _ssid, _pass in place of system-stored ssid and pass
            if (connectWifi(_ssid, _pass) != WL_CONNECTED) {
                DEBUG_WM(F("Failed to connect."));
            } else {
                //connected
                WiFi.mode(WIFI_STA);
                //notify that configuration has changed and any optional parameters should be saved
                if ( _savecallback != NULL) {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }
                break;
            }

            if (_shouldBreakAfterConfig) {
                //flag set to exit after config after trying to connect
                //notify that configuration has changed and any optional parameters should be saved
                if ( _savecallback != NULL) {
                    //todo: check if any custom parameters actually exist, and check if they really changed maybe
                    _savecallback();
                }
                break;
            }
        }
        yield();
    }

    server.reset();
    dnsServer.reset();

    return  WiFi.status() == WL_CONNECTED;
}
void WiFiManager::setupConfigPortal() {
    dnsServer.reset(new DNSServer());
    server.reset(new WebServer(80));

    _configPortalStart = millis();

    if (_apPassword != NULL) {
        if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63)
            _apPassword = NULL;
    }

    if (_ap_static_ip) //optional soft ip config
        WiFi.softAPConfig(_ap_static_ip, _ap_static_gw, _ap_static_sn);

    if (_apPassword != NULL) WiFi.softAP(_apName, _apPassword);
    else WiFi.softAP(_apName);

    delay(500); // Without delay I've seen the IP address blank

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

    /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
    server->on("/", std::bind(&WiFiManager::handleRoot, this));
    server->onNotFound(std::bind(&WiFiManager::handleNotFound, this));

    server->begin(); // Web server start
}

boolean WiFiManager::configPortalHasTimeout() {
    if(_configPortalTimeout == 0){  // TODO
        _configPortalStart = millis(); // kludge, bump configportal start time to skew timeouts
        return false;
    }
    return (millis() > _configPortalStart + _configPortalTimeout);
}

int WiFiManager::connectWifi(String ssid, String pass) {
    DEBUG_WM(F("Connecting as wifi client..."));

    // check if we've got static_ip settings, if we do, use those.
    if (_sta_static_ip) {
        DEBUG_WM(F("Custom STA IP/GW/Subnet"));
        WiFi.config(_sta_static_ip, _sta_static_gw, _sta_static_sn);
        DEBUG_WM(WiFi.localIP());
    }
    //fix for auto connect racing issue
    if (WiFi.status() == WL_CONNECTED) {
        DEBUG_WM("Already connected. Bailing out.");
        return WL_CONNECTED;
    }
    //check if we have ssid and pass and force those, if not, try with last saved values
    if (ssid != "") {
        WiFi.begin(ssid.c_str(), pass.c_str());
    } else {
        if (WiFi.SSID()) {
            DEBUG_WM("Using last saved values, should be faster");
            #if defined(ESP8266)
            //trying to fix connection in progress hanging
            ETS_UART_INTR_DISABLE();
            wifi_station_disconnect();
            ETS_UART_INTR_ENABLE();
            #else
            esp_wifi_disconnect();
            #endif

            WiFi.begin();
        } else {
            DEBUG_WM("No saved credentials");
        }
    }

    int connRes = waitForConnectResult();
    DEBUG_WM ("Connection result: ");
    DEBUG_WM ( connRes );
    //not connected, WPS enabled, no pass - first attempt
    if (_tryWPS && connRes != WL_CONNECTED && pass == "") {
        startWPS();
        //should be connected at the end of WPS
        connRes = waitForConnectResult();
    }
    return connRes;
}

uint8_t WiFiManager::waitForConnectResult() {
    if (_connectTimeout == 0) {
        return WiFi.waitForConnectResult();
    } else {
        DEBUG_WM (F("Waiting for connection result with time out"));
        unsigned long start = millis();
        boolean keepConnecting = true;
        uint8_t status;
        while (keepConnecting) {
            status = WiFi.status();
            if (millis() > start + _connectTimeout) {
                keepConnecting = false;
                DEBUG_WM (F("Connection timed out"));
            }
            if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
                keepConnecting = false;
            }
            delay(100);
        }
        return status;
    }
}

void WiFiManager::startWPS() {
    #if defined(ESP8266)
    DEBUG_WM("START WPS");
    WiFi.beginWPSConfig();
    DEBUG_WM("END WPS");
    #else
    // TODO
    DEBUG_WM("ESP32 WPS TODO");
    #endif
}

String WiFiManager::getSSID() {
    if (_ssid == "") {
        DEBUG_WM(F("Reading SSID"));
        _ssid = WiFi.SSID();
        DEBUG_WM(F("SSID: "));
        DEBUG_WM(_ssid);
    }
    return _ssid;
}
String WiFiManager::getPassword() {
    if (_pass == "") {
        DEBUG_WM(F("Reading Password"));
        _pass = WiFi.psk();
        DEBUG_WM("Password: " + _pass);
        //DEBUG_WM(_pass);
    }
    return _pass;
}
String WiFiManager::getConfigPortalSSID() {
    return _apName;
}
void WiFiManager::resetSettings() {
    DEBUG_WM(F("settings invalidated"));
    DEBUG_WM(F("THIS MAY CAUSE AP NOT TO START UP PROPERLY. YOU NEED TO COMMENT IT OUT AFTER ERASING THE DATA."));
    // TODO On ESP32 this does not erase the SSID and password. See
    // https://github.com/espressif/arduino-esp32/issues/400
    // For now, use "make erase_flash".
    WiFi.disconnect(true);
    //delay(200);
}
void WiFiManager::setTimeout(unsigned long seconds) {
    setConfigPortalTimeout(seconds);
}

void WiFiManager::setConfigPortalTimeout(unsigned long seconds) {
    _configPortalTimeout = seconds * 1000;
}

void WiFiManager::setConnectTimeout(unsigned long seconds) {
    _connectTimeout = seconds * 1000;
}

void WiFiManager::setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
    _ap_static_ip = ip;
    _ap_static_gw = gw;
    _ap_static_sn = sn;
}
void WiFiManager::setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn) {
    _sta_static_ip = ip;
    _sta_static_gw = gw;
    _sta_static_sn = sn;
}
void WiFiManager::setMinimumSignalQuality(int quality) {
    _minimumQuality = quality;
}
void WiFiManager::setBreakAfterConfig(boolean shouldBreak) {
    _shouldBreakAfterConfig = shouldBreak;
}

/** Handle root or redirect to captive portal */
void WiFiManager::handleRoot() {
    DEBUG_WM(F("Handle root"));
    if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
        return;
    }

    String page = FPSTR(HTTP_HEAD);
    page.replace("{v}", "Options");
    page += FPSTR(HTTP_SCRIPT);
    page += FPSTR(HTTP_STYLE);
    page += _customHeadElement;
    page += FPSTR(HTTP_HEAD_END);
    page += "<h1>";
    page += _apName;
    page += "</h1>";
    page += F("<h3>WiFiManager</h3>");
    page += FPSTR(HTTP_PORTAL_OPTIONS);
    page += FPSTR(HTTP_END);

    server->sendHeader("Content-Length", String(page.length()));
    server->send(200, "text/html", page);

}

void WiFiManager::handleNotFound() {
    if (captivePortal()) { // If captive portal redirect instead of displaying the error page.
        return;
    }
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server->uri();
    message += "\nMethod: ";
    message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server->args();
    message += "\n";

    for ( uint8_t i = 0; i < server->args(); i++ ) {
        message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
    }
    server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server->sendHeader("Pragma", "no-cache");
    server->sendHeader("Expires", "-1");
    server->sendHeader("Content-Length", String(message.length()));
    server->send ( 404, "text/plain", message );
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean WiFiManager::captivePortal() {
    if (!isIp(server->hostHeader()) ) {
        DEBUG_WM(F("Request redirected to captive portal"));
        server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
        server->send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
        server->client().stop(); // Stop is needed because we sent no content length
        return true;
    }
    return false;
}

//start up config portal callback
void WiFiManager::setAPCallback( void (*func)(WiFiManager* myWiFiManager) ) {
    _apcallback = func;
}

//start up save config callback
void WiFiManager::setSaveConfigCallback( void (*func)(void) ) {
    _savecallback = func;
}

//sets a custom element to add to head, like a new style tag
void WiFiManager::setCustomHeadElement(const char* element) {
    _customHeadElement = element;
}

//if this is true, remove duplicated Access Points - defaut true
void WiFiManager::setRemoveDuplicateAPs(boolean removeDuplicates) {
    _removeDuplicateAPs = removeDuplicates;
}

int WiFiManager::getRSSIasQuality(int RSSI) {
    int quality = 0;

    if (RSSI <= -100) {
        quality = 0;
    } else if (RSSI >= -50) {
        quality = 100;
    } else {
        quality = 2 * (RSSI + 100);
    }
    return quality;
}

/** Is this an IP? */
boolean WiFiManager::isIp(String str) {
    for (int i = 0; i < str.length(); i++) {
        int c = str.charAt(i);
        if (c != '.' && (c < '0' || c > '9')) {
            return false;
        }
    }
    return true;
}

/** IP to String? */
String WiFiManager::toStringIp(IPAddress ip) {
    String res = "";
    for (int i = 0; i < 3; i++) {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }
    res += String(((ip >> 8 * 3)) & 0xFF);
    return res;
}
