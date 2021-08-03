/*******************************************************
Project : HW3-Rx
Author  : Hossain Khademian - 9830339

Chip type : ATmega32
Clock freq: 8.000000 MHz
*******************************************************/

#include <mega32.h>

#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver interrupt service routine
interrupt [USART_RXC]
void usart_rx_isr(void) {
	char status, data;
	status=UCSRA;
	data=UDR; 
	PORTA = data;
	
	if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0) {
		char temp; // temp = ( data[0:255] /256 [0:640 milivolts] *64 [0:64 deg celcius] ) = data/4 = data>>2
		temp = data >> 2;
		if(temp < 10) {
			PORTB = 0x01;
		} else if(temp > 25) {
			PORTB = 0x02;
		} else {
			PORTB = 0;
		}
	}
}


void main(void) {
	DDRA=0xFF; PORTA=0;
	DDRB=0xFF; PORTB=0;
	DDRC= PORTC= 0;
	DDRD= PORTD= 0;

	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: Off
	// USART Mode: Asynchronous
	// USART Baud Rate: 9600
	UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
	UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
	UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
	UBRRH=0x00;
	UBRRL=0x33;

	// Global enable interrupts
	#asm("sei")

	while (1);
}
