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
#include "lib/minimized/TinyUart.h"

void loop() {
	UART_tx_str("Buenos Dias\n");
	_delay_ms(1000);
}

int main() {
	UART_init();
	UART_tx_str("START\n");
	while (1) loop();
	return 0;
}