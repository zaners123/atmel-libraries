#define F_CPU 1000000L
#define LED_PIN 3

#define true 1
#define false 0

//contains device constants, such as memory size, signature, etc
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/builtins.h>
//contains ability to delay
#include <util/delay.h>
#include "UART.h"
#include <stdint.h>
#include <stdlib.h>



#include "BasicSerial/BasicSerial.h"

// sketch to test Serial

// change LEDPIN based on your schematic
#define LEDPIN  PINB3

/*void setup(){
	DDRB |= (1<<LEDPIN);    // set LED pin to output mode
}

void serOut(const char* str)
{
	while (*str) TxByte (*str++);
}

void loop(){
	serOut("Turning on LED\n");
	PORTB |= (1<<LEDPIN);  // turn on LED
	_delay_ms(500);  // 0.5 second delay
	PORTB &= ~(1<<LEDPIN); // turn off LED
	_delay_ms(1000); // 1 second delay
}
int main() {
	setup();
	while(1) loop();
	return 0;
}*/

unsigned char getPinBIT(unsigned char pin) {
	return (1<<pin);
}

void setPortB(int pin, bool value) {
	//toggle Data Register for PORTB
	//  (It's a write pin now, so writing to this register sets the pin)
	if (value) {
		PORTB |= getPinBIT(pin);
	} else {
		PORTB &= ~getPinBIT(pin);
	}
}

void setup() {
	//set Data Direction Register for a pin on port B to true (write)
	DDRB |= getPinBIT(LED_PIN);
	//Serial.begin(19200);
	Serial.println("Hewwo Wowld");

	//__builtin_avr_sei();//enable interrupts
	//__builtin_avr_cli();//disable interrupts
}

void loop() {
//	if (mySerial.available()) {
//    Serial.write(mySerial.read());
//  }
//
	setPortB(LED_PIN,true);
	_delay_ms(500);
//
	setPortB(LED_PIN,false);
//	_delay_ms(1000);
	_delay_ms(500);
}

int main() {
	setup();
	while(1) loop();
	return 0;
}