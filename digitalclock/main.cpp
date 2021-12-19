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

#include "lib/RTC.h"

#define uart_pin Pin(B,1)
#include "lib/minimized/BitbangUart.h"

class LEDShiftRegister {
	Pin SDI,CLK,LE,NOE;
public:
	LEDShiftRegister(Pin SDI, Pin CLK, Pin LE, Pin NOE) : SDI(SDI),CLK(CLK),LE(LE),NOE(NOE) {
		pinMode(SDI,OUTPUT);
		pinMode(CLK,OUTPUT);
		pinMode(LE ,OUTPUT);
		pinMode(NOE,OUTPUT);
	}
	void write(uint32_t data) {
		for (uint8_t i=0;i<32;i++) {
			digitalWrite(CLK,false);
			_delay_ms(1);//TODO test then remove
			digitalWrite(SDI,data & 1<<(31-i));
			digitalWrite(CLK,true);
			_delay_ms(1);//TODO test then remove
		}
		digitalWrite(LE,true);
		_delay_ms(1);//TODO test then remove
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

uint32_t timeToSevenSegTime(const uint8_t hour, const uint8_t min, const uint8_t sec) {
	const uint8_t mux[10] = {
			//XGFEDCBA <- Bits are set by
			0b00111111,//0
			0b00000110,//1
			0b01011011,//2
			0b01001111,//3
			0b01100110,//4
			0b01101101,//5
			0b01111101,//6
			0b00000111,//7
			0b01111111,//8
			0b01100111,//9
	};
	return
		(uint32_t)(hour/10?mux[hour/10]:0) |//Hour Tens - This also makes it display "1:30" instead of "01:30"
		(uint32_t)(mux[hour%10])<<7 |//Hour Ones
		(uint32_t)(mux[min /10])<<14 |//Minute Tens
		(uint32_t)(mux[min %10])<<21 |//Minute Ones
		(uint32_t)(sec%2)<<28;//cursor
}

/**
 * @brief This LED Display's pinout is in a mixed order to minimize trace length.
 *  This puts the output in the right order
 * @param unordered The unordered pins
 * @returns The pins, in order
 * */
uint32_t orderLEDDisplay(const uint32_t unordered) {
	return
		/*HorX*/
		((unordered & 1L<< 0)?1L<< 7:0) |
		((unordered & 1L<< 1)?1L<< 6:0) |
		((unordered & 1L<< 2)?1L<<11:0) |
		((unordered & 1L<< 3)?1L<<12:0) |
		((unordered & 1L<< 4)?1L<<10:0) |
		((unordered & 1L<< 5)?1L<< 5:0) |
		((unordered & 1L<< 6)?1L<< 4:0) |
		/*Hor1*/
		((unordered & 1L<< 7)?1L<< 2:0) |
		((unordered & 1L<< 8)?1L<< 3:0) |
		((unordered & 1L<< 9)?1L<< 9:0) |
		((unordered & 1L<<10)?1L<<13:0) |
		((unordered & 1L<<11)?1L<<15:0) |
		((unordered & 1L<<12)?1L<< 0:0) |
		((unordered & 1L<<13)?1L<< 1:0) |
		/*MinX*/
		((unordered & 1L<<14)?1L<<30:0) |
		((unordered & 1L<<15)?1L<<31:0) |
		((unordered & 1L<<16)?1L<<18:0) |
		((unordered & 1L<<17)?1L<<19:0) |
		((unordered & 1L<<18)?1L<<23:0) |
		((unordered & 1L<<19)?1L<<28:0) |
		((unordered & 1L<<20)?1L<<29:0) |
		/*Min1*/
		((unordered & 1L<<21)?1L<<24:0) |
		((unordered & 1L<<22)?1L<<27:0) |
		((unordered & 1L<<23)?1L<<22:0) |
		((unordered & 1L<<24)?1L<<20:0) |
		((unordered & 1L<<25)?1L<<21:0) |
		((unordered & 1L<<26)?1L<<25:0) |
		((unordered & 1L<<27)?1L<<26:0) |
		//Flashing Cursor
		((unordered & 1L<<28)?1L<<14:0);
}

void updateLEDDisplay() {
	static int num = 0;
	lsr.write(orderLEDDisplay(timeToSevenSegTime(num,num,num)));
	num = (num+1)%60;
}

uint32_t ringCounter32() {
	static uint32_t b = 0b1;
	b = (b << 1)|(b>>31);
	return b;
}

Pin CE = Pin(B,4);
Pin SCLK = Pin(B,2);
Pin IO = Pin(B,0);

RTC rtc = RTC(SCLK,IO,CE);

void loop() {
	lsr.write(ringCounter32());
	_delay_ms(1000);
//	lsr.write(0xAAAAAAAA);
//	_delay_ms(1000);
//	lsr.write(0x55555555);
//	_delay_ms(1000);
//	updateLEDDisplay();
//	_delay_ms(1000);

	UART_tx_str("I'm a UART string!");
	static int i=0;
	UART_tx_uint16(i+=100);

	UART_tx_str("\r\nTime:");
	UART_tx_uint16(rtc.getHour());
	UART_tx_str(" : ");
	UART_tx_uint16(rtc.getMinute());
	UART_tx_str(" : ");
	UART_tx_uint16(rtc.getSecond());
	UART_tx_str("\r\n");


}

int main() {
	rtc.writeCompileTime();
	UART_init();
	while (1) loop();
	return 0;
}
