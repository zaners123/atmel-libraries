#ifndef ATMEGA328P_RTC_H
#define ATMEGA328P_RTC_H

#include "Pins.h"
#include "NativeUart.h"

#define BURST_BYTES 8

#define SECONDS_TO_COMPILE 11 // offset from __TIME__

#define byte uint8_t

class RTC {

	Pin SCLK;//"CLK"
	Pin IO;//"DAT"
	Pin CE;//"RST"

public:
	RTC(Pin sclk, Pin io, Pin ce): SCLK(sclk),IO(io),CE(ce) {

		//init pins/stuff
		pinMode(CE,OUTPUT);
		pinMode(SCLK,OUTPUT);
		//writeCompileTime();
		//try reading some register
		//Serial.println("Writing");
		//rtcWrite(0x8C,0x20);//year 2020
		//Serial.println("Reading one");
		//Serial.println(rtcRead(0x8D),HEX);
		//write to RTC
		/*Serial.println("BurstRead");
		byte allbytes[BURST_BYTES];
		rtcBurstRead(allbytes);
		for (int i=0;i<BURST_BYTES;i++) {
		  Serial.println(allbytes[i],HEX);
		}*/
	}

	void rtcAllowIO() {digitalWrite(CE,HIGH);}
	void rtcDenyIO() {digitalWrite(CE,LOW);}

	/**
	 * Frequently off by roughly 6 seconds. Likely since it gets time when you hit run, but sets time when fully uploaded and restarted
	*/
	void writeCompileTime() {
		//todo I think this doesnt work / wasnt finished
		byte ct[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		rtcCompileTime(ct);
		/* todo for (int i=0;i<BURST_BYTES;i++) {
			print("FOR BYTE ");Serial.print(i);
			print("\tYOU WROTE");Serial.println(ct[i],HEX);
		}*/
		rtcBurstWrite(ct);
	}

	byte rtcRead(byte addr) {
		rtcAllowIO();
		//send request
		pinMode(IO,OUTPUT);shiftOut(IO,SCLK,LSBFIRST,addr);
		pinMode(IO,INPUT);
		//get response
		byte toRecieve = shiftIn(IO,SCLK,LSBFIRST);
		rtcDenyIO();
		return toRecieve;
	}

	/**readTo needs to be length BURST_BYTES*/
	void rtcBurstRead(byte readTo[]) {
		rtcAllowIO();

		//send request
		pinMode(IO,OUTPUT);shiftOut(IO,SCLK,LSBFIRST,0xBF);

		//read responce
		pinMode(IO,INPUT);
		for (int i=0;i<BURST_BYTES;i++) {
			readTo[i] = shiftIn(IO,SCLK,LSBFIRST);
		}
		rtcDenyIO();
	}

	void rtcWrite(byte addr, byte data) {
		rtcAllowIO();

		//send
		pinMode(IO,OUTPUT);
		shiftOut(IO,SCLK,LSBFIRST,addr);
		shiftOut(IO,SCLK,LSBFIRST,data);

		rtcDenyIO();
	}

	/**Data must be of length BURST_BYTES*/
	void rtcBurstWrite(byte data[]) {
		rtcAllowIO();

		//send
		pinMode(IO,OUTPUT);
		shiftOut(IO,SCLK,LSBFIRST,0xBE);
		for (int i=0;i<BURST_BYTES;i++) {
			shiftOut(IO,SCLK,LSBFIRST,data[i]);
		}

		rtcDenyIO();
	}

	static void rtcCompileTime(byte data[]) {
		const char cd[] = __DATE__ " " __TIME__;

		int second      = 10*(cd[18]-'0')+(cd[19]-'0');
		int minute      = 10*(cd[15]-'0')+(cd[16]-'0');
		int hour        = 10*(cd[12]-'0')+(cd[13]-'0');
		int dayOfMonth  = 10*(cd[ 4]-'0')+(cd[ 5]-'0');

		second += SECONDS_TO_COMPILE;
		if (second>=60) {
			second-=60;
			minute++;
			if (minute>=60) {
				minute-=60;
				hour++;
				if (hour>=24) {
					hour-=24;
					dayOfMonth++;
					//todo could make it so if you run this at 0:00:00, it doesn't explode
				}
			}
		}

		//todo compile time offset (need to parse as base 10 THEN into data[] fields

		data[0] = ((second     /10)<<4) | (second    %10);//second
		data[1] = ((minute     /10)<<4) | (minute    %10);//minute
		data[2] = ((hour       /10)<<4) | (hour      %10);//hour
		data[3] = ((dayOfMonth /10)<<4) | (dayOfMonth%10);//day-of-month
		data[4] =
				cd[0]=='J'?(cd[1]=='a'?0x01:(cd[2]=='n'?0x06:0x07)):
				cd[0]=='F'?0x02:
				cd[0]=='M'?(cd[2]=='r'?0x03:0x05):
				cd[0]=='A'?(cd[1]=='p'?0x04:0x08):
				cd[0]=='S'?0x09:
				cd[0]=='O'?0x10:
				cd[0]=='N'?0x11:
				0x12;
		data[5] = 0x01;//todo how do I get weekday
		data[6] = ((cd[9]-'0')<<4) | (cd[10]-'0');

		//old system -- without offset
		/*data[0] = ((cd[18]-'0')<<4) | (cd[19]-'0');//second
		data[1] = ((cd[15]-'0')<<4) | (cd[16]-'0');//minute
		data[2] = ((cd[12]-'0')<<4) | (cd[13]-'0');//hour
		data[3] = ((cd[ 4]-'0')<<4) | (cd[ 5]-'0');//day-of-month
		data[4] =
				cd[0]=='J'?(cd[1]=='a'?0x01:(cd[2]=='n'?0x06:0x07)):
				cd[0]=='F'?0x02:
				cd[0]=='M'?(cd[2]=='r'?0x03:0x05):
				cd[0]=='A'?(cd[1]=='p'?0x04:0x08):
				cd[0]=='S'?0x09:
				cd[0]=='O'?0x10:
				cd[0]=='N'?0x11:
				0x12;
		data[5] = 0x01;//todo how do I get weekday
		data[6] = ((cd[9]-'0')<<4) | (cd[10]-'0');*/
	}

	/*void print() {
		Serial.println("BurstRead");
		byte allbytes[BURST_BYTES];
		rtcBurstRead(allbytes);
		for (int i=0;i<BURST_BYTES;i++) {
			Serial.println(allbytes[i],HEX);
		}
		delay(1000);
	}*/
	/*RETRIEVAL FUNCTIONS********************/
	byte getSecond() {
		byte read = rtcRead(0x81);
		return read%16 + (read>>4)*10;
	}

	byte getMinute() {
		byte read = rtcRead(0x83);
		return read%16 + (read>>4)*10;
	}

	byte getHour() {
		byte read = rtcRead(0x85);
		return read%16 + (read>>4)*10;
	}
};
#endif //ATMEGA328P_RTC_H
