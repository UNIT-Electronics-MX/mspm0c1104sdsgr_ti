/*
 * Copyright (c)     while(1) 
    {
        // Turn on PA24
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms on
        
        // Turn off PA24
        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms off Electronics MX
 * 
 * MSPM0C1104 Development Template - Main Application
 * 
 * This file is part of the MSPM0C1104 development template.
 * Licensed under MIT License - see LICENSE file for details.
 * 
 * This template uses TI MSPM0 SDK components which are licensed
 * under TI Commercial License and BSD-3-Clause License.
 * See LICENSES.md for complete licensing information.
 */

#include "ti_msp_dl_config.h"

int main(void)
{
    // Initialize the system
    SYSCFG_DL_init();
    
    while(1) 
    {
        // Encender PA0, PA24 y PA27 todos juntos
        DL_GPIO_setPins(GPIO_PA0_PORT, GPIO_PA0_PIN);
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_GPIO_setPins(GPIO_PA27_PORT, GPIO_PA27_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 2);  // 500ms encendido
        
        // Apagar PA0, PA24 y PA27 todos juntos
        DL_GPIO_clearPins(GPIO_PA0_PORT, GPIO_PA0_PIN);
        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_GPIO_clearPins(GPIO_PA27_PORT, GPIO_PA27_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 2);  // 500ms apagado
    }
}
