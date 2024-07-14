#include "temphum11.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef *hi2c; // do not change the variable name
static uint8_t data_mode = 0;

void tempHum11_open(uint8_t sda, uint8_t scl, uint8_t i2c_port)
{
    // not applicable
}

void tempHum11_writeConfig(uint16_t config)
{
    uint8_t write_reg[3];

    write_reg[0] = TEMPHUM11_REG_CONFIGURATION;
    write_reg[1] = (uint8_t)(config >> 8);
    write_reg[2] = (uint8_t)(config);

    HAL_I2C_Master_Transmit(hi2c, TEMPHUM11_DEVICE_SLAVE_ADDR << 1, write_reg, 3, 1000);
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
    HAL_StatusTypeDef status = HAL_OK;

    HAL_I2C_Master_Transmit(hi2c, TEMPHUM11_DEVICE_SLAVE_ADDR << 1, &reg, 1, 1000);
    HAL_Delay(10);
    status = HAL_I2C_Master_Receive(hi2c, TEMPHUM11_DEVICE_SLAVE_ADDR << 1, read_reg, 2, 1000);
    if (status != HAL_OK)
        return 0;

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
    uint16_t read_data = 0;
    HAL_StatusTypeDef status = HAL_OK;

    HAL_I2C_Master_Transmit(hi2c, TEMPHUM11_DEVICE_SLAVE_ADDR << 1, 0x00, 1, 1000);
    HAL_Delay(15);
    status = HAL_I2C_Master_Receive(hi2c, TEMPHUM11_DEVICE_SLAVE_ADDR << 1, read_reg, 4, 1000);
    if (status != HAL_OK)
        return;

    read_data = ((uint16_t)read_reg[0]) << 8 | read_reg[1];
    *temperature = (float)(read_data / 65536.0) * 165.0 - 40;

    read_data = ((uint16_t)read_reg[2]) << 8 | read_reg[3];
    *humidity = (float)(read_data / 65536.0) * 100.0;
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
