#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "i2c.h"
#include TSL2591.h

#define TSL2591_I2C_ADDRESS 0x29
#define TSL2591_ENABLE 0x00
#define TSL2591_CONTROL 0x01
#define TSL2591_COMMAND_BIT 0x80 // Indicates a command transaction
#define TSL2591_C0DATAL 0x14 

// Definitions for enabling the device
#define TSL2591_AEN 0x02 // ALS Enable bit in ENABLE register
#define TSL2591_PON 0x01 // Power ON bit in ENABLE register
#define TSL2591_POWER_ON (TSL2591_AEN | TSL2591_PON) 

void tsl2591_start_measurement() {
    uint8_t cmd[2];
    cmd[0] = TSL2591_COMMAND_BIT | TSL2591_ENABLE; // Prepare the command byte
    cmd[1] = TSL2591_POWER_ON;
    i2c_io(TSL2591_I2C_ADDRESS, cmd, 2, NULL, 0);
    _delay_ms(80); 
}


void tsl2591_read_light(uint16_t *channel0, uint16_t *channel1) {
    	uint8_t buffer[4]; // To hold data from the 4 registers
	while(1) {
    		uint8_t command = TSL2591_COMMAND_BIT | TSL2591_C0DATAL;
    		i2c_io(TSL2591_I2C_ADDRESS, &command, 1, buffer, 4);

    		*channel0 = (buffer[1] << 8) | buffer[0]; 
    		*channel1 = (buffer[3] << 8) | buffer[2];
	} 
}