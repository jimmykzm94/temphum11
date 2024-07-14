#include "temphum11.h"
#include "Wire.h"

static TwoWire *_wire = NULL;
static uint8_t data_mode = 0;

void tempHum11_open(uint8_t sda, uint8_t scl, uint8_t i2c_port)
{
    if (i2c_port == 0)
    {
        // Wire1 is used
        _wire = &Wire;
    }
    else
    {
        // Wire2 is used
        _wire = &Wire1;
    }
    _wire->begin(sda, scl, 100000);
}

void tempHum11_writeConfig(uint16_t config)
{
    uint8_t write_reg[3];

    write_reg[0] = TEMPHUM11_REG_CONFIGURATION;
    write_reg[1] = (uint8_t)(config >> 8);
    write_reg[2] = (uint8_t)(config);

    _wire->beginTransmission(TEMPHUM11_DEVICE_SLAVE_ADDR);
    for (uint8_t i = 0; i < 3; i++)
    {
        _wire->write(write_reg[i]);
    }
    _wire->endTransmission();
}

void tempHum11_setDefaultCfg()
{
    tempHum11_writeConfig(TEMPHUM11_NORMAL_OPERATION | TEMPHUM11_HEATER_DISABLED |
                          TEMPHUM11_TEMP_HUM_ACQUIRED | TEMPHUM11_TEMP_RESOLUTION_14bit |
                          TEMPHUM11_HUM_RESOLUTION_14bit);
}

uint16_t tempHum11_readData(uint8_t reg)
{
    uint8_t read_reg[2] = {0};
    uint16_t read_data = 0;

    _wire->beginTransmission(TEMPHUM11_DEVICE_SLAVE_ADDR);
    _wire->write(reg);
    _wire->endTransmission();

    delay(10);

    _wire->requestFrom(TEMPHUM11_DEVICE_SLAVE_ADDR, 2U);
    _wire->readBytes(read_reg, 2);

    read_data = read_reg[0] << 8;
    read_data = read_data | read_reg[1];

    return read_data;
}

float tempHum11_getTemperature(uint8_t temp_in)
{
    uint16_t temp_out = UINT16_MAX;
    float temperature;

    temp_out = tempHum11_readData(TEMPHUM11_REG_TEMPERATURE);
    temperature = (float)(temp_out / 65536.0) * 165.0 - 40;

    if (temp_in == TEMPHUM11_TEMP_IN_KELVIN)
    {
        temperature = temperature + 273.15;
    }
    else if (temp_in == TEMPHUM11_TEMP_IN_FAHRENHEIT)
    {
        temperature = (temperature * 9.0 / 5.0) + 32;
    }

    return temperature;
}

float tempHum11_getHumidity()
{
    uint16_t hum_out = UINT16_MAX;
    float humidity;

    hum_out = tempHum11_readData(TEMPHUM11_REG_HUMIDITY);
    humidity = (float)(hum_out / 65536.0) * 100.0;

    return humidity;
}

void tempHum11_getTempAndHum(float *temp, float *hum)
{
    uint8_t read_reg[4] = {0};
    uint32_t read_data = 0;

    if (data_mode == 1)
    {
        _wire->beginTransmission(TEMPHUM11_DEVICE_SLAVE_ADDR);
        _wire->write((uint8_t)TEMPHUM11_REG_TEMPERATURE);
        _wire->endTransmission();

        delay(15);

        _wire->requestFrom(TEMPHUM11_DEVICE_SLAVE_ADDR, 4U);
        _wire->readBytes(read_reg, 4);

        *temp = (float)((uint16_t)(read_reg[0] << 8 | read_reg[1]) / 65536.0) * 165.0 - 40;
        *hum = (float)((uint16_t)(read_reg[2] << 8 | read_reg[3]) / 65536.0) * 100.0;
    }
    else
    {
        // data mode is not set, so return 0
        *temp = 0;
        *hum = 0;
    }
}

uint16_t tempHum11_getDeviceId()
{
    uint16_t res = tempHum11_readData(TEMPHUM11_REG_DEVICE_ID);
    return res;
}

uint16_t tempHum11_getManufactureId()
{
    uint16_t res = tempHum11_readData(TEMPHUM11_REG_MANUFACTURER_ID);
    return res;
}

void tempHum11_setDataInMode(uint8_t mode)
{
    if (mode == 0)
    {
        // temp or hum
        tempHum11_writeConfig(TEMPHUM11_TEMP_HUM_ACQUIRED);
        data_mode = 0;
    }
    else if (mode == 1)
    {
        // temp and hum
        tempHum11_writeConfig(TEMPHUM11_TEMP_FIRST);
        data_mode = 1;
    }
}
