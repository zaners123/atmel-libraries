# Some Cool Microcontroller Projects

![](humidity/humidity.png)

This is all of my work so far on learning / making microcontroller stuff! If you are familiar with Atmel microcontrollers, I'd appreciate help/suggestions/pull requests!

It is primarily two parts: Code and Kicad PCB design.

## Atmel Processors Used

The clock was made using an ATMega328p, while the humidity sensor and sprinkler were made using an ATTiny4313.

## Code
The code is all compilable with plain old avr-g++ using makefiles.

To compile/install/etc:
1. Change directory into the desired project (such as "cd humidity")
2. Edit the makefile if you plan on using a different microcontroller or connection.
I currently just have it plugged into an arduino as an In-System Programmer using the ATTiny In-System Programmer SPI port.
- ```make verifyconnection``` to test if your microcontroller is connected
3. 
- ```make compile``` to compile
- ```make default``` to build, install, and clean.

## Kicad PCB Design

Not yet in Github project (Feel free to ask for me to add it!)