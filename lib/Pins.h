#ifndef ATMEGA328P_PINS_H
#define ATMEGA328P_PINS_H

enum Level {HIGH=true,LOW=false};
enum BitOrder {LSBFIRST,MSBFIRST};
enum PinMode {
	INPUT
	,OUTPUT
	,INPUT_PULLUP
};
enum PortLetter {
#ifdef DDRA
	A,
#endif

	B,
#ifdef DDRC
	C,
#endif

#ifdef DDRD
	D,
#endif
};
/**
 * Pin(PORTB,PINB0)
 * */
class Pin {
public:
	volatile uint8_t* ddr;
	volatile uint8_t* port;
	volatile uint8_t* pin;
	volatile uint8_t pinNumber;
	Pin(
			volatile uint8_t& ddr,
			volatile uint8_t& port,
			volatile uint8_t& pin,
			volatile uint8_t pinNumber) {
		this->ddr       =&ddr;
		this->port      =&port;
		this->pin       =&pin;
		this->pinNumber =pinNumber;
	}
	Pin(PortLetter portLetter, uint8_t pinNumber) {
		switch (portLetter) {
#ifdef DDRA
			case A:this->ddr=&DDRA;this->port=&PORTA;this->pin=&PINA;break;
#endif
#ifdef DDRB
			case B:this->ddr=&DDRB;this->port=&PORTB;this->pin=&PINB;break;
#endif
#ifdef DDRC
			case C:this->ddr=&DDRC;this->port=&PORTC;this->pin=&PINC;break;
#endif
#ifdef DDRD
			case D:this->ddr=&DDRD;this->port=&PORTD;this->pin=&PIND;break;
#endif
		}
		this->pinNumber =pinNumber;
	}
};

unsigned char getPinBIT(unsigned char pin) {
	return (1<<pin);
}

void pinMode(Pin pin, PinMode mode) {
	/* (megaAVR® Data Sheet 14.2.1)
	 * The DDxn bit in the DDRx Register selects the direction of this pin.
	 *  - If DDxn is written logic one, Pxn is configured as an output pin.
	 *  - If DDxn is written logic zero, Pxn is configured as an input pin.
	 *
	 * */
	switch (mode) {
		case OUTPUT:
			//set as output
			*pin.ddr |= getPinBIT(pin.pinNumber);
			break;
		case INPUT:
			//set as input
			*pin.ddr  &= ~getPinBIT(pin.pinNumber);
			//turn off pullup
			*pin.port &= ~getPinBIT(pin.pinNumber);
			break;
		case INPUT_PULLUP:
			//set as input
			*pin.ddr  &= ~getPinBIT(pin.pinNumber);
			//turn on pullup
			*pin.port |= getPinBIT(pin.pinNumber);
			break;
	}

}

void digitalWrite(Pin pin, bool level) {
	//toggle Data Register for PORTB
	//  (It's a write pin now, so writing to this register sets the pin)
	if (level) {
		*pin.port |=    getPinBIT(pin.pinNumber);
	} else {
		*pin.port &=    ~getPinBIT(pin.pinNumber);
	}
}

void digitalWrite(Pin pin, Level value) {
	digitalWrite(pin,value==HIGH);
}


Level digitalRead(Pin pin) {
	return (*pin.pin & getPinBIT(pin.pinNumber))?HIGH:LOW;
}

uint8_t shiftIn(Pin dataPin, Pin clockPin, BitOrder bitOrder) {
	uint8_t value = 0;
	uint8_t i;
	for (i = 0; i < 8; ++i) {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST ) {
			value |= digitalRead(dataPin) << i;
		} else if (bitOrder==MSBFIRST) {
			value |= digitalRead(dataPin) << (7 - i);
		}
		digitalWrite(clockPin, LOW);
	}
	return value;
}

void shiftOut(Pin dataPin, Pin clockPin, BitOrder bitOrder, uint8_t val)
{
	uint8_t i;
	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, val & 1);
			val >>= 1;
		} else if (bitOrder==MSBFIRST) {
			digitalWrite(dataPin, (val & 128) != 0);
			val <<= 1;
		}
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

#endif //ATMEGA328P_PINS_H