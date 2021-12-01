# TODO

 - Solder everything
 - Install into sprinkler box and wire
 - Code advanced stuff like rain detectors
 - Get AVR ICE and use it on stuff like the "ATMEGA808"

# ATTiny4313

For serial:
 - Connect FTDI's RX pin to the programmer's MISO pin (arduino's pin 50)
 - Use the minimized/TinyUart.h library 
 - Call UART_init 
 - Call UART_tx_str

```bash
stty -F /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0 9600
cat /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
```

Connecting from terminal:
```bash
# 1. Get STTY to say this
bean@bean:~$ stty -F /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K0CV6-if00-port0 
speed 115200 baud; line = 0;
min = 100; time = 2;
-icrnl -imaxbel
-opost -onlcr
-isig -icanon -echo

stty -F /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K0CV6-if00-port0 115200 -isig -icanon -echo -opost -onlcr -icrnl -imaxbel min 100 time 2

stty -F /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0 9600 -isig -icanon -echo -opost -onlcr -icrnl -imaxbel min 100 time 2

# 2. Run these
cat /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K0CV6-if00-port0 
printf "AT\r\n" > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AG0K0CV6-if00-port0


```

# ESP8266 useful commands

## Initialization

To initialize the ESP8266, you have to configure it with things such as a baud rate, your WiFi password, etc.
The default baud rate is 115200, so you initally have to run these at that baud rate, but after setting DEF (default) variables once, they stay set.

```bash
printf "AT+CWMODE_DEF=1\r\n" > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
printf 'AT+CWJAP_DEF="wifiname","wifipassword"\r\n' > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
printf "AT+UART_DEF=9600,8,1,0,0\r\n" > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
```

## To get sprinklers that are on

Use this to test a complete web request - note to use "wc -c" to recalculate bytes for CIPSEND if you change the URL

```bash
printf 'AT+CIPSTART="TCP","73.140.213.122",51919\r\n' > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
printf 'AT+CIPSEND=78\r\n' > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
printf 'GET /?c=123123123123 HTTP/1.1\r\nHost: datadeer.net:51919\r\nUser-Agent:pcbs\r\n\r\n\r\n' > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
printf 'AT+CIPCLOSE\r\n' > /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
```
AT+UART_CUR sets current UART info such as a new baud rate

AT+UART_CUR=9600,8,1,0,0

9600 baud rate aka speed
8 data bits (cs8)
1 stop bits
0 so no parity
0 so no flow control

# FTDI232

DTR
RX
TX
VCC
CTS
GND

# Adafruit one

RTS
RX
TX
VCC
CTS
GND