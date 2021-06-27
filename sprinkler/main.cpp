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
#include "lib/Pins.h"
#include "lib/minimized/TinyUSART.h"

void loop() {
/*	GOAL

    1. Get Wifi chip all set up:
        a. UART send something like "use this wifi, etc etc" and get back "OK" every time
    2. Loop:
        a. Download something like "datadeer.net/s?u={12 character code or somethin}"
        b. Recieve back and use USAR0_RX_bracket() to parse out which valves should be open

 */
//	USART0_Transmit( 'X' );
//	USART0_Transmit( USART0_Receive() );
//	USART0_Transmit( '\n' );
	USART0_TX_str("Buenos Dias\n");
	char* bracket_data = USART0_RX_bracket();

	USART0_TX_str(bracket_data);
	_delay_ms(1000);
}

int main() {
	USART0_Init();
//	UART_tx_str("START\n");
	while (1) loop();
	return 0;
}