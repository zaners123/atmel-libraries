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
#include "lib/minimized/russiandht.h"

int* temp = (int*)malloc(sizeof(int));
int* hum = (int*)malloc(sizeof(int));

#define SEG_A  0
#define SEG_B  1
#define SEG_C  2
#define SEG_D  3
#define SEG_E  4
#define SEG_F  5
#define SEG_G  6
#define SEG_DP 7
Pin LED_SEGMENTS[8] = {
		Pin(A,1),//A
		Pin(D,1),//B
		Pin(D,0),//C
		Pin(D,2),//D
		Pin(A,0),//E
		Pin(D,4),//F
		Pin(D,5),//G
		Pin(D,3),//DP
};
#define LED_R1 0
#define LED_G1 1
#define LED_B1 2
#define LED_R2 3
#define LED_G2 4
#define LED_B2 5
Pin LED_DIGITS[6] = {
		Pin(B,7),//R1
		Pin(B,4),//G1
		Pin(B,3),//B1
		Pin(B,2),//R2
		Pin(B,1),//G2
		Pin(B,0),//B2
};

#define DHT11_TIME 1500
#define REFRESH_TIME 2

/**
 *
 * TODO:
 *  - Create SevenSegment class for a 6 digit common cathode display
 *      - Initialize something like SevenSegment(digitArray,6,anodeArray,8)
 *      - Set with something like display.set(0,5) for setting digit 0 to a 5
 * */

void setDigit(int digit, int value) {
	const uint8_t mux[10] = {
	//Bits are set by:
	//    PGFEDCBA
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
	digitalWrite(LED_DIGITS[digit],HIGH);
	for (uint8_t i=0;i<8;i++) digitalWrite(LED_SEGMENTS[i],(mux[value]>>i) & 1);
	_delay_ms(REFRESH_TIME);
	for (uint8_t i=0;i<8;i++) digitalWrite(LED_SEGMENTS[i],false);
	digitalWrite(LED_DIGITS[digit],LOW);
}

uint16_t frameNumber = 0;
void loop() {
	if (frameNumber < DHT11_TIME/REFRESH_TIME / 2) {
		setDigit(LED_R1,(*temp/10) % 10);
		setDigit(LED_R2,(*temp)    % 10);
	} else {
		setDigit(LED_B1,(*hum/10)  % 10);
		setDigit(LED_B2,(*hum)     % 10);
	}

	if (frameNumber >= DHT11_TIME/REFRESH_TIME) {
		if (dhtread(6,hum,temp)) {
			*temp = (int)round(1.8 * ((double)(*temp)/10))+32;
			*hum = (int)round((double)(*hum)/10);
//			UART_tx_str("ACTUAL Temp:");
//			UART_tx_uint8(*temp);
//			UART_tx_str("\nACTUAL Hum :");
//			UART_tx_uint8(*hum);
//			UART_tx_str("\n");
//		} else {
//			UART_tx_str("dht read error\n");
		}
		frameNumber=0;
	}
	frameNumber++;
}

int main() {
	*temp=0;
	*hum=0;
	for (int i=0;i<8;i++) pinMode(LED_SEGMENTS[i],OUTPUT);
	for (int i=0;i<6;i++) pinMode(LED_DIGITS[i],OUTPUT);
//	UART_init();
//	UART_tx_str("START\n");
	while (1) loop();
	return 0;
}