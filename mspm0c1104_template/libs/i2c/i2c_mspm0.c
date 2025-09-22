#include "ti_msp_dl_config.h"

// I2C functions for MSPM0C1104 adapted for SSD1306

// I2C Defines for our configuration
#define I2C_CONTROLLER     I2C0
#define I2C_CONTROLLER_IRQ I2C0_INT_IRQn

void APP_I2C_Transmit(uint8_t devAddress, uint8_t memAddress, uint8_t *pData, uint16_t len)
{
    // Create buffer with register address + data
    uint8_t buffer[16]; // Max 15 bytes data + 1 register byte
    
    if (len > 15) {
        return; // Too much data for our buffer
    }
    
    // First byte is the register/command address
    buffer[0] = memAddress;
    
    // Copy data
    for (uint16_t i = 0; i < len; i++) {
        buffer[i + 1] = pData[i];
    }
    
    // Wait for I2C to be Idle
    while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    // Fill FIFO with our data
    DL_I2C_fillControllerTXFIFO(I2C_CONTROLLER, buffer, len + 1);
    
    // Start transfer (this sends start + data + stop automatically)
    DL_I2C_startControllerTransfer(I2C_CONTROLLER, devAddress, DL_I2C_CONTROLLER_DIRECTION_TX, len + 1);
    
    // Wait for transfer to complete
    while (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    
    // Check for errors
    if (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        // Handle error - for now we just return
        return;
    }
}

void SSD1306_WriteCommand(uint8_t command)
{
    APP_I2C_Transmit(0x3C, 0x00, &command, 1);  // 0x3C = 7-bit address, 0x00 = command mode
}

void SSD1306_WriteData(uint8_t data)
{
    APP_I2C_Transmit(0x3C, 0x40, &data, 1);    // 0x3C = 7-bit address, 0x40 = data mode
}

// Alternative simplified I2C write function
uint8_t i2c_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = reg_addr;
    buffer[1] = data;
    
    // Wait for I2C to be Idle
    while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    // Fill FIFO
    DL_I2C_fillControllerTXFIFO(I2C_CONTROLLER, buffer, 2);
    
    // Start transfer
    DL_I2C_startControllerTransfer(I2C_CONTROLLER, device_addr, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    
    // Wait for completion
    while (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    
    // Check for errors
    if (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        return 1; // Error
    }
    
    return 0; // Success
}

// Bulk data write for faster screen updates
uint8_t i2c_write_bulk(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    // For bulk writes, we need to handle chunks due to FIFO size limits
    const uint16_t MAX_CHUNK = 15; // Leave room for register address
    
    uint16_t offset = 0;
    while (offset < len) {
        uint16_t chunk_len = (len - offset > MAX_CHUNK) ? MAX_CHUNK : (len - offset);
        
        // Wait for I2C to be Idle
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_IDLE));
        
        // Create buffer for this chunk
        uint8_t buffer[16];
        buffer[0] = reg_addr;
        
        for (uint16_t i = 0; i < chunk_len; i++) {
            buffer[i + 1] = data[offset + i];
        }
        
        // Fill FIFO
        DL_I2C_fillControllerTXFIFO(I2C_CONTROLLER, buffer, chunk_len + 1);
        
        // Start transfer
        DL_I2C_startControllerTransfer(I2C_CONTROLLER, device_addr, DL_I2C_CONTROLLER_DIRECTION_TX, chunk_len + 1);
        
        // Wait for completion
        while (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
        
        // Check for errors
        if (DL_I2C_getControllerStatus(I2C_CONTROLLER) & DL_I2C_CONTROLLER_STATUS_ERROR) {
            return 1; // Error
        }
        
        offset += chunk_len;
        reg_addr += chunk_len; // Increment register address for next chunk
    }
    
    return 0; // Success
}
