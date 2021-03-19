#define F_CPU 1000000UL

//#include <avr/eeprom.h>
//#include <avr/interrupt.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
//#include <avr/sleep.h>
//#include <avr/wdt.h>
//#include <avr/builtins.h>
//#include <stdint.h>

#include <util/delay.h>
#include <stdlib.h>
#include "Pins.h"
#include "Stepper.h"
#include "RTC.h"
#include "UART.h"
#include "AnalogClock.h"

//Pin LED_ONE=    Pin(B,0);
//Pin LED_TWO=    Pin(B,1);
//Pin READ   =    Pin(D,7);

AnalogClock clock = AnalogClock(
		Pin(C,5),Pin(C,4),//buttons
		Pin(D,6),Pin(D,7),Pin(B,0),//RTC
		Pin(C,3),Pin(C,2),Pin(C,1),Pin(C,0)//stepper
		);
void setup() {

//	pinMode(LED_ONE, OUTPUT);
//	pinMode(LED_TWO, OUTPUT);
//	pinMode(READ,INPUT);

	USART_Init();

	//__builtin_avr_sei();//enable interrupts
	//__builtin_avr_cli();//disable interrupts
}

void loop() {
	clock.tick();

	printString("Now: ");
	printui(clock.getRTC().getHour());printString(":");
	printui(clock.getRTC().getMinute());printString(":");
	printui(clock.getRTC().getSecond());printString("\n");



	/*if (digitalRead(clock.getButtonInc()) == HIGH) printString("INC is held\n");
	if (digitalRead(clock.getButtonDec()) == HIGH) printString("DEC is held\n");

	printString("\n");
	printString("DATE: \"");printString(__DATE__);
	printString("\" TIME: ");printString(__TIME__);printString("\"\n");*/
}

int main() {
	setup();
	while(1) loop();
	return 0;
}