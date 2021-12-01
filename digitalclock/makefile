FILENAME   = main

FILEA = main.cpp

PORT       = /dev/serial/by-id/usb-FTDI_USB_Serial_Converter_FTCGVOD6-if00-port0
DEVICE     = attiny85
PROGRAMMER = stk500
BAUD	   :=
COMPILE    = avr-gcc -Wall -Os -s -fdata-sections -ffunction-sections -mmcu=$(DEVICE) -Wl,--gc-sections

default: compile upload clean
all: compile

compile:
	$(COMPILE) -c $(FILEA) -o $(FILEA).o
	$(COMPILE) -o $(FILENAME).elf $(FILEA).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

verifyconnection:
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) $(BAUD)

upload:
	avrdude -e -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) $(BAUD) -U flash:w:$(FILENAME).hex:i
clean:
	rm $(FILEA).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
