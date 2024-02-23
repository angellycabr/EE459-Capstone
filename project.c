#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "lcd.h"
#include "DHT20.h"
//#include <stddef.h>

int main(void) {
	
	i2c_init(0x20); // Set appropriate baud rate divisor
    _delay_ms(100); // Wait for sensor to power on
	
	// Start measurement
	dht20_start_measurement();

    // Main loop
    while(1) {
        // Read temperature and humidity from DHT20
		PORTC |= 1 << PC0;      // Set PC0 to a 1, LED on
		
		// Read status byte and check if conversion is complete
		uint8_t status = dht20_read_status();
		
		// Read data
		uint8_t buffer[7];
		dht20_read_data(buffer);
		
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
        
        // Display the strings on the LCD
		lcd_writedata(tempString, 0); // Display temperature on line 0
        lcd_writedata(humString, 1);  // Display humidity on line 1
        
        // Delay before next read 
        _delay_ms(1000); // Adjust delay as needed
		PORTC &= ~(1 << PC0);   // Set PC0 to a 0, LED off
    }
}