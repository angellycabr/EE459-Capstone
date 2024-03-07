#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> // Include for sprintf
#include <stdint.h> // Include for int32_t
#include "i2c.h"
#include "lcd.h"
#include "DHT20.h"
//#include <stddef.h>

int main(void) {
	
	i2c_init(0x66); // Set appropriate baud rate divisor 0x42, start with 0x48, initially 0x20
    _delay_ms(100); // Wait for sensor to power on
	lcd_init();
	
	// Start measurement
	//dht20_start_measurement();

    // Main loop
    while(1) {
        // Read temperature and humidity from DHT20
		lcd_init();
		dht20_start_measurement();
		_delay_ms(1000);
		
		PORTC |= 1 << PC0;      // Set PC0 to a 1, LED on
		
		uint8_t buffer[7];
		dht20_read_data(buffer);
		
		/**
		// Read status byte and check if conversion is complete
		uint8_t status = dht20_read_status();
		
        // Assemble Celsius
		int32_t temperature = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
		float tf = (float)temperature;
		float Celsius = (tf * 200.0f) / 1048576.0f - 50.0f;

		// Assemble Humidity
		int32_t humidity = (buffer[1] << 12) | (buffer[2] << 4) | (buffer[3] >> 4);
		float hf = (float)humidity;
		float Humidity = (hf * 100.0f) / 1048576.0f;

		// Convert temperature and humidity to strings
		char tempString[20];
        char humString[20];
		
		sprintf(tempString, "Temp: %.2fC", Celsius);
		sprintf(humString, "Hum: %.2f%%", Humidity);
        **/
		char lcdBuffer[20]; 
		int32_t temperature = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
		float tf = (float)temperature;
		float Celcius = (tf * 200) / 1048576 - 50;
		
		int temp = (int)(Celcius * 10); //originally number
		int ones = abs(temp / 10); 
		int tenths = abs(temp % 10);
		
		sprintf(lcdBuffer, "Temp: %d.%dF", ones, tenths);	
		lcd_writedata(lcdBuffer, 0);
		
        // Delay before next read 
        _delay_ms(20000); // Adjust delay as needed
		PORTC &= ~(1 << PC0);   // Set PC0 to a 0, LED off
    }
}