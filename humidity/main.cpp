#define F_CPU 1000000UL

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <avr/sleep.h>
//#include <avr/wdt.h>
#include <avr/builtins.h>
#include <stdint.h>
#include "string.h"

#include <util/delay.h>
#include <stdlib.h>
#include "lib/Pins.h"
#include "lib/minimized/TinyUart.h"
#include "lib/minimized/russiandht.h"
//#include "lib/NativeUart.h"

int* temp = (int*)malloc(sizeof(int));
int* hum = (int*)malloc(sizeof(int));

Pin LED_G = Pin(D,5);
Pin LED_E = Pin(A,0);

Pin LED_B2 = Pin(B,0);
/**
 *
 * TODO:
 *  - Create SevenSegment class for a 6 digit common cathode display
 *      - Initialize something like SevenSegment(digitArray,6,anodeArray,8)
 *      - Set with something like display.set(0,5) for setting digit 0 to a 5
 * */

void loop() {
	digitalWrite(LED_B2,HIGH);
	digitalWrite(LED_G,HIGH);
	digitalWrite(LED_E,HIGH);
	_delay_ms(1000);
	digitalWrite(LED_B2,LOW);
	digitalWrite(LED_G,LOW);
	digitalWrite(LED_E,LOW);
	_delay_ms(1000);
	UART_tx_str("Tock\n");
//	printString("Hey wassup boomer\n");

	/*if (dhtread(6,hum,temp)) {
		*temp = (int)round((double)(*temp)/10);
		*hum = (int)round((double)(*hum)/10);
		UART_tx_str("ACTUAL Temp:");printui(*temp);printString("\n");
		UART_tx_str("ACTUAL Hum :");printui(*hum );printString("\n");
	} else {
		UART_tx_str("dhtread error\n");
	}*/
}

int main() {
	pinMode(LED_B2, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_E, OUTPUT);
	UART_init();
	while (1) loop();
	return 0;
}