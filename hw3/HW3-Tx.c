/*******************************************************
Project : HW3 sender side
Author  : Hossain Khademian - 9830339
Chip type: ATmega32
frequency: 8.000000 MHz
*******************************************************/

#include <mega32.h>
#include <delay.h>

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (1<<ADLAR))

// Read the 8 most significant bits
// of the AD conversion result
unsigned char read_adc(unsigned char adc_input) {
	ADMUX=adc_input | ADC_VREF_TYPE;
	// Delay needed for the stabilization of the ADC input voltage
	delay_us(10);
	// Start the AD conversion
	ADCSRA|=(1<<ADSC);
	// Wait for the AD conversion to complete
	while ((ADCSRA & (1<<ADIF))==0);
	ADCSRA|=(1<<ADIF);
	return ADCH;
}

void writeChar(char data) {
	while(UDRE == 0){} // wait prev usart complete
	UDR = data; // write data to TX UDR
	// while(UDRE == 0){} // wait usart complete
}

void mainLoop() {
	char temp = read_adc(0);
	PORTC = temp;
	writeChar(temp); 
	
	/*writeChar((temp/100) + '0');
	delay_ms(1);
	writeChar((temp%100)/10 + '0');   
	delay_ms(1);
	writeChar((temp%10) + '0');   
	delay_ms(1);
	writeChar(' ');*/ 
	
	delay_ms(100);
}

void main(void) {
	DDRA=PORTA=0;
	DDRB=PORTB=0;
	DDRC=0xFF; PORTC=0;
	DDRD=PORTD=0;

	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: Off
	// USART Transmitter: On
	// USART Mode: Asynchronous
	// USART Baud Rate: 9600
	UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
	UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
	UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
	UBRRH=0x00;
	UBRRL=0x33;

	// ADC initialization
	// ADC Clock frequency: 125.000 kHz
	// ADC Voltage Reference: AREF pin
	// ADC Auto Trigger Source: ADC Stopped
	// Only the 8 most significant bits of
	// the AD conversion result are used
	ADMUX=ADC_VREF_TYPE;
	ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
	SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

	while (1) mainLoop();
}
