#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define F_CPU 16000000UL // assuming a clock speed of 16 MHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// Initialize UART
void uart_init(unsigned int ubrr) {
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /*Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 1stop bit */
    UCSR0C = (3<<UCSZ00);
}

// Transmit a single character over UART
void uart_transmit(char data) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

// Transmit a string over UART
void uart_print(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

// Transmit a string followed by a newline over UART
void uart_println(const char* str) {
    uart_print(str);
    uart_transmit('\r');
    uart_transmit('\n');
}