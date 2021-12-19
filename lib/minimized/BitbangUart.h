
/**
 * A bit-bang UART library, made to be as small/simple as possible
 * This doesn't even use dedicated UART or USI (since that changes depending on the chip)
 * I usually set the UART pin to MOSI/MISO/SCK/etc so that it can be read over ICSP
 * Uses timer 0, so look out I guess
 * */

#include <avr/builtins.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "../Pins.h"


volatile uint16_t tx_shift_reg = 0;

void UART_tx(char character)
{
	uint16_t local_tx_shift_reg = tx_shift_reg;
	//if sending the previous character is not yet finished, return
	//transmission is finished when tx_shift_reg == 0
	if(local_tx_shift_reg){return;}
	//fill the TX shift register witch the character to be sent and the start & stop bits (start bit (1<<0) is already 0)
	local_tx_shift_reg = (character<<1) | (1<<9); //stop bit (1<<9)
	tx_shift_reg = local_tx_shift_reg;
	//start timer0 with a prescaler of 8
	TCCR0B = (1<<CS01);
}

void UART_tx_str(const char* string){
	//until null char
	while( *string ){
		//send a char
		UART_tx(*string++ );
		//wait until transmission is finished
		while(tx_shift_reg);
	}
}

void UART_tx_uint16(uint16_t number, int radix = 10){
	static char buffer[sizeof(int)*8+1];
	utoa(number,buffer,radix);
	UART_tx_str(buffer);
}

void UART_init(){
	//set TX pin as output
	pinMode(uart_pin,OUTPUT_PULLUP);
	//set timer0 to CTC mode
	TCCR0A = (1<<WGM01);
	//enable output compare 0 A interrupt

#ifdef TIMSK0
	TIMSK0 |= (1<<OCIE0A);
#else
	TIMSK |= (1<<OCIE0A);
#endif
	//OCR0A = F_CPU /(Baud * 64), due to CS01 prescaler
	//1200baud 8MHz = 104
	OCR0A = 104;
	//enable interrupts
	sei();
}

//timer0 compare A match interrupt
#if defined(TIM0_OVF_vect)
ISR(TIM0_COMPA_vect)
#else
ISR(TIMER0_COMPA_vect)
#endif
{
	uint16_t local_tx_shift_reg = tx_shift_reg;
	//output LSB of the TX shift register at the TX pin
	digitalWrite(uart_pin,local_tx_shift_reg & 0x01);
	//shift the TX shift register one bit to the right
	local_tx_shift_reg >>= 1;
	tx_shift_reg = local_tx_shift_reg;
	//if the stop bit has been sent, the shift register will be 0
	//and the transmission is completed, so we can stop & reset timer0
	if(!local_tx_shift_reg)
	{
		TCCR0B = 0;
		TCNT0 = 0;
	}
}