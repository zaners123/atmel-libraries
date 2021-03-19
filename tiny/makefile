FILENAME   = main

FILEA = main.cpp
FILEB = BasicSerial/BasicSerial.S

PORT       = /dev/ttyACM0
DEVICE     = attiny85
PROGRAMMER = arduino
BAUD       = 19200
COMPILE    = avr-g++ -Wall -Os -I arduino/avr/libraries/SoftwareSerial/src -I arduino -I arduino/avr/libraries -I arduino/avr/cores/arduino -mmcu=$(DEVICE)

default: compile upload clean
all: compile

compile:
	 # Each individually
	$(COMPILE) -c $(FILEA) -o $(FILEA).o
	$(COMPILE) -c $(FILEB) -o $(FILEB).o
	 # Combining
	$(COMPILE) -o $(FILENAME).elf $(FILEA).o $(FILEB).o
	 # Converting
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i
	 # avrdude -v -v -v -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i


 # Command that works with Arduino Mega:
 # avrdude -v -v -v -v -p attiny85 -c arduino -P /dev/ttyACM0 -b 19200 -U flash:w:easy_blink.hex:i

clean:
	rm $(FILEA).o
	rm $(FILEB).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
