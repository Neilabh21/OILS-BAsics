// This code as of now is incomplete and untested. Testing can only be done once new compiler cum programmer for ATXmega is acquired.

#define F_CPU 2000000                                    //Defining the clock frequency to be 2 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

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
	
	// pi = 3.1415926
	// 0x400921FB54442D18
	
	
	int a = 0x40;                             //declare input value recieved from serial connection
	int b = 0x09;
	int c = 0x21;
	int d = 0xFB;
	int e = 0x54;
	int f = 0x44;
	int g = 0x2D;
	int h = 0x18;
	
	int tab_reg[8];                  
	union IntFloat { unsigned long long int i;  double doub;  }; //Declare combined datatype for HEX to FLOAT conversion
	union IntFloat val;
	char buff[20];                          //Declare buffer for string
	char buff2[20];

	transmit(0x00);
	transmit(0x00);
	transmit(0x00);
	transmit(0x00);
	
	transmit(sizeof(short int));
	transmit(0x00);
	transmit(sizeof(int));
	transmit(0x00);
	transmit(sizeof(long int));
	transmit(0x00);
	transmit(sizeof(long long int));
	transmit(0x00);
	transmit(sizeof(char));
	transmit(0x00);
	transmit(sizeof(float));
	transmit(0x00);
	transmit(sizeof(double));
	transmit(0x00);
	transmit(sizeof(long double));
	
	
	transmit(0x00);
	transmit(0x00);
	transmit(0x00);
	transmit(0x00);
	
	tab_reg[0]=a;                    //to test the code without a data stream,
	tab_reg[1]=b;                 //you may uncomment these two lines.
	tab_reg[2]=c;
	tab_reg[3]=d;
	tab_reg[4]=e;
	tab_reg[5]=f;
	tab_reg[6]=g;
	tab_reg[7]=h;
	
	/*
	PORTF.DIR = 0xFF;
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

	sprintf(buff,"0X%X%X%X%X%X%X%X%X", tab_reg[0], tab_reg[1], tab_reg[2], tab_reg[3], tab_reg[4], tab_reg[5], tab_reg[6], tab_reg[7]); 
	
	
	unsigned long long int num;
	sscanf(buff, "%llx", &num);  // assuming you checked input
	//val.doub = *((double*)&num);
	
	val.doub = 3.141592653589793;
	// The function in val.f comes here.
	//val.f = val.f;
	
	sprintf(buff2,"%llx", val.i);
	char *hexstring = buff2;
	int bytearray[16];
	int str_len = 16;
	for (int i=0; i < (str_len / 2); i++) {
		sscanf(hexstring + 2*i, "%02x", &bytearray[i]);
		transmit(uint8_t(bytearray[i]));
		//transmit(uint8_t (int(0xAB)));
	}
}
