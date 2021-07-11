//Use a Polled UART, since it is smaller and would be more intuitive for this use

// https://ww1.microchip.com/downloads/en/Appnotes/Atmel-1451-Using-the-AVR-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf

//#define UBRR 25 // (1000000/16/2400)−1
//#define UBRR 12 // (1000000/16/4800)−1

//#define UBRR 51 // (1000000/8/2400)−1
//#define UBRR 25 // (1000000/8/4800)−1
#define UBRR 12 // (1000000/8/9600)−1

#define RX_TIMEOUT 100000

/* Initialize UART */
void USART0_Init() {
	/* Set the baud rate */
	UBRRH = (unsigned char) (UBRR>>8);
	UBRRL = (unsigned char) UBRR;

	/* Set to double-speed. You can only use this when running asynchronously.
	 * You also need to use /8 in the UBRR calculation not /16
	 * */
	UCSRA = (1<<U2X);
	/* Enable UART receiver and transmitter */
	UCSRB = ( ( 1 << RXEN ) | ( 1 << TXEN ) );
	/* Set frame format: 8 data 2stop */
//	UCSRC = (1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1);
	/* Set frame format: 8 data 1stop */
	UCSRC = (1<<UCSZ0)|(1<<UCSZ1);
}
/* Read and write functions */
bool rxTimedOut;
unsigned char USART0_Receive() {
	/* Wait for incoming data */
	//todo if this loops over x times, set stillRecieve to false
	static int timeout = RX_TIMEOUT;
	while ( !(UCSRA & (1<<RXC)) && (timeout-->0));
	/* Return the data */
	return UDR;
}
#define BUFSIZE 32 // Entire chip has only 256 bytes of RAM
char rx_buffer[BUFSIZE];
unsigned char buffer_index;

char* USART0_RX_bracket() {
	buffer_index = 0;
	rxTimedOut = false;
	//find starter quote
	while(!rxTimedOut && USART0_Receive() != '{');
	//read till end quote
	while(!rxTimedOut) {
		rx_buffer[buffer_index] = USART0_Receive();
		if (buffer_index==BUFSIZE-1 || rx_buffer[buffer_index]=='\0' || rx_buffer[buffer_index]=='}') {
			break;
		}
		buffer_index++;
	}
	rx_buffer[buffer_index] = '\0';
	return rx_buffer;
}

void USART0_Transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while ( !(UCSRA & (1<<UDRE)) );
	/* Start transmittion */
	UDR = data;
}
void USART0_TX_str(const char* string){
	//until null char
	while(*string){
		USART0_Transmit( *string++ );
	}
}

/**
 * Send request over UART. Expect either OK or ERROR.
 *  If it receives OK, return true
 *  If it recieves ERROR or times out, return false
 */
bool expectOk(const char* command) {
	USART0_TX_str("AT\r\n");
}