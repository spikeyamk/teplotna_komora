#include <iostream>
#include "panel/sevseg/green_yellow/green_yellow.hpp"

extern SPI_HandleTypeDef hspi2;

namespace panel {
namespace sevseg {
namespace green_yellow {
    // Function to send a command to the MAX6954 using SPI
    void write(uint8_t address, uint8_t data) {
        uint8_t txBuffer[2];
        txBuffer[0] = address;  // Command byte (address)
        txBuffer[1] = data;     // Data byte

        // Send command (address + data)
        std::printf("HAL_SPI_Transmit(&hspi2, txBuffer, 2, HAL_MAX_DELAY): %d\n\r", HAL_SPI_Transmit(&hspi2, txBuffer, 2, HAL_MAX_DELAY));
    }

    void init() {
        // Decode mode enabled
        write(0x01, 0b11111111);
        // Scan Limit - all digits
        write(0x03, 0b00000111);
        // Configuration - fast blinking and normal operation, invidual digit brightness
        //write(0x04, 0b01000101);
        // Configuration - fast blinking and normal operation, global brightness
        write(0x04, 0b00000101);
        // Digit Type - all 16 segments
        write(0x0C, 0b00000000);
    }

    void test() {
      // Decode mode disabled
  write(0x01, 0b00000000);

  // Turn off all segments manually
  for (int i=0x20; i<=0x2F; i++) {
    write(i, 0b00000000);
  }

  // Turn on all segments manually (decode mode off)
  // write(0x20, 0b11111111);
  // write(0x28, 0b11111111);
  // delay(5000);

  int dl = 40;
  for (int i=0; i<4; i++) {
    write(0x20,0b00000001);
    HAL_Delay(dl);
    write(0x20,0b00000000);
    write(0x28,0b00100000);
    HAL_Delay(dl);
    write(0x28,0b00010000);
    HAL_Delay(dl);
    write(0x28,0b00001000);
    HAL_Delay(dl);
    write(0x28,0b01000000);
    HAL_Delay(dl);
    write(0x28,0b00000100);
    HAL_Delay(dl);
    write(0x28,0b00000010);
    HAL_Delay(dl);
    write(0x28,0b00000001);
    HAL_Delay(dl);
  }

  for (int i=0x20; i<=0x2F; i++) {
    write(i, 0b00000000);
  }

  // Test each segment bit slowly
//  dl = 3000;
//  int segment = 0b10000000;
//  for (int i=0; i<8; i++) {
//    write(0x20,segment);
//    segment = segment >> 1;
//    HAL_Delay(dl);
//  }
//  write(0x20,0b00000000);
//  segment = 0b10000000;
//  for (int i=0; i<8; i++) {
//    write(0x28,segment);
//    segment = segment >> 1;
//    HAL_Delay(dl);
//  }
//  write(0x28,0b00000000);

  // Decode mode enabled
  write(0x01, 0b11111111);
    }
}
}
}