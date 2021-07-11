//Use a Polled UART, since it is smaller and would be more intuitive for this use

// https://ww1.microchip.com/downloads/en/Appnotes/Atmel-1451-Using-the-AVR-USART-on-tinyAVR-and-megaAVR-devices_ApplicationNote_AVR306.pdf

//#define UBRR 25 // (1000000/16/2400)−1
//#define UBRR 12 // (1000000/16/4800)−1

//#define UBRR 51 // (1000000/8/2400)−1
//#define UBRR 25 // (1000000/8/4800)−1
#define UBRR 12 // (1000000/8/9600)−1

#define RX_TIMEOUT (long)750000 // about 5 seconds

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
	//todo if this loops over x times, set stillReceive to false
	long timeout = RX_TIMEOUT;
	while ( !(UCSRA & (1<<RXC)) && (0 <-- timeout));
	rxTimedOut = timeout==0;
	/* Return the UART data*/
	return UDR;
}
void USART0_Transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while ( !(UCSRA & (1<<UDRE)) );
	/* Start transmission */
	UDR = data;
}
void USART0_TX_str(const char* string){
	//until null char
	while(*string){
		USART0_Transmit( *string++ );
	}
}
void USART0_TX_ustr(const unsigned char* string){
	//until null char
	while(*string){
		USART0_Transmit( *string++ );
	}
}

#define CIPSTART 0
const char string_0[] PROGMEM = "AT+CIPSTART=\"TCP\",\"73.140.213.122\",51919\r\n";
#define CIPSEND 1
const char string_1[] PROGMEM = "AT+CIPSEND=61\r\n";
#define CIPDATA 2
const char string_2[] PROGMEM = "GET /?c=123123123123 HTTP/1.1\r\nHost: datadeer.net:51919\r\n\r\n\r\n";
#define CIPCLOSE 3
const char string_3[] PROGMEM = "AT+CIPCLOSE\r\n";
#define MESSAGE_STAT 4
const char string_4[] PROGMEM = "=STAT\r\n";
#define MESSAGE_LOST_CONNECTION 5
const char string_5[] PROGMEM = "NOCONN\r\n";
#define MESSAGE_INVALID 6
const char string_6[] PROGMEM = "INV\r\n";
PGM_P const string_table[] PROGMEM = {
		string_0,
		string_1,
		string_2,
		string_3,
		string_4,
		string_5,
		string_6,
};
void USART0_TX_str_pgm(const uint8_t id){
	char buffer[65];
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[id])));
	USART0_TX_str(buffer);
}

#define BUFSIZE 12 // Entire chip has only 256 bytes of RAM
unsigned char rx_buffer[BUFSIZE];
unsigned char* USART0_RX_bracket() {
	unsigned char buffer_index = 0;
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

/**
 * Sends specified request over UART. Then reads uart until either "OK\r\n", "ERROR\r\n", or a timeout.
 *  If it receives "OK\r\n", return true
 *  If it receives "ERROR\r\n" or a timeout, return false
 */
bool expectOk(uint8_t command) {
	USART0_TX_str_pgm(command);
	static unsigned char lastChars[8];//circular buffer
	uint8_t cbIndex = 0;
	while (true) {
		lastChars[cbIndex%8] = USART0_Receive();
		if (rxTimedOut) return false;
		if (lastChars[(cbIndex+7)%8]=='\r' && lastChars[(cbIndex+0)%8]=='\n' && lastChars[(cbIndex+5)%8]=='O') {
			//test if last 4 characters are "OK\r\n" and if so return true
			if (
					lastChars[(cbIndex+6)%8]=='K'
					) {
				return true;
			}
			//test if last 7 characters are "ERROR\r\n" and if so return false
			if (
					lastChars[(cbIndex+2)%8]=='E' &&
					lastChars[(cbIndex+3)%8]=='R' &&
					lastChars[(cbIndex+4)%8]=='R' &&
					lastChars[(cbIndex+6)%8]=='R'
					) {
				return false;
			}
		}
		cbIndex++;
	}
}