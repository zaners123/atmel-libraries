FILENAME   = main

FILEA = main.cpp

PORT       = /dev/ttyACM0
DEVICE     = attiny4313
PROGRAMMER = arduino
BAUD       = 19200
COMPILE    = avr-g++ -Wall -Os -s -mmcu=$(DEVICE)

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
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i
clean:
	rm $(FILEA).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
