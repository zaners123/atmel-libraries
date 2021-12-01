#define F_CPU 1000000UL

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

#include "lib/Pins.h"

#define nullptr 0

class LEDShiftRegister {
	Pin SDI,CLK,LE,NOE;
public:
	LEDShiftRegister(Pin SDI, Pin CLK, Pin LE, Pin NOE) : SDI(SDI),CLK(CLK),LE(LE),NOE(NOE) {
		pinMode(SDI,OUTPUT);
		pinMode(CLK,OUTPUT);
		pinMode(LE ,OUTPUT);
		pinMode(NOE,OUTPUT);
	}
	void write(uint16_t data) {
		for (uint8_t i=0;i<16;i++) {
			const bool valueToWrite = data & 1<<i;
			digitalWrite(CLK,false);
			_delay_ms(1);
			digitalWrite(SDI,valueToWrite);
			digitalWrite(CLK,true);
			_delay_ms(1);
		}
		digitalWrite(LE,true);
		digitalWrite(LE,false);
	}
	void enable(bool enable) {
		digitalWrite(NOE,!enable);
	}
};

Pin SDI = Pin(B,0);
Pin NOE = Pin(B,1);
Pin CLK = Pin(B,2);
Pin LE =  Pin(B,3);
LEDShiftRegister lsr = LEDShiftRegister(SDI,CLK,LE,NOE);

void loop() {
	lsr.write(0xFFFF);
}

int main() {

	while (1) loop();
	return 0;
}