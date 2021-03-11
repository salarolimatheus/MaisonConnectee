#pragma once

#include "bsec.h"     // bsec is a Hardware Layer
#include "Packages.h" // Package is a Hardware Layer
#include <EEPROM.h>   // E2PROM is a Hardware Layer

#ifndef ESP_PLATFORM
  #error "Ce croquis doit être compilé pour le type de carte 'ESP32'"
#endif

#define STATE_SAVE_PERIOD	UINT32_C(360 * 60 * 1000) // 360 minutes - 4 times a day
/*
 * @brief Bsec bme680 sensors status
 * @arg SENSOR_ALIVE : Bsec is working fine
 * @arg NEW_DATA     : New Data available to get
 * @arg SENSOR_ERROR : Bsec has an Error
 */
enum SensorStatus {
    SENSOR_ALIVE = 0,
    NEW_DATA = 1,
    SENSOR_ERROR = 2
};

/* Configure the BSEC library with information about the sensor
    18v/33v = Voltage at Vdd. 1.8V or 3.3V
    3s/300s = BSEC operating mode, BSEC_SAMPLE_RATE_LP or BSEC_SAMPLE_RATE_ULP
    4d/28d = Operating age of the sensor in days
    generic_18v_3s_4d
    generic_18v_3s_28d
    generic_18v_300s_4d
    generic_18v_300s_28d
    generic_33v_3s_4d
    generic_33v_3s_28d
    generic_33v_300s_4d
    generic_33v_300s_28d
*/
const uint8_t bsec_config_iaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

/*
 * @brief Function to Configure Bsec bme680 sensor
 * @param sensorBsec : Bsec class from Bosch bsec.h
 * @param spi_bus    : Selection of SPI bus (HSPI, VSPI)
 */
SensorStatus ConfigureBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus);

/*
 * @brief Function to Check if Bsec and bme680 are working correctly
 * @param sensorBsec : Bsec class from Bosch bsec.h
 */
SensorStatus checkBsecStatus(Bsec* sensorBsec);

/*
 * @brief Function to retry the connection with Bsec sensor and
 * prepare a package error to send to the host via WiFi
 * @param debugMode : (Enter 'debug mode')
 */
PackageError AnalyseAndRetryBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus);

/*
 * @brief
 * @param sensorBsec : Bsec class from Bosch bsec.h
 */
SensorStatus BsecHasNewData(Bsec* sensorBsec, PackageDataBsec* packageDataBsec);

void loadState(Bsec* sensorBsec);
void updateState(Bsec* sensorBsec);
