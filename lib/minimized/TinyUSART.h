//Use a Polled UART, since it is smaller and would be more intuitive for this use
//#define UBRR (unsigned int)(F_CPU / 8 / 9600 - 1)

// https://ww1.microchip.com/downloads/en/Appnotes/Atmel-1451-Using-the-AVR-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf

#define UBRR 12

/* Initialize UART */
void USART0_Init() {
	/* Set the baud rate */
	UBRRH = (unsigned char) (UBRR>>8);
	UBRRL = (unsigned char) UBRR;
	/* Enable UART receiver and transmitter */
	UCSRB = ( ( 1 << RXEN ) | ( 1 << TXEN ) );
	/* Set frame format: 8 data 2stop */
	//UCSRC = (1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1);
	/* Set frame format: 8 data 1stop */
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1);
}
/* Read and write functions */
bool stillReceive;
unsigned char USART0_Receive() {
	/* Wait for incoming data */
	//if this loops over x times, set stillRecieve to true
	while ( !(UCSRA & (1<<RXC)) );
	/* Return the data */
	return UDR;
}
#define BUFSIZE 128 // Entire chip has only 256 bytes of RAM
char buffer[BUFSIZE];
unsigned char buffer_index;
#define MAXPAUSE 5000
char* USART0_RX_bracket() {
	buffer_index = 0;
	stillReceive = true;
	//find starter quote
	while(stillReceive && USART0_Receive() != '{');
	//read till end quote
	while(stillReceive) {
		buffer[buffer_index] = USART0_Receive();
		if (buffer_index==BUFSIZE-1 || buffer[buffer_index]=='\0' || buffer[buffer_index]=='}') {
			break;
		}
		buffer_index++;
	}
	buffer[buffer_index] = '\0';
	return buffer;
}

void USART0_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */
	while ( !(UCSRA & (1<<UDRE)) );
	/* Start transmittion */
	UDR = data;
}
void USART0_TX_str(const char* string){
	//until null char
	while( *string ){
		//send a char
		USART0_Transmit( *string++ );
		//wait until transmission is finished
	}
}