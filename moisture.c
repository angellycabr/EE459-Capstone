#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

void ADC_init() {
    // 1. Configure the REFS[1:0] bits to select AVCC as the reference voltage
    ADMUX = (1 << REFS0);
    // 2. Clear the ADLAR bit to right adjust the result (for 10-bit resolution)
    ADMUX &= ~(1 << ADLAR);
    // 4. Configure the ADPS[2:0] bits to select the clock prescaler (128 for 16MHz clock)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // 5. Set the ADEN bit in ADCSRA to enable the ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t ADC_read(uint8_t channel) {
    // 3. Configure the MUX[3:0] bits to select the ADC input channel dynamically
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Select ADC channel dynamically

    // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Polling the ADSC bit waiting for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read the ADC value (10-bit)
    uint16_t result = ADC;

    return result;
}

/**
#define ADC_CHANNEL 1 //PIN 24

void ADC_init() {
    // 1. Configure the REFS[1:0] bits to select AVCC as the reference voltage
    ADMUX = (1 << REFS0);
    // 2. Clear the ADLAR bit to right adjust the result (for 10-bit resolution)
    ADMUX &= ~(1 << ADLAR);
    // 3. Configure the MUX[3:0] bits to select the ADC input channel
    ADMUX = (ADMUX & 0xF0) | (ADC_CHANNEL & 0x0F); // Select ADC channel
    // 4. Configure the ADPS[2:0] bits to select the clock prescaler (128 for 16MHz clock)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // 5. Set the ADEN bit in ADCSRA to enable the ADC
    ADCSRA |= (1 << ADEN);
}

uint16_t ADC_read() {
    // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Polling the ADSC bit waiting for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Read the ADC value (10-bit)
    uint16_t result = ADC;

    return result;
}
**/