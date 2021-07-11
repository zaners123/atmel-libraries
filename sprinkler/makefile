FILENAME   = main

FILEA = main.cpp

PORT       = /dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_95635333331351F011E1-if00
DEVICE     = attiny4313
PROGRAMMER = arduino
BAUD       = 19200
COMPILE    = avr-gcc -Wall -Os -s -fdata-sections -ffunction-sections -mmcu=$(DEVICE) -Wl,--gc-sections

default: compile upload clean
all: compile

compile:
	$(COMPILE) -c $(FILEA) -o $(FILEA).o
	$(COMPILE) -o $(FILENAME).elf $(FILEA).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

verifyconnection:
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD)

upload:
	avrdude -e -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i
clean:
	rm $(FILEA).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
