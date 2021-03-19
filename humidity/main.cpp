#define F_CPU 800000UL

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/builtins.h>
#include <stdint.h>

#include <util/delay.h>
#include <stdlib.h>
#include "lib/Pins.h"

Pin LED_ONE=    Pin(B,3);
//Pin LED_TWO=    Pin(B,1);
//Pin READ   =    Pin(D,7);

void setup() {
	pinMode(LED_ONE, OUTPUT);
//	pinMode(LED_TWO, OUTPUT);
//	pinMode(READ,INPUT);

//	USART_Init();
}

void loop() {
	digitalWrite(LED_ONE,HIGH);
	_delay_ms(500);
	digitalWrite(LED_ONE,LOW);
	_delay_ms(500);
}

int main() {
	setup();
	while(1) loop();
	return 0;
}