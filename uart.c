#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

void uart_init(unsigned int ubrr) {
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (3<<UCSZ00);
}

// Transmit a single character over UART
void uart_transmit(char data) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

// Transmit a string over UART
void uart_print(char *str) {	
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
