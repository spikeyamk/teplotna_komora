#include <iostream>
#include <array>
#include "stm32h7xx_nucleo.h"
#include "main.h"

void redirect_printf() {
    /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
    COM_InitTypeDef BspCOMInit {
        .BaudRate   = 115200,
        .WordLength = COM_WORDLENGTH_8B,
        .StopBits   = COM_STOPBITS_1,
        .Parity     = COM_PARITY_NONE,
        .HwFlowCtl  = COM_HWCONTROL_NONE,
    };
    if(BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
        Error_Handler();
    }
}