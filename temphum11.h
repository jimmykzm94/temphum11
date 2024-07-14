/*!
 * \file temphum11.h
 * \brief Temp Hum 11 library header
 * \author Kueh Zi Ming
 * \date 14/07/2024
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#include <cstdint>
#else
#include <stdint.h>
#endif


// Slave address
#define TEMPHUM11_DEVICE_SLAVE_ADDR 0x40

// Registers
#define TEMPHUM11_REG_TEMPERATURE 0x00
#define TEMPHUM11_REG_HUMIDITY 0x01
#define TEMPHUM11_REG_CONFIGURATION 0x02
#define TEMPHUM11_REG_SERIAL_ID_0 0xFB
#define TEMPHUM11_REG_SERIAL_ID_1 0xFC
#define TEMPHUM11_REG_SERIAL_ID_2 0xFD
#define TEMPHUM11_REG_MANUFACTURER_ID 0xFE
#define TEMPHUM11_REG_DEVICE_ID 0xFF

// Configuration
#define TEMPHUM11_NORMAL_OPERATION 0x0000
#define TEMPHUM11_SOFTWARE_RESET 0x8000
#define TEMPHUM11_HEATER_DISABLED 0x0000
#define TEMPHUM11_HEATER_ENABLED 0x2000
#define TEMPHUM11_TEMP_HUM_ACQUIRED 0x0000
#define TEMPHUM11_TEMP_FIRST 0x1000
#define TEMPHUM11_BAT_VOLT_BIGGER_THAN_2p8 0x0000
#define TEMPHUM11_BAT_VOLT_SMALLER_THAN_2p8 0x0800
#define TEMPHUM11_TEMP_RESOLUTION_14bit 0x0000
#define TEMPHUM11_TEMP_RESOLUTION_11bit 0x0400
#define TEMPHUM11_HUM_RESOLUTION_14bit 0x0000
#define TEMPHUM11_HUM_RESOLUTION_11bit 0x0100
#define TEMPHUM11_HUM_RESOLUTION_8bit 0x0200

// Return value
#define TEMPHUM11_DEVICE_ID 0x1050
#define TEMPHUM11_MANUFACTURER_ID 0x5449

// Temperature data in
#define TEMPHUM11_TEMP_IN_CELSIUS 0x00
#define TEMPHUM11_TEMP_IN_KELVIN 0x01
#define TEMPHUM11_TEMP_IN_FAHRENHEIT 0x02

/// @brief Open connection
/// @param sda SDA pin
/// @param scl SCL pin
/// @param i2c_port I2C port to use (Start from 0)
void tempHum11_open(uint8_t sda, uint8_t scl, uint8_t i2c_port);

/// @brief Set configurations
/// @param config Configuration bits
void tempHum11_writeConfig(uint16_t config);

/// @brief Set default sensor configuration
void tempHum11_setDefaultCfg();

/// @brief Read data from register
/// @param reg Register to read
/// @return Data in 16bits
uint16_t tempHum11_readData(uint8_t reg);

/// @brief Get temperature
/// @param temp_in Temperature type: celcius, fahrenheit, kelvin
/// @return Temperature
float tempHum11_getTemperature(uint8_t temp_in);

/// @brief Get humidity
/// @return Humidity
float tempHum11_getHumidity();

/// @brief Get temperature and humidity
/// @param temp Temperature data
/// @param hum Humidity data
void tempHum11_getTempAndHum(float *temp, float *hum);

/// @brief Get device ID
/// @return Device ID
uint16_t tempHum11_getDeviceId();

/// @brief Get manufactured ID
/// @return Manufactured ID
uint16_t tempHum11_getManufactureId();

/// @brief  Set sensor data in mode
/// @param mode Mode
/// @note 0 - Use separate temperature and humidity functions
/// @note 1 - Use one function to read both temperature and humidity
void tempHum11_setDataInMode(uint8_t mode);

#ifdef __cplusplus
}
#endif
