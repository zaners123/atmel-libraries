FILENAME   = main

FILEA = main.cpp

PORT       = /dev/ttyACM0
DEVICE     = attiny4313
PROGRAMMER = arduino
BAUD       = 19200
 # COMPILE    = avr-g++ -Wall -Os -I arduino/avr/libraries/SoftwareSerial/src -I arduino -I arduino/avr/libraries -I arduino/avr/cores/arduino -mmcu=$(DEVICE)
COMPILE    = avr-g++ -Wall -Os -mmcu=$(DEVICE)

default: compile upload clean
all: compile

compile:
	 # Each individually
	$(COMPILE) -c $(FILEA) -o $(FILEA).o
	 # Combining
	$(COMPILE) -o $(FILENAME).elf $(FILEA).o
	 # Converting
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

verifyconnection:
	# verifying connection
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD)

upload:
	avrdude -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i
	 # avrdude -v -v -v -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i

	 # To get FT232R to work:
	 # Device program not enabled
	 # sudo avrdude -v -v -v -v -p atmega328p -c ft232r -Pusb:ft0

	 # Adafruit avrdude: ft245r_open(): invalid device identifier '        '
	 # avrdude -v -v -v -v -p atmega328p -c arduino-ft232r -P /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K0CV6-if00-port0

	 # Command that works with Arduino Mega:
	 # avrdude -v -v -v -v -p attiny85 -c arduino -P /dev/ttyACM0 -b 19200 -U flash:w:easy_blink.hex:i

clean:
	rm $(FILEA).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex
