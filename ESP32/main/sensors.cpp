#include "sensors.hpp"

bool ConfigureBsec(Bsec* sensorBsec, SPIClass* spi, const uint8_t spi_bus) {
    spi->begin();

    if (spi_bus == VSPI)        sensorBsec->begin(05, *spi);
    else if (spi_bus == HSPI)   sensorBsec->begin(15, *spi);

    if(!checkBsecStatus(sensorBsec)) return false;

    //TODO: Falta fazer os roles do bme680
    bsec_virtual_sensor_t sensorList[10] = {
      BSEC_OUTPUT_RAW_TEMPERATURE,
      BSEC_OUTPUT_RAW_PRESSURE,
      BSEC_OUTPUT_RAW_HUMIDITY,
      BSEC_OUTPUT_RAW_GAS,
      BSEC_OUTPUT_IAQ,
      BSEC_OUTPUT_STATIC_IAQ,
      BSEC_OUTPUT_CO2_EQUIVALENT,
      BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
      BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };
    sensorBsec->updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);

    return checkBsecStatus(sensorBsec);
}

bool checkBsecStatus(Bsec* sensorBsec) {
    return ((sensorBsec->status == BSEC_OK) && (sensorBsec->bme680Status == BME680_OK));
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
