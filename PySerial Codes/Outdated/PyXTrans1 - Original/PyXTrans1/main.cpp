// This is just a copy of the main file. nno longer of any use. The update are all in the folder named PyXTrans

#define F_CPU 2000000                                    //Defining the clock frequency to be 2 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>

volatile uint8_t d;                                          //The number to be fed and processed for calculating the value
volatile bool count;                                      //To check if interrupt has been received

void transmit(uint8_t dat)
{
	while(!(USARTC0_STATUS & (USART_DREIF_bm)));         //Until data register empty interrupt flag is set, wait
	USARTC0_DATA = dat;                                  //Load the data into the transmit buffer register
}
uint8_t receive(void)
{
	while(!(USARTC0_STATUS & (USART_RXCIF_bm)));         //Until receive complete interrupt flag is set, wait
	return USARTC0_DATA;                                 //Access the receive buffer register
}
int main(void)
{
	PORTC_DIRSET=PIN3_bm;                                //Setting TX pin as output
	PORTC_DIRCLR=PIN2_bm;	                             //Setting RX pin as input
	USARTC0_BAUDCTRLA = 12;                              //Setting the BSEL number in the baud rate register for rate of 9600
	USARTC0_BAUDCTRLB = 0; //Setting the BSCALE = 0
	USARTC0_CTRLB = (USART_RXEN_bm)|(USART_TXEN_bm);     //Enable transmitter and receiver port
	USARTC0_CTRLC = (USART_CHSIZE_8BIT_gc)|(USART_PMODE_DISABLED_gc);  //8 bits with 1 stop bit
	int a = 0x42;
	int b = 0x69;
	int c = 0x50;
	int d = 0x8c;
	int tab_reg[64];                  //declare input value recieved from serial connection
	union IntFloat { long int i;  float f;  }; //Declare combined datatype for HEX to FLOAT conversion
	union IntFloat val;
	int i;
	char buff[10];                          //Declare buffer for string
	char buff2[10];

	i=0;
	
	tab_reg[i]=a;                    //to test the code without a data stream,
	tab_reg[i+1]=b;                 //you may uncomment these two lines.
	tab_reg[i+2]=c;
	tab_reg[i+3]=d;

	sprintf(buff,"0X%X%X%X%X", tab_reg[i], tab_reg[i+1], tab_reg[i+2], tab_reg[i+3]);   //I need to swap the words, as the response is with the opposite endianness.
	val.f = atof(buff);                     //Convert string to float :-)
	
	
	
	// The function in val.f comes here.
	//val.f = val.f;
	
	sprintf(buff2,"%lx", val.i);
	char *hexstring = buff2;
	int bytearray[8];
	int str_len = 8;
	for (int i=0; i < (str_len / 2); i++) {
		sscanf(hexstring + 2*i, "%02x", &bytearray[i]);
		transmit(uint8_t(bytearray[i]));
		//transmit(uint8_t (int(0xAB)));
	}
}
