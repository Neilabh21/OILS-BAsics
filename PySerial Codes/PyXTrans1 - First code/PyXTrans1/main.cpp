#define F_CPU 2000000								//Defining the clock frequency to be 2 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

// volatile uint8_t d;								// ***** The number to be fed and processed for calculating the value
volatile bool count;								// To check if interrupt has been received
// Above variables were defined while thinking we might have to use ISRs, which were ultimately not used.

void transmit(uint8_t dat)
{
	// USART transmit function.
	while(!(USARTC0_STATUS & (USART_DREIF_bm)));				//Until data register empty interrupt flag is set, wait
	USARTC0_DATA = dat;							//Load the data into the transmit buffer register
}
uint8_t receive(void)
{
	// USART recieve function.
	while(!(USARTC0_STATUS & (USART_RXCIF_bm)));      		   	// Until receive complete interrupt flag is set, wait
	return USARTC0_DATA;                              		   	// Access the receive buffer register
}
int main(void)
{
	// Main function in the program.
	PORTC_DIRSET = PIN3_bm;							// Setting TX pin as output - C3
	PORTC_DIRCLR = PIN2_bm;							// Setting RX pin as input - C2
	USARTC0_BAUDCTRLA = 12;							// Setting the BSEL number in the baud rate register for rate of 9600
	USARTC0_BAUDCTRLB = 0;							// Setting the BSCALE = 0
	USARTC0_CTRLB = (USART_RXEN_bm)|(USART_TXEN_bm);			// Enable transmitter and receiver port
	USARTC0_CTRLC = (USART_CHSIZE_8BIT_gc)|(USART_PMODE_DISABLED_gc);	// 8 bits with 1 stop bit
	//sei();								// ***** Global interrupt declared for potential usage.
    PORTF_DIRSET=0xFF;								
	count=false;								// Boolean variable used here.
	while(1)								// Infinite loop, will proceed further only on getting the required inute.
	{
        	PORTF.OUT=0xFF;
		d=receive();
		if(d == 0x61)
		{
        		PORTF.OUT=0x00;
			while(1)
			{
				uint8_t e=receive();
		 		if(e == 0xFF)
		 		{
		  			break;
		 		}
		 		transmit(e);
			}
		 	_delay_ms(1000);
		}
	}
}
// The following code was for debugging and is no longer useful.
     //d=receive();
     //count=true;
     //transmit('d');
	 //_delay_ms(100);
		
		//transmit('c');
		//_delay_ms(100);
		/*if(count == true & )
		{
		 int d3=d*d*d;
		 transmit(d3);
		 count=false;
		}*/
	}
}


// The following is the global interrupt and is no longer useful
/*ISR(USARTC0_RXC_vect)
{
 d=receive();
 count=true;
 transmit(d);
}*/
