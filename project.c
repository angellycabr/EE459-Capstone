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

#define F_CPU 7372800
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#define ADC_CHANNEL_1 1 // PIN 24
#define ADC_CHANNEL_2 2 // PIN 25

int main(void) {
	
	i2c_init(0x66); // Set appropriate baud rate divisor 0x42, start with 0x48, initially 0x20
    _delay_ms(100); // Wait for sensor to power on
	lcd_init();
	ADC_init(); 
	
	//DDRC |= (1<<PC3);  //Pin 26 to output 
	
	// Initialize UART
	uart_init(MYUBRR);

    // Main loop
    while(1) {
        // Read temperature and humidity from DHT20
		lcd_init();
		
		dht20_start_measurement();
		_delay_ms(1000);
		
		//Pump
		//PORTC |= (1<<PC3);
		
		// DTH20 light intensity reading
		uint8_t buffer[7];
		dht20_read_data(buffer);

		// Moisture and Light reading
		uint16_t adcResult1, adcResult2;
		
		adcResult1 = ADC_read(ADC_CHANNEL_1);
		adcResult2 = ADC_read(ADC_CHANNEL_2);
		
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
		int light = 100 - (adcResult2 / 10);
		
		sprintf(lightString, "Light: %d%%", light);
		lcd_writedata(lightString, 2);
		
		// Assemble Moisture	
		int moisture = (adcResult1 / 10);	//adc_result
		
		sprintf(moistString, "Moisture: %d%%", moisture);	
		lcd_writedata(moistString, 3);
		
		// Send sensor data across UART
		_delay_ms(1000);
		uart_print("Hello, UART!");

		//uart_print(tempString);
        //uart_println(humString);
        //uart_println(lightString);
        //uart_println(moistString);

        // Delay before next read 
        _delay_ms(20000); // Adjust delay as needed
    }
}