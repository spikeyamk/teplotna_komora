#include "stm32h7xx_nucleo.h"
#include "comm/usb_uart/usb_uart.hpp"

extern "C" void Error_Handler(void);

namespace comm {
namespace usb_uart {
    void init() {
        COM_InitTypeDef BspCOMInit;
        BspCOMInit.BaudRate   = 115200;
        BspCOMInit.WordLength = COM_WORDLENGTH_8B;
        BspCOMInit.StopBits   = COM_STOPBITS_1;
        BspCOMInit.Parity     = COM_PARITY_NONE;
        BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
        if(BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
            Error_Handler();
        }
    }

    void deinit() {
        if(BSP_COM_DeInit(COM1) != BSP_ERROR_NONE) {
            Error_Handler();
        }
    }
}
}