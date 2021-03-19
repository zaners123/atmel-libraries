#ifndef ATMEGA328P_STEPPER_H
#define ATMEGA328P_STEPPER_H

#include "Pins.h"

#define STEPPER_TICKS_PER_ROTATION 2048L

/**
 * Controls stepper motor. Wire this through a transistor array (or else you'll kill the poor pins)!
 *
 * todo could do half-stepping (bool halfstep)
 * */
class Stepper {

	bool writingSomething;

	Pin IN1;
	Pin IN2;
	Pin IN3;
	Pin IN4;
public:
	void stepM(bool clockwise) {
		static int i=0;
		if (clockwise) {
			i--;
			if (i<0)i=3;
		} else {
			i++;
			if (i>=4)i=0;
		}
		digitalWrite(IN1,i==0);
		digitalWrite(IN2,i==1);
		digitalWrite(IN3,i==2);
		digitalWrite(IN4,i==3);
		writingSomething = true;
		_delay_ms(3);
	}

	void turn(long steps) {
		bool clockwise = steps>0;
		steps = steps>=0?steps:-steps;
		for (int step=0;step<steps;step++) stepM(clockwise);
	}

	Stepper(Pin IN1, Pin IN2, Pin IN3, Pin IN4) : IN1(IN1),IN2(IN2),IN3(IN3),IN4(IN4) {
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
		writingSomething = true;//defaults to true, just in case the pins are on
	}

	/**Ok you can stop driving the motor now
	 *
	 * Call this when you're done cause it doesn't need to be always dumping that holding torque
	 * */
	void stopWriting() {
		if (writingSomething) {
			digitalWrite(IN1,false);
			digitalWrite(IN2,false);
			digitalWrite(IN3,false);
			digitalWrite(IN4,false);
			//all done
			writingSomething = false;
		}
	}
};

#endif //ATMEGA328P_STEPPER_H
