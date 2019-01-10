#define F_CPU 2000000									//Defining the clock frequency to be 2 MHz
#include <avr/io.h>										// Necessary for AVR I/P / O/P
#include <util/delay.h>									// ***** Can be removed. Necessary for delays during debugging.
#include <stdio.h>										// Including stdio.h for ome functional requirements
#include <avr/interrupt.h>								// ***** Can be commented out. not being used.
#include <math.h>										// Will be used for actual calculations.

void transmit(uint8_t dat)
{
	// This is the transmit function using the USART functionality of ATXmega128A3U.
	// Here, we transmit the data 'dat', which has to be 8 bits of data, or 1 byte.
	while(!(USARTC0_STATUS & (USART_DREIF_bm)));		// Until data register empty interrupt flag is set, wait. (So that wrong data is not sent.)
	USARTC0_DATA = dat;									// Load the data into the transmit buffer register. (Sends the data)
}

uint8_t receive(void)
{
	// This is the receive function using the USART functionality of ATXmega128A3U.
	// Here, we receive data in packets of 8 bits, or a byte.
	while(!(USARTC0_STATUS & (USART_RXCIF_bm)));		// Until receive complete interrupt flag is set, wait
	return USARTC0_DATA;								// Access the receive buffer register and return the contents of the buffer register.
}

int main(void)
{
	// This is the main function of the program.
	PORTC_DIRSET = PIN3_bm;								// Setting TX pin as output (C3)
	PORTC_DIRCLR = PIN2_bm;								// Setting RX pin as input (C2)
	USARTC0_BAUDCTRLA = 12;								// Setting the BSEL number in the baud rate register for rate of 9600
	USARTC0_BAUDCTRLB = 0;								// Setting the BSCALE = 0
	USARTC0_CTRLB = (USART_RXEN_bm)|(USART_TXEN_bm);	// Enable transmitter and receiver ports.
	USARTC0_CTRLC = (USART_CHSIZE_8BIT_gc)|(USART_PMODE_DISABLED_gc);  // 8 bits with 1 stop bit.
	
	// pi = 3.1415926 // These are the various values of 32 bit HEX numbers, which were tested using ATXmega.
	// 0x3f9d70a4
	// 0x12345678
	
	// The actual data is to be received from Python via a start flag (From Python) followed by 4 bytes of data, which will be stored in the variables a, b, c, d.
	// That code will look something like this:
	while(1){
		uint8_t head1 = receive();
		if (head1 == 'a'){
			//transmit('b');
			break;
		}
	}
	
	uint8_t a = receive();					// These are the received data from the Python Console, stored in a, b, c and d.
	uint8_t b = receive();
	uint8_t c = receive();
	uint8_t d = receive();
	
	/*
	int a = 0x3f;						// These are superfluous as these have already been defined from the received data from Python.
	int b = 0x9d;						// These were for basic testing and debugging.
	int c = 0x70;
	int d = 0xa4;
	*/
	
	// I am not sure, but I think, we can replace the array with a, b, c and d.
	// To be checked.
	// I also needs to be reomved from the entire code as is unnecessary. At the time of writing, we felt it might be needed at a later point in time.
	
	int tab_reg[64];					// *****
	union IntFloat { long int i;  float f;  };		// Declare combined union for HEX to FLOAT conversion
	union IntFloat val;					// This is an instance of the union created in the above line.
	int i;							// *****
	char buff[10];						// Declare the 1st buffer for string.
	char buff2[10];						// Declaring the 2nd buffer for string, to be used later.

	i=0;							// *****
	
	tab_reg[i]=a;						// *****
	tab_reg[i+1]=b;						// *****
	tab_reg[i+2]=c;						// *****
	tab_reg[i+3]=d;						// *****
	
	/*
	PORTF.DIR = 0xFF;					// For debugging, to check whether data accepted correctly.
	PORTF.OUT = 0xFF;
	_delay_ms(5000);
	PORTF.OUT = tab_reg[i];
	_delay_ms(5000);
	PORTF.OUT = tab_reg[i+1];
	_delay_ms(5000);
	PORTF.OUT = tab_reg[i+2];
	_delay_ms(5000);
	PORTF.OUT = tab_reg[i+3];
	_delay_ms(5000);
	PORTF.OUT = 0xFF;
	*/
	
	// The following function converts the 4 packets of data into a single string.
	
	sprintf(buff,"0X%X%X%X%X", tab_reg[i], tab_reg[i+1], tab_reg[i+2], tab_reg[i+3]); 
	
	// This is the function we were trying to use earlier.
	// We commented it out as it was malfunctioning and we could not figurre out the precise reason other than that there is some kind of data type size mismatch.
	// val.f = atof(buff);
	
	long int num;						// This is a 32-bit integer, num, which accepts the numerical value from the string buff.
	sscanf(buff, "%lx", &num); 				// This line scans the string buff, in the format "%lx" and stores the value of the long integer ('l')
								// in HEX form ('x') in the variable num, passed by reference.
	val.f = *((float*)&num);				// Here the magic takes place and the integer gets converted to float.
								// We store the value of the num, passed by reference, so that we can use the same data with a different data type.
								// Here, the different datat type is float.
								// So, the (float*)&num retuns the address of the floating point number in num.
								// Thus, *((float*)&num) returns the value in the address of the floating point number in num.
	// val.f = 58.328659;					// This was for testing the last part of the code.
								// Working as of now. Verified.
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~The function in val.f comes here.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	val.f = val.f * val.f * val.f;
	//val.f = val.f*val.f*val.f;
	
	// The following function takes the 32-bit integer value from the union val (in the HEX form), sparates it into 4 packets of 8 bits each and the sends them one by one.
	sprintf(buff2,"%lx", val.i);				// This writes the val.i into the 2nd buffer, buff2.
	char *hexstring = buff2;				// This creates a pointer to the start of the character array.
	int bytearray[8];					// This is the array of the bytes to be sent.
	int str_len = 8;					// This is the string lenth (length of buff2). Here, it is 8.
	for (int i=0; i < (str_len / 2); i++) {			// i < (str_len / 2) as we send 2 characters of HEX together.
		sscanf(hexstring + 2*i, "%02x", &bytearray[i]);	// %02x means print at least 2 digits, prepend it with 0 's if there's less.
		// The above line essentially takes the character array buff2 ( from the pointer hexstring ), and writes the first 2 character after 2*i from start to be written to 
		// the bytearray[i], by reference.
		transmit(uint8_t(bytearray[i]));		// Transmit the bytearray[i]
		//transmit(uint8_t (int(0xAB)));		// For debugging and testing
	}
}
