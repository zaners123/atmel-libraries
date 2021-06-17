# ATMega328P



# ATTiny4313

For serial:
 - Connect FTDI's RX pin to the programmer's MISO pin (arduino's pin 50)
 - Use the minimized/TinyUart.h library 
 - Call UART_init 
 - Call UART_tx_str

```bash
stty -F/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0 9600
cat /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A50285BI-if00-port0
```

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