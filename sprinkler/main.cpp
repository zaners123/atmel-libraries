#define F_CPU 1000000UL

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/builtins.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "lib/Pins.h"
#include "lib/minimized/TinyUSART.h"
#define NUM_STATIONS 8

//NOTE: Use PSTR to force strings into ROM (4KB) instead of RAM (512 bytes)
#define nullptr 0

Pin UC_RELAY[NUM_STATIONS] = {
	Pin(D,2),//1
	Pin(D,3),//2
	Pin(D,4),//3
	Pin(B,4),//4
	Pin(B,3),//5
	Pin(B,2),//6
	Pin(B,1),//7
	Pin(B,0),//8
};

/**
 * Sends web request, gets result, parses result into list of sprinkler heads to set.
 * @returns char[NUM_STATIONS] either containing 0 or 1
 * */
const unsigned char* requestStations() {
//	const unsigned char CIPSTART[] PROGMEM  = "AT+CIPSTART=\"TCP\",\"73.140.213.122\",51919\r\n";
//	const unsigned char CIPSEND[] PROGMEM   = "AT+CIPSEND=61\r\n";*//*Change the bytes if you change the GET string*//*
//	const unsigned char CIPDATA[] PROGMEM   = "GET /?c=123123123123 HTTP/1.1\r\nHost: datadeer.net:51919\r\n\r\n\r\n";

	//Start connection, return nullptr if you can't start connection
	if (
		!expectOk(CIPSTART) ||
		!expectOk(CIPSEND)
		) {
		return nullptr;
	}
	USART0_TX_str_pgm(CIPDATA);
	unsigned char* ret = USART0_RX_bracket();
	//verify output and return
	for (uint8_t i=0;i<NUM_STATIONS;i++) {
		if (ret[i]=='\0' || (ret[i]!='0' && ret[i]!='1')) {
			return nullptr;
		}
	}
	ret[NUM_STATIONS] = '\0';
	return ret;
}

void setRelay(uint8_t number, bool enable) {

}
/**
 * The sprinkler, on loop does this:
 *  1. HTTP GETs to datadeer.net to get every sprinkler state:
 *      a. If it can't connect 10 times in a row it turns off all lines.
 *      b. If it recieves the sprinkler states, it sets that on the lines.
 * */
void loop() {

	static int errorsInARow = 0;
	const unsigned char* stations = requestStations();

	if (stations) {
		errorsInARow = 0;
		USART0_TX_ustr(stations);
		USART0_TX_str_pgm(MESSAGE_STAT);
		for (int i=0;i<NUM_STATIONS;i++) {
			setRelay(i,stations[i]=='1');
		}
	} else {
		errorsInARow++;
		if (errorsInARow < 10) {
			USART0_TX_str_pgm(MESSAGE_INVALID);
		} else {
			USART0_TX_str_pgm(MESSAGE_LOST_CONNECTION);
			for (int i=0;i<NUM_STATIONS;i++) {
				setRelay(i,false);
			}
		}
	}

	/*It is automatically closed by the server, but you might as well be extra-sure.
	 * This returns error if it's already closed, so don't bother checking if it returns an error
	 */
	USART0_TX_str_pgm(CIPCLOSE);

	_delay_ms(5000);
}

int main() {
	USART0_Init();
	while (1) loop();
	return 0;
}