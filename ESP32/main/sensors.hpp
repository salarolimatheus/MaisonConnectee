#pragma once

#include "bsec.h"   // bsec is a Hardware Layer
#include "PackageError.h" // Package is a Hardware Layer

#ifndef ESP_PLATFORM
  #error "Ce croquis doit être compilé pour le type de carte 'ESP32'"
#endif

/*
 * @brief Function to Configure Bsec bme680 sensor
 * @param sensorBsec : Bsec class from Bosch bsec.h
 * @param spi_bus    : Selection of SPI bus (HSPI, VSPI)
 */
bool ConfigureBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus);

/*
 * @brief Function to Check if Bsec and bme680 are working correctly
 * @param sensorBsec : Bsec class from Bosch bsec.h
 */
bool checkBsecStatus(Bsec* sensorBsec);

/*
 * @brief Function to retry the connection with Bsec sensor and
 * prepare a package error to send to the host via WiFi
 * @param debugMode : (Enter 'debug mode')
 */
PackageError AnalyseAndRetryBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus);
