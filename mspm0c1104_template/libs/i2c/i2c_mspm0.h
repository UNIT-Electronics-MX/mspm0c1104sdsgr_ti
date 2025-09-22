#ifndef I2C_MSPM0_H
#define I2C_MSPM0_H

#include <stdint.h>

// Function declarations for I2C communication adapted to MSPM0C1104

/**
 * @brief I2C transmit function for SSD1306 communication
 * @param devAddress: 7-bit I2C device address
 * @param memAddress: Register/command address (0x00 for command, 0x40 for data)
 * @param pData: Pointer to data to send
 * @param len: Length of data in bytes
 */
void APP_I2C_Transmit(uint8_t devAddress, uint8_t memAddress, uint8_t *pData, uint16_t len);

/**
 * @brief Write command to SSD1306
 * @param command: Command byte to send
 */
void SSD1306_WriteCommand(uint8_t command);

/**
 * @brief Write data to SSD1306
 * @param data: Data byte to send
 */
void SSD1306_WriteData(uint8_t data);

/**
 * @brief Write single byte to I2C device
 * @param device_addr: 7-bit I2C device address
 * @param reg_addr: Register address
 * @param data: Data byte to write
 * @return 0 on success
 */
uint8_t i2c_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

/**
 * @brief Write multiple bytes to I2C device
 * @param device_addr: 7-bit I2C device address
 * @param reg_addr: Register address
 * @param data: Pointer to data array
 * @param len: Number of bytes to write
 * @return 0 on success
 */
uint8_t i2c_write_bulk(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

#endif // I2C_MSPM0_H
