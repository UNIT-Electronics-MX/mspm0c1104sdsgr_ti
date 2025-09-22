/*
 * ssd1306_simple.c - Implementación simplificada del driver SSD1306
 * Sin buffer completo, escritura directa al display
 */

#include "ssd1306_simple.h"
#include "ti_msp_dl_config.h"

// Font 5x7 simplificado - solo números y letras básicas
static const uint8_t font5x7_basic[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // (space) - 0
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 - 1
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1 - 2
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 - 3
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3 - 4
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4 - 5
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 - 6
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6 - 7
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 - 8
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 - 9
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9 - 10
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A - 11
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B - 12
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C - 13
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D - 14
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E - 15
    {0x7F, 0x09, 0x09, 0x01, 0x01}, // F - 16
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H - 17
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I - 18
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K - 19
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L - 20
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // M - 21
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O - 22
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P - 23
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R - 24
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S - 25
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T - 26
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // + - 27
    {0x08, 0x08, 0x08, 0x08, 0x08}, // - - 28
    {0x00, 0x36, 0x36, 0x00, 0x00}, // : - 29
};

// Mapeo de caracteres a índices del font
static uint8_t get_font_index(char c) {
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return c - '0' + 1;
    if (c >= 'A' && c <= 'Z') {
        switch (c) {
            case 'A': return 11;
            case 'B': return 12;
            case 'C': return 13;
            case 'D': return 14;
            case 'E': return 15;
            case 'F': return 16;
            case 'H': return 17;
            case 'I': return 18;
            case 'K': return 19;
            case 'L': return 20;
            case 'M': return 21;
            case 'O': return 22;
            case 'P': return 23;
            case 'R': return 24;
            case 'S': return 25;
            case 'T': return 26;
        }
    }
    if (c == '+') return 27;
    if (c == '-') return 28;
    if (c == ':') return 29;
    
    return 0; // Espacio por defecto
}

// Enviar comando al SSD1306
ssd1306_result_t ssd1306_send_command(uint8_t cmd) {
    // Iniciar transferencia I2C
    DL_I2C_startControllerTransfer(I2C_INST, SSD1306_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    
    // Enviar control byte y comando
    DL_I2C_fillControllerTXFIFO(I2C_INST, &(uint8_t){SSD1306_CONTROL_CMD}, 1);
    DL_I2C_fillControllerTXFIFO(I2C_INST, &cmd, 1);
    
    // Esperar finalización
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    
    return SSD1306_OK;
}

// Enviar un byte de datos
ssd1306_result_t ssd1306_send_data_byte(uint8_t data) {
    DL_I2C_startControllerTransfer(I2C_INST, SSD1306_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    
    DL_I2C_fillControllerTXFIFO(I2C_INST, &(uint8_t){SSD1306_CONTROL_DATA}, 1);
    DL_I2C_fillControllerTXFIFO(I2C_INST, &data, 1);
    
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    
    return SSD1306_OK;
}

// Inicializar SSD1306
ssd1306_result_t ssd1306_init_simple(void) {
    // Secuencia de inicialización mínima
    ssd1306_send_command(SSD1306_CMD_DISPLAY_OFF);
    
    ssd1306_send_command(SSD1306_CMD_SET_OSC_FREQ);
    ssd1306_send_command(0x80);
    
    ssd1306_send_command(0xA8); // Set multiplex ratio
    ssd1306_send_command(0x3F); // 64-1
    
    ssd1306_send_command(0xD3); // Set display offset
    ssd1306_send_command(0x00);
    
    ssd1306_send_command(SSD1306_CMD_SET_START_LINE | 0x00);
    
    ssd1306_send_command(SSD1306_CMD_CHARGE_PUMP);
    ssd1306_send_command(0x14);
    
    ssd1306_send_command(SSD1306_CMD_SET_MEM_MODE);
    ssd1306_send_command(SSD1306_MEM_MODE_HORIZONTAL);
    
    ssd1306_send_command(SSD1306_CMD_SET_SEG_REMAP);
    ssd1306_send_command(SSD1306_CMD_SET_COM_SCAN);
    
    ssd1306_send_command(SSD1306_CMD_SET_COM_PINS);
    ssd1306_send_command(0x12);
    
    ssd1306_send_command(SSD1306_CMD_SET_CONTRAST);
    ssd1306_send_command(0x7F);
    
    ssd1306_send_command(SSD1306_CMD_SET_PRECHARGE);
    ssd1306_send_command(0xF1);
    
    ssd1306_send_command(SSD1306_CMD_SET_VCOM_LEVEL);
    ssd1306_send_command(0x40);
    
    ssd1306_send_command(SSD1306_CMD_INVERT_OFF);
    
    ssd1306_clear_screen();
    ssd1306_send_command(SSD1306_CMD_DISPLAY_ON);
    
    return SSD1306_OK;
}

// Limpiar toda la pantalla
ssd1306_result_t ssd1306_clear_screen(void) {
    ssd1306_send_command(SSD1306_CMD_SET_COL_ADDR);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0x7F);
    
    ssd1306_send_command(SSD1306_CMD_SET_PAGE_ADDR);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0x07);
    
    // Enviar 0x00 para limpiar toda la pantalla
    for (int i = 0; i < SSD1306_WIDTH * SSD1306_PAGES; i++) {
        ssd1306_send_data_byte(0x00);
    }
    
    return SSD1306_OK;
}

// Establecer cursor
ssd1306_result_t ssd1306_set_cursor(uint8_t page, uint8_t col) {
    ssd1306_send_command(SSD1306_CMD_SET_COL_ADDR);
    ssd1306_send_command(col);
    ssd1306_send_command(0x7F);
    
    ssd1306_send_command(SSD1306_CMD_SET_PAGE_ADDR);
    ssd1306_send_command(page);
    ssd1306_send_command(0x07);
    
    return SSD1306_OK;
}

// Imprimir texto en una página específica
ssd1306_result_t ssd1306_print_text(uint8_t page, const char *text) {
    if (!text || page >= SSD1306_PAGES) return SSD1306_ERROR;
    
    uint8_t col = 0;
    ssd1306_set_cursor(page, col);
    
    while (*text && col < SSD1306_WIDTH - 6) {
        uint8_t font_index = get_font_index(*text);
        const uint8_t *font_data = font5x7_basic[font_index];
        
        // Enviar los 5 bytes del carácter
        for (int i = 0; i < 5; i++) {
            ssd1306_send_data_byte(font_data[i]);
        }
        
        // Espacio entre caracteres
        ssd1306_send_data_byte(0x00);
        
        col += 6;
        text++;
    }
    
    return SSD1306_OK;
}
