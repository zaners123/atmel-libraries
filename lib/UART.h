#ifndef ATMEGA328P_UART_H
#define ATMEGA328P_UART_H

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/builtins.h>
//contains ability to delay
#include <util/delay.h>
//#include "UART.h"
#include <stdint.h>
#include <stdlib.h>

#define BAUD 4800
#define UBRR F_CPU/16/BAUD-1

#define ATOIMAX sizeof(unsigned int)*8+1
#define LTOIMAX sizeof(long int)*8+1
#define ULTOIMAX sizeof(unsigned long int)*8+1

void USART_Init() {
	unsigned int ubrr = UBRR;
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_TransmitString(const char* str) {
	while (*str) USART_Transmit(*str++);
}
/**
 * Alias for
 * @see USART_TransmitString(const char*)
 * */
void printString(const char* str) {
	USART_TransmitString(str);
}

void printNumber(unsigned int number, int radix) {
	static char buffer[ATOIMAX];
	utoa(number,buffer,radix);
	USART_TransmitString(buffer);
}

void printui(unsigned int number) {
	printNumber(number, 10);
}

void printl(long number) {
	static char buffer[LTOIMAX];
	ltoa(number,buffer,10);
	USART_TransmitString(buffer);
}

void printul(unsigned long number) {
	static char buffer[ULTOIMAX];
	ultoa(number,buffer,10);
	USART_TransmitString(buffer);
}

unsigned char USART_Receive(){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

#endif //ATMEGA328P_UART_H