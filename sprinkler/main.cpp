#define F_CPU 1000000UL

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <avr/sleep.h>
//#include <avr/wdt.h>
//#include <avr/builtins.h>
#include <stdint.h>
//#include "string.h"

#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "lib/Pins.h"
#include "lib/minimized/TinyUSART.h"
#define NUM_STATIONS 8

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

//Set to true after connecting to WIFI and stuff. Set to false on any ERRORs or timeouts
bool initialized = false;

//Do something like:
// #define WIFI_CREDS "AT+CWJAP=\"host\",\"pass\""

/**
 * Sends web request, gets result, parses result into list of sprinkler heads to set
 *
 * */
const char* requestStations() {
	//TODO move to startup; only run on errors to reset


	const char* webrequest = "AT+SOMETHING http://datadeer.net:51919/?c=123123123123\r\n";
	USART0_TX_str(webrequest);//todo
	char* bracket_data = USART0_RX_bracket();
	USART0_TX_str(bracket_data);
	const char* tmp = "00001111";
	return tmp;
}
/**
 * TODO it is obviously mission critical that you
 *   SHUT OFF sprinklers if connection is lost (EX: for over 10 seconds)
 *   1. Get Wifi chip all set up:
 *          a. Might have to verify this, but looks like all setup is done in DEF variables
 * 	        b. UART send something like "use this wifi, etc etc" and get back "OK" every time
 * 	    2. Loop:
 * 	        a. Download something like "datadeer.net/s?u={12 character code or somethin}"
 * 	        b. Recieve back and use USAR0_RX_bracket() to parse out which valves should be open
 *
 * */
void loop() {
	//NOTE: Use PSTR to force strings into ROM (4KB) instead of RAM (512 bytes)
	USART0_TX_str(PSTR("AT+SOMETHING http://73.140.213.122:51919/?c=123123123123"));

	expectOk("AT\r\n");

	/*	GOAL
	     */
	const char* stations = requestStations();
	for (int i=0;i<NUM_STATIONS;i++) {
		if (stations[i]=='\0') {
			USART0_TX_str("V Invalid Bracketstr\n");
			USART0_TX_str(stations);
			USART0_TX_str("^ Invalid Bracketstr\n");
		}
		if (stations[i]=='0') {
			USART0_TX_str("Station ");
			USART0_Transmit('0'+i);
			USART0_TX_str(" off\n");
		} else if (stations[i]=='1') {
			USART0_TX_str("Station ");
			USART0_Transmit('0'+i);
			USART0_TX_str(" on\n");
		}
	}
	_delay_ms(5000);
}

int main() {
	USART0_Init();
	while (1) loop();
	return 0;
}