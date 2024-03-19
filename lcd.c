#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <string.h>

#include "i2c.h"

#define LCD_I2C_ADDRESS 0x50 // Default I2C address for the LCD

// Required for an init sequence
void lcd_init(void) {

    _delay_ms(500); // Wait for the module to be ready
	uint8_t cmd1[2] = {0xFE, 0x51};
    i2c_io(LCD_I2C_ADDRESS, cmd1, 2, NULL, 0);
	
    _delay_ms(500); // Set the backlight lev
	uint8_t cmd2[3] = {0xFE, 0x53, 0x04};
	i2c_io(LCD_I2C_ADDRESS, cmd2, 3, NULL, 0);
}

/**
void lcd_writedata(char *str, uint8_t line) {
    // Lines 1 and 2 on the LCD, the remaining lines: 0x14, 0x54
    uint8_t line_addresses[] = {0x00, 0x40};
    uint8_t command = 0x80 | line_addresses[line]; // Set DDRAM address command
    uint8_t control_byte = 0x40; // Data mode control byte, if needed
    
    // Set the cursor to the beginning of the specified line
    uint8_t cursor_set[3] = {0xFE, 0x45, 0x00}; //command
    i2c_io(LCD_I2C_ADDRESS, cursor_set, 3, NULL, 0);

    
    // Write each character of the string to the LCD
	size_t i;
    for (i = 0; i < strlen(str); i++) {
		char data = str[i];
        i2c_io(LCD_I2C_ADDRESS, &data, 1, NULL, 0);
    }
}
**/


void lcd_writedata(char *str, uint8_t line) {
    uint8_t line_addresses[] = {0x00, 0x40}; // Assuming a 2-line display for simplicity
    uint8_t command[3] = {0xFE, 0x00, 0x00}; // Prepare command buffer

    if (line == 1) {
        // Custom cursor set command for line 1
        command[1] = 0x45;
        command[2] = 0x40; // Assuming this is the intended position
    } else {
        // Standard command to set DDRAM address for lines 0 or other
        command[1] = 0x80 | line_addresses[line];
    }

    // Send the command to set the cursor position
    i2c_io(LCD_I2C_ADDRESS, command, 3, NULL, 0);

    // Write each character of the string to the LCD
	size_t i;
    for (i = 0; i < strlen(str); i++) {
        char data = str[i];
        i2c_io(LCD_I2C_ADDRESS, &data, 1, NULL, 0);
    }
}



