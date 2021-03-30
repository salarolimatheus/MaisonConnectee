#pragma once

#include "WiFiManager.h" // WiFiManager is a Hardware Layer

#ifndef ESP_PLATFORM
  #error "Ce croquis doit être compilé pour le type de carte 'ESP32'"
#endif

void ConfigureWiFi(void);

void checkWiFi(void);
