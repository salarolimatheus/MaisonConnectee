#pragma once

#include "Arduino.h"
#include "Packages.h"

#ifndef ESP_PLATFORM
  #error "Ce croquis doit être compilé pour le type de carte 'ESP32'"
#endif

/*
 * @brief Function to Configure the Serial connexion
 * @param baud : Sets Serial's data rate in bits per second (baud)
 */
void ConfigureSerialDebug(const unsigned long baud);

void PrintBsecData(PackageDataBsec* packageDataBsec);
