/*
 * ssd1306_simple.h - Driver simplificado para SSD1306 sin buffer completo
 * Optimizado para MSPM0C1104 con memoria limitada
 */

#ifndef SSD1306_SIMPLE_H
#define SSD1306_SIMPLE_H

#include <stdint.h>
#include <stdbool.h>

// Dirección I2C del SSD1306
#define SSD1306_I2C_ADDRESS         0x3C

// Dimensiones del display
#define SSD1306_WIDTH               128
#define SSD1306_HEIGHT              64
#define SSD1306_PAGES               8

// Comandos básicos
#define SSD1306_CONTROL_CMD         0x00
#define SSD1306_CONTROL_DATA        0x40
#define SSD1306_CMD_DISPLAY_OFF     0xAE
#define SSD1306_CMD_DISPLAY_ON      0xAF
#define SSD1306_CMD_SET_CONTRAST    0x81
#define SSD1306_CMD_INVERT_OFF      0xA6
#define SSD1306_CMD_INVERT_ON       0xA7
#define SSD1306_CMD_SET_MEM_MODE    0x20
#define SSD1306_MEM_MODE_HORIZONTAL 0x00
#define SSD1306_CMD_SET_COL_ADDR    0x21
#define SSD1306_CMD_SET_PAGE_ADDR   0x22
#define SSD1306_CMD_SET_START_LINE  0x40
#define SSD1306_CMD_SET_SEG_REMAP   0xA1
#define SSD1306_CMD_SET_COM_SCAN    0xC8
#define SSD1306_CMD_SET_COM_PINS    0xDA
#define SSD1306_CMD_SET_OSC_FREQ    0xD5
#define SSD1306_CMD_SET_PRECHARGE   0xD9
#define SSD1306_CMD_SET_VCOM_LEVEL  0xDB
#define SSD1306_CMD_CHARGE_PUMP     0x8D

// Estados
typedef enum {
    SSD1306_OK = 0,
    SSD1306_ERROR
} ssd1306_result_t;

// Funciones básicas
ssd1306_result_t ssd1306_init_simple(void);
ssd1306_result_t ssd1306_clear_screen(void);
ssd1306_result_t ssd1306_print_text(uint8_t page, const char *text);
ssd1306_result_t ssd1306_send_command(uint8_t cmd);
ssd1306_result_t ssd1306_send_data_byte(uint8_t data);
ssd1306_result_t ssd1306_set_cursor(uint8_t page, uint8_t col);

#endif // SSD1306_SIMPLE_H
