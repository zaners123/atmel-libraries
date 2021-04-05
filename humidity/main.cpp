#define F_CPU 1000000UL

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/builtins.h>
#include <stdint.h>
#include "string.h"

#include <util/delay.h>
#include <stdlib.h>
#include "lib/Pins.h"
//#include "lib/minimized/TinyUart.h"
//#include "lib/minimized/russiandht.h"
#include "lib/NativeUart.h"

int main() {
	Pin LED = Pin(B,0);
	pinMode(LED, OUTPUT);
	USART_Init();
//	UART_init();//main using pin B1 for attiny85 or B6 for attiny4313
	while (1) {
		digitalWrite(LED,HIGH);
		_delay_ms(1000);
		digitalWrite(LED,LOW);
		_delay_ms(1000);
//		UART_tx_str("Hey I'm stuck in a ATtiny4313 High Performance, Low Power AVR® 8-Bit Microcontroller\n");
		printString("Hey wassup boomer\n");
	}
	return 0;
}

/*Pin LED_ONE=    Pin(B,0);
Pin LED_TWO=    Pin(B,2);

int* temp = (int*)malloc(sizeof(int));
int* hum = (int*)malloc(sizeof(int));

void setup() {
	pinMode(LED_ONE, OUTPUT);
	pinMode(LED_TWO, OUTPUT);
	UART_init();//main using pin B1
	//UART_tx_str("Here's your temp\n\n\n");
}

void loop() {
	UART_tx_str("AT\r\n");
	digitalWrite(LED_ONE,HIGH);
	digitalWrite(LED_TWO,LOW);
	_delay_ms(1000);
	UART_tx_str("AT\r\n");
	digitalWrite(LED_ONE,LOW);
	digitalWrite(LED_TWO,HIGH);
	_delay_ms(1000);

	if (dhtread(3,hum,temp)) {
		*temp = (int)round((double)(*temp)/10);
		*hum = (int)round((double)(*hum)/10);

		//UART_tx_str("ACTUAL Temp:");UART_tx_int(*temp);UART_tx_str("\n");
		//UART_tx_str("ACTUAL Hum :");UART_tx_int(*hum );UART_tx_str("\n");

//		digitalWrite(LED_ONE,*hum > 50);


	} else {
		//UART_tx_str("dhtread error\n");
	}
}

int main() {
	setup();
	while(1) loop();
	return 0;
}*/