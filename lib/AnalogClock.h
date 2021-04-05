#ifndef ATMEGA328P_ANALOGCLOCK_H
#define ATMEGA328P_ANALOGCLOCK_H

#include "Stepper.h"
#include "RTC.h"
#include "Pins.h"
#include "NativeUart.h"

#define SET_DATE false
#define HOURS_PER_RESET 12L
#define TICKS_PER_RESET STEPPER_TICKS_PER_ROTATION * HOURS_PER_RESET

long max(long a, long b) {return a>b?a:b;}
long min(long a, long b) {return a<b?a:b;}

class AnalogClock {

	Pin buttonInc;
	Pin buttonDec;
	RTC rtc;
	Stepper stepper;

	bool initializedHands;


	bool readButtonDec() {return LOW==digitalRead(buttonDec);}
	bool readButtonInc() {return LOW==digitalRead(buttonInc);}

	void update() {
		static long ticksPastZero = 0;

		int hour = rtc.getHour()%((uint8_t)HOURS_PER_RESET);
		int minute = rtc.getMinute();

//		printString("hour: \"");printl(hour);printString("\"\n");
//		printString("minute: \"");printl(minute);printString("\"\n");

//		long moveToTick =(STEPPER_TICKS_PER_ROTATION * hour)+((STEPPER_TICKS_PER_ROTATION * minute) / 60);
		long moveToTick = (long)((hour*60+minute) * (((double)STEPPER_TICKS_PER_ROTATION)/60));
//		printString("moveToTick: \"");printl(moveToTick);printString("\"\n");

		long moveBy = moveToTick - ticksPastZero;

		//prevent it from unnecessarily going around the world
		moveBy = max(min(moveBy,STEPPER_TICKS_PER_ROTATION),-STEPPER_TICKS_PER_ROTATION);

		moveToTick = moveBy + ticksPastZero;
//		printString("moveBy: \"");printl(moveBy);printString("\"\n");
		stepper.turn(moveBy);
		ticksPastZero = moveToTick;
//		printString("ticksPastZero: \"");printul(ticksPastZero);printString("\"\n");

		//printString("\n\n");

		//saves battery life maybe? I dunno
		_delay_ms(500);

	}

public:

	RTC& getRTC() {return rtc;}
	Stepper& getStepper() {return stepper;}
	Pin& getButtonInc() {return buttonInc;}
	Pin& getButtonDec() {return buttonDec;}

	AnalogClock(Pin inc, Pin dec, Pin rtcSclk, Pin rtcIo, Pin rtcCe, Pin stepper1, Pin stepper2, Pin stepper3, Pin stepper4)
		: buttonInc(inc), buttonDec(dec),
			rtc(rtcSclk, rtcIo, rtcCe), stepper(stepper1, stepper2, stepper3, stepper4),
		initializedHands(false) {

		pinMode(buttonInc, INPUT_PULLUP);
		pinMode(buttonDec, INPUT_PULLUP);

		if (SET_DATE) {
			rtc.writeCompileTime();
			while(1) {
				_delay_ms(1000);
			}
		}
	}

	/**Put this in your loop() function (or just call it at least every minute, preferably as often as you can*/
	void tick() {
		if (initializedHands) {
			if (readButtonDec() || readButtonInc()) {
				//todo could add clock-offset feature for if the clock was wrong
			}
			update();
		} else {
			while ( readButtonDec() && !readButtonInc()) stepper.turn(-1);
			while (!readButtonDec() &&  readButtonInc()) stepper.turn(1);
			if (    readButtonDec() &&  readButtonInc()) initializedHands=true;
		}
		stepper.stopWriting();
	}

};
#endif //ATMEGA328P_ANALOGCLOCK_H
