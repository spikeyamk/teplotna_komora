#include "MAX6954.h"
#include <stdint.h>
#include "stdio.h"

extern SPI_HandleTypeDef hspi2;

void printBinary(uint8_t value)
{
    for (int i = 7; i >= 0; i--)
    {
        // Use bitwise AND to check if the ith bit is set
        printf("%d", value & (1 << i) ? 1 : 0);
    }
    printf("\n");
}

void printBinary16(uint16_t value)
{
    for (int i = 15; i >= 0; i--)
    {
        // Use bitwise AND to check if the ith bit is set
        printf("%d", value & (1 << i) ? 1 : 0);
    }
    printf("\n");
}

/*********************** Begin Private functions *************************/
/**
 * Write x bytes using software SPI
 *
 * @param data Pointer to transmit buffer
 * @param len  Amount of bytes to send
 */

void MAX6954_spi_write(uint8_t *data, uint8_t len)
{
    // Start by pulling the chip select low to begin communication
    HAL_GPIO_WritePin(MAX6954_CE_PORT, MAX6954_CE_PIN, GPIO_PIN_RESET);

    for (uint8_t i = 0; i < len; i++)
    {
        // Transmit the data using HAL SPI transmit function
        if (HAL_SPI_Transmit(&hspi2, data[i], 1, HAL_MAX_DELAY) != HAL_OK)
        {
            // Handle error
            printf("SPI Transmission Error\n");
        }
        else
        {
            printf("Successfully sent to MAX6954 via SPI bytes\n", 1);
            printBinary(data[i]);
        }
    }

    // Pull the chip select high to end communication
    HAL_GPIO_WritePin(MAX6954_CE_PORT, MAX6954_CE_PIN, GPIO_PIN_SET);
}

/**
 * Send a command to the MAX6954
 *
 * @param address       The address of the digit to write to
 * @param data          The data to write to the digit
 * @param decimal_point Whether to display a decimal point on the digit
 */
void MAX6954_send_command(uint8_t address, uint8_t data, bool decimal_point)
{
    uint16_t command = 0;

    // D15 is 0 for write, set by default
    // D14-D8: Address bits
    command |= (address << 8);
    printf("Sending command to MAX6954 address %#X\n", address);
    printBinary(address);
    printf("Sending data MAX6954\n");
    printBinary(data);

    // D7: Decimal point control (1 for on, 0 for off)
    if (decimal_point)
    {
        command |= (1 << 7);
    }

    // D6-D4: Unused, should be 0 by default (no need to set)

    // D3-D0: Data (character to display, from 0 to F)
    command |= (data & 0x0F);
    printf("Sending command to MAX6954\n");
    printBinary16(command);

    // Send the 16-bit command via SPI
    uint8_t data_to_send[2];
    data_to_send[0] = (command >> 8) & 0xFF; // MSB first
    data_to_send[1] = command & 0xFF;        // LSB second
    MAX6954_spi_write(data_to_send, 2);
}

/**
 * Convert a float number to individual digits for display
 *
 * @param number The float number to convert
 * @return MAX6954_Float_Digits The individual digits and decimal points
 */
MAX6954_Float_Digits convert_to_float_digits(float number)
{
    printf("Converting float %f to digits\n", number);
    // Split the float number into integer and fractional parts
    int integer_part = (int)number;
    int fractional_part = (int)((number - integer_part) * 100); // Assume 2 decimal places for now

    // Break down each part into individual digits
    int digits[5] = {0};              // Array to hold the digits
    bool decimal_points[5] = {false}; // Array to hold which digits have decimal points

    // Get integer digits
    digits[4] = integer_part % 10;         // Ones place
    digits[3] = (integer_part / 10) % 10;  // Tens place
    digits[2] = (integer_part / 100) % 10; // Hundreds place
    decimal_points[2] = true;              // Decimal point for the tens place

    // Get fractional digits
    digits[1] = fractional_part / 10; // Tenths place
    digits[0] = fractional_part % 10; // Hundredths place

    MAX6954_Float_Digits result;
    for (int i = 0; i < 5; i++)
    {
        result.digits[i] = digits[i];
        result.decimal_points[i] = decimal_points[i];
    }

    return result;
}

/**
 * Display the digits on the yellow 7-segment display
 *
 * @param float_digits The float digits to display
 */
void display_on_yellow_7_segment(MAX6954_Float_Digits float_digits)
{
    // Display the digits on the yellow 7-segment display
    printf("Displaying on yellow color first digit %d in MAX6954\n", float_digits.digits[0]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS4, float_digits.digits[4], float_digits.decimal_points[4]);
    printf("Displaying on yellow color second digit %d in MAX6954\n", float_digits.digits[1]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS7, float_digits.digits[3], float_digits.decimal_points[3]);
    printf("Displaying on yellow color third digit %d in MAX6954\n", float_digits.digits[2]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS10, float_digits.digits[2], float_digits.decimal_points[2]);
    printf("Displaying on yellow color fourth digit %d in MAX6954\n", float_digits.digits[1]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS12, float_digits.digits[1], float_digits.decimal_points[1]);
    printf("Displaying on yellow color fifth digit %d in MAX6954\n", float_digits.digits[0]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS14, float_digits.digits[0], float_digits.decimal_points[0]);
}

/**
 * Display the digits on the green 7-segment display
 *
 * @param float_digits The float digits to display
 */
void display_on_green_7_segment(MAX6954_Float_Digits float_digits)
{
    // Display the digits on the green 7-segment display
    printf("Displaying on green color first digit %d in MAX6954\n", float_digits.digits[0]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS5, float_digits.digits[4], float_digits.decimal_points[4]);
    printf("Displaying on green color second digit %d in MAX6954\n", float_digits.digits[1]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS8, float_digits.digits[3], float_digits.decimal_points[3]);
    printf("Displaying on green color third digit %d in MAX6954\n", float_digits.digits[2]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS11, float_digits.digits[2], float_digits.decimal_points[2]);
    printf("Displaying on green color fourth digit %d in MAX6954\n", float_digits.digits[1]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS13, float_digits.digits[1], float_digits.decimal_points[1]);
    printf("Displaying on green color fifth digit %d in MAX6954\n", float_digits.digits[0]);
    MAX6954_send_command(MAX6954_DIGIT_ADDRESS_DS15, float_digits.digits[0], float_digits.decimal_points[0]);
}
/*********************** End Private functions *************************/

/*********************** Begin Public functions *************************/

/**
 * Initialize the MAX6954 display
 */
void MAX6954_init(void)
{
    // Implement function to set intensity and other display configurations
    printf("Initializing display in MAX6954\n");
}

/**
 * Display a float number on the 7-segment display
 *
 * @param number The number to display
 * @param color The color of the display (YELLOW or GREEN)
 */
void MAX6954_display_float(float number, enum DisplayColor color)
{
    MAX6954_Float_Digits float_digits = convert_to_float_digits(number);
    printf("Displaying float %f in MAX6954\n", number);

    if (color == YELLOW)
    {
        display_on_yellow_7_segment(float_digits);
    }
    else if (color == GREEN)
    {
        display_on_green_7_segment(float_digits);
    }
}
