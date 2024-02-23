#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "i2c.h"

#define DHT20_I2C_ADDRESS   0x70

void dht20_start_measurement() {
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    i2c_io(DHT20_I2C_ADDRESS, cmd, 3, NULL, 0);
    _delay_ms(80); 
}

uint8_t dht20_read_status() {
    uint8_t status;
    do {
        i2c_io(DHT20_I2C_ADDRESS, NULL, 0, &status, 1);
    } while (status & 0x80); // Loop until bit 7 is clear
    return status;
}

void dht20_read_data(uint8_t *buffer) {
    i2c_io(DHT20_I2C_ADDRESS, NULL, 0, buffer, 7);
}

