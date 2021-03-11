#include "sensors.hpp"

SensorStatus ConfigureBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus) {
    EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1);
    spi->begin();

    if (spi_bus == VSPI)        sensorBsec->begin(05, *spi);
    else if (spi_bus == HSPI)   sensorBsec->begin(15, *spi);

    if(checkBsecStatus(sensorBsec) == SENSOR_ERROR) return SENSOR_ERROR;

    sensorBsec->setConfig(bsec_config_iaq);

    if(checkBsecStatus(sensorBsec) == SENSOR_ERROR) return SENSOR_ERROR;

    loadState(sensorBsec);

    bsec_virtual_sensor_t sensorList[8] = {
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,

      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,

      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,  // if BSEC_SAMPLE_RATE_LP is used
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,     // if BSEC_SAMPLE_RATE_LP is used
    };

    sensorBsec->updateSubscription(sensorList, 8, BSEC_SAMPLE_RATE_LP); //BSEC_SAMPLE_RATE_ULP ultra low power

    return checkBsecStatus(sensorBsec);
}

SensorStatus checkBsecStatus(Bsec* sensorBsec) {
    if((sensorBsec->status == BSEC_OK) && (sensorBsec->bme680Status == BME680_OK)) return SENSOR_ALIVE;
    else return SENSOR_ERROR;
}

PackageError AnalyseAndRetryBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus) {
    *sensorBsec = Bsec();
    delay(50);
    PackageError packageError = PackageError("BSEC-error");
    if(ConfigureBsec(sensorBsec, spi, spi_bus)) {
        packageError.setText("Retry worked");
    } else {
        packageError.addContent("BSEC", String(sensorBsec->status));
        packageError.addContent("BME680", String(sensorBsec->bme680Status));
    }
    return packageError;
}

SensorStatus BsecHasNewData(Bsec* sensorBsec, PackageDataBsec* packageDataBsec) {
    if ((sensorBsec->run())) { // If new data is available
        packageDataBsec->getData(sensorBsec->staticIaq, sensorBsec->iaqAccuracy, sensorBsec->temperature, sensorBsec->humidity);
        return NEW_DATA;
    } else {
      return checkBsecStatus(sensorBsec);
    }
}

void loadState(Bsec* sensorBsec) {
    if (EEPROM.read(0) == BSEC_MAX_STATE_BLOB_SIZE) {
        uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
        for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++) {
            bsecState[i] = EEPROM.read(i + 1);
        }
        sensorBsec->setState(bsecState);
        checkBsecStatus(sensorBsec);
    } else {
        for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE + 1; i++)
        EEPROM.write(i, 0);
        EEPROM.commit();
    }
}

void updateState(Bsec* sensorBsec) {
    bool update = false;
    static uint16_t stateUpdateCounter = 0;

    if (stateUpdateCounter == 0) {
        if (sensorBsec->iaqAccuracy >= 3) {
            update = true;
            stateUpdateCounter++;
        }
    } else {
        if ((stateUpdateCounter * STATE_SAVE_PERIOD) < millis()) {
            update = true;
            stateUpdateCounter++;
        }
    }

    if (update) {
        uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE] = {0};
        sensorBsec->getState(bsecState);
        checkBsecStatus(sensorBsec);
        for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE ; i++) {
            EEPROM.write(i + 1, bsecState[i]);
            Serial.println(bsecState[i], HEX);
        }
        EEPROM.write(0, BSEC_MAX_STATE_BLOB_SIZE);
        EEPROM.commit();
    }
}
