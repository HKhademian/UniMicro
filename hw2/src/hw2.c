/*******************************************************
Project : HW2
Date    : 6/19/2021
Author  : Hossain Khademian
Company : 9830339
Comments: 

Chip type               : ATmega32
AVR Core Clock frequency: 8.000000 MHz
*******************************************************/

#include <mega32.h>
#include <alcd.h>
#include <delay.h>

void init(void);
void loop(void);

unsigned short tickTime = 1000; /*ms*/
char countDown = 0;

void main(void) {
    init();
    while (1) loop();
}

void init(void) {
DDRA=0x00;
PORTA=0xFF;

DDRD=0x00;
PORTD=0xFF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 1 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x83;
OCR0=0x00;
// Timer(s)/Counter(s) Interrupt(s) initialization
// TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// External Interrupt(s) initialization
// INT0 Mode: Rising Edge
// INT1 Mode: Falling Edge
// INT2: Off
GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (0<<INTF2);

// Alphanumeric LCD initialization on PORTC
// Characters/line: 16
lcd_init(16);
lcd_clear(); 
lcd_putsf("ready"); 

delay_ms(100);

// Global enable interrupts
#asm("sei")
}
    
void loop(void) {  
    delay_ms(1000); 
}

void writeI(char i) {
	const char NUMS[] = "0123456789ABCDEF";
    //char u = (i>>4)&0xF;
    //char d = i&0xF;  
    char u = i/10;
    char d = i%10;  
    lcd_putchar(NUMS[u]);
    lcd_putchar(NUMS[d]);
}

void onBombTick(void) {
    if(--countDown <= 0) {  
        lcd_clear(); 
        lcd_putsf("***Boooom!***"); 
        // stop timer0  
        TIMSK &= ~(1<<TOIE0);
    } else {
        lcd_gotoxy(0,0);
        lcd_putsf("time ");   
        writeI(countDown);   
    }  
}

// Timer 0 overflow <every 1ms>
interrupt [TIM0_OVF]
void onTimer(void) {
    static short ms = 0;
    TCNT0=0x83; // limit to 1ms
    if(++ms>=tickTime) {
        ms=0;
        onBombTick();
    }
}

// External Interrupt 0
interrupt [EXT_INT0] 
void onButtonPressed(void) {  
    // stop timer0  
    TIMSK &= ~(1<<TOIE0);
      
    lcd_clear(); 
    lcd_putsf("restart bomb"); 
    delay_ms(250);
	lcd_clear(); 

    tickTime = 1000; // each tick 1sec 
    countDown = 60;  // 60 sec to explode
	  
    // start timer0 
    TIMSK |= (1<<TOIE0);
}

// External Interrupt 1
interrupt [EXT_INT1] 
void onFuse(void) {
	bit a0 = (PINA & (1<<PINA0));
	bit a1 = (PINA & (1<<PINA1));
	bit a2 = (PINA & (1<<PINA2));
	bit a3 = (PINA & (1<<PINA3)); 
	
	bit pattern = !(a3&&(!a2&&(a1||a0)));
	
	if(! TIMSK&(1<<TOIE0)){
//	lcd_clear(); 
//	lcd_putchar(a3?'1':'0'); 
//	lcd_putchar(a2?'1':'0'); 
//	lcd_putchar(a1?'1':'0'); 
//	lcd_putchar(a0?'1':'0'); 
//	lcd_putchar('='); 
//	lcd_putchar(pattern?'1':'0'); 
//	delay_ms(500);

		return; // not in count down
	}  

	lcd_clear(); 

    if(pattern) {
		// on correct inputs  
		
		// stop timer0  
		TIMSK &= ~(1<<TOIE0);   
    	countDown = 0;  
		 
		lcd_putsf("bomb defused");
	} else {
		// on wrong inputs
		tickTime>>=1; // half tick time
		 
		lcd_putsf("wrong");
	}        
	
	delay_ms(500);
}