#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> // Include for sprintf
#include <stdint.h> // Include for int32_t
#include <math.h>
#include "i2c.h"
#include "lcd.h"
#include "DHT20.h"
#include "TSL2591.h"
#include "moisture.h"
#include "uart.h"
//#include <stddef.h>

#define F_CPU 16000000UL // assuming a clock speed of 16 MHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


int main(void) {
	
	i2c_init(0x66); // Set appropriate baud rate divisor 0x42, start with 0x48, initially 0x20
    _delay_ms(100); // Wait for sensor to power on
	lcd_init();
	ADC_init(); 
	
	// Start measurement
	//dht20_start_measurement();
	
	// Initialize UART
	uart_init(MYUBRR);

    // Main loop
    while(1) {
        // Read temperature and humidity from DHT20
		lcd_init();
		dht20_start_measurement();
		_delay_ms(1000);
		
		// DTH20 light intensity reading
		uint8_t buffer[7];
		dht20_read_data(buffer);
		
		// TSL25911 light intensity reading

		// Moisture reading
		uint16_t adc_result;
		adc_result = ADC_read();
		
		char tempString[20]; 
		char humString[20];
		char lightString[20];
		char moistString[20];
		
		// Assemble Temperature
		int32_t temperature = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
		float tf = (float)temperature;
		float Celcius = (tf * 200) / 1048576 - 50;
		
		int temp = (int)(Celcius * 10); //originally number
		int ones = abs(temp / 10); 
		int tenths = abs(temp % 10);
		
		sprintf(tempString, "Temp: %d.%dF", ones, tenths);	
		lcd_writedata(tempString, 0);
		
		// Assemble Humidity
		int32_t humidity = (buffer[1] << 12) | (buffer[2] << 4) | (buffer[3] >> 4);
		float hf = (float)humidity;
		float Humidity = (hf * 100) / 104858; //1048576
		
		int percent = (int)(Humidity);
		int hum = abs(percent);
		
		sprintf(humString, "Humidity: %d%%", hum);
		lcd_writedata(humString, 1);
		
		// Assemble Brightness
		int light = 50;
		
		sprintf(lightString, "Light: %d%%", light);
		lcd_writedata(lightString, 2);
		
		// Assemble Moisture	
		int moisture = (adc_result / 100);	
		/**
		float Es = 6.112 * exp((17.67 * Celcius) / (Celcius + 243.5)); // previously ones
		float E = (Humidity) * Es;
		float vapor = (216.7 * E) / (273.15 + Celcius); // previously ones
		
		int moisture = abs((int)(vapor * 10));
		**/
		
		sprintf(moistString, "Moisture: %d%%", moisture);	
		lcd_writedata(moistString, 3);
		
		// Send sensor data across UART
		uart_println(tempString);
        uart_println(humString);
        uart_println(lightString);
        uart_println(moistString);

        // Delay before next read 
        _delay_ms(20000); // Adjust delay as needed
    }
}