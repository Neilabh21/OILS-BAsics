/*
 * OILS - Euclidean Norm.cpp
 *
 * Created: 15-02-2019 19:15:31
 * Author : Neilabh
 */ 

#include <avr/io.h>


#define F_CPU 2000000									//Defining the clock frequency to be 2 MHz
#include <avr/io.h>										// Necessary for AVR I/P / O/P
#include <util/delay.h>									// ***** Can be removed. Necessary for delays during debugging.
#include <stdio.h>										// Including stdio.h for ome functional requirements
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


	for(int i = 0; i < 13; i++){
		//transmit('a');
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
		uint8_t a = 0x45;						// These are superfluous as these have already been defined from the received data from Python.
		uint8_t b = 0x7e;						// These were for basic testing and debugging.
		uint8_t c = 0xc0;
		uint8_t d = 0x00;
		*/
		
		while(1){
			uint8_t head1 = receive();
			if (head1 == 'a'){
				//transmit('c');
				break;
			}
		}
		
		uint8_t a1 = receive();					// These are the received data from the Python Console, stored in a, b, c and d.
		uint8_t b1 = receive();
		uint8_t c1 = receive();
		uint8_t d1 = receive();
		
		/*
		uint8_t a1 = 0x45;						// These are superfluous as these have already been defined from the received data from Python.
		uint8_t b1 = 0x7e;						// These were for basic testing and debugging.
		uint8_t c1 = 0xc0;
		uint8_t d1 = 0x00;
		*/
		
		//uint16_t address = 0x0000;
		//uint16_t address = (uint16_t)(TEST_PAGE_ADDR_1*EEPROM_PAGESIZE)|(TEST_BYTE_ADDR_a & (EEPROM_PAGESIZE-1));
		
		uint8_t foo = 0;
		uint8_t * address = &foo + 1;
		
		*(uint8_t*)(address+0) = a; // write a
		*(uint8_t*)(address+1) = b; // write b
		*(uint8_t*)(address+2) = c; // write c
		*(uint8_t*)(address+3) = d; // write d
		
		*(uint8_t*)(address+4) = a1; // write a1
		*(uint8_t*)(address+5) = b1; // write b1
		*(uint8_t*)(address+6) = c1; // write c1
		*(uint8_t*)(address+7) = d1; // write d1
		
		float float_input_1 = *(float*)(address); // read float_input
		float float_input_2 = *(float*)(address+4); //read 2nd float input
		
		//float_input_1 = 3;
		//float_input_2 = 4;
		
		// Function Implemented here
		float float_output = sqrt(float_input_1*float_input_1+float_input_2*float_input_2);
		
		//float_output = 5;
		
		*(float*)(address+8) = float_output; // Write float_output
		
		
		uint8_t e = *(uint8_t*)(address+8); // read e
		uint8_t f = *(uint8_t*)(address+9); // read f
		uint8_t g = *(uint8_t*)(address+10); // read h
		uint8_t h = *(uint8_t*)(address+11); // read e
		
		transmit(e);
		transmit(f);
		transmit(g);
		transmit(h);
	}
}

