#include <avr/io.h>
#include <util/delay.h>
// #include <stdio.h>


// default portC as lcd data bus and portD as lcd ctrl bus
/***** I modified this file *****/
#include "../lib/LCD.h" 

// default portB
#include "../lib/keypad.h" 

// default portA
#include "../lib/seg7.h" 

#define BitSet(___PORT___, ___N___)        ___PORT___ |= (1U << ___N___)
#define BitReset(___PORT___, ___N___)      ___PORT___ &= ~(1U << ___N___)
#define BitToggle(___PORT___, ___N___)     ___PORT___ ^= (1U << ___N___)
#define BitGet(___PIN___, ___N___)         ((___PIN___ >> ___N___) & 1U)

// each key number has a display value
const unsigned char KEY_MAP[] = "789/456X123-C0=+";

unsigned char password[4] = {};
unsigned char counter = 0;

static void setup() {
    Seg7_init(); // init DDRA and PORTA for output

    KeyPad_init(); // init DDRB and PORTB

    LCD_init(); // init DDRC and DDRD and lcd module
    LCD_cmd(LCD_CMD_BLINKING);
    _delay_ms(100);

    LCD_writestr("set Pass: ");
    for(int i=0; i<4; i++) {
        password[i] = KeyPad_read(1);
        _delay_ms(10);
        LCD_write(KEY_MAP[password[i]]);
    }

    int wrong=1;
    while (wrong) {    
        wrong=0;

        LCD_cmd(LCD_CMD_CLEAR);
        LCD_writestr("Enter pass: ");

        for(int i=0; i<4; i++) {
            if(KeyPad_read(1)!=password[i]) {
                wrong=1;
            }
            LCD_write('*');
        }

        if(wrong) {
            LCD_cmd(LCD_CMD_CLEAR);
            LCD_writestr("Wrong pass!");
            _delay_ms(1500);
        }
    }

    LCD_cmd(LCD_CMD_CLEAR);
    LCD_writestr("Welcome");
    _delay_ms(200);

    LCD_write('.');
    _delay_ms(200);

    LCD_write('.');
    _delay_ms(200);

    LCD_write('.');
    
    _delay_ms(500);
    LCD_cmd(LCD_CMD_CLEAR);
    LCD_writestr("Counting");

    PORTD |= (1<<7); // pullup D7
    DDRD &= ~(1<<7); // set D7 as input button
    DDRD |= (1<<6); // set D6 as output LED
    BitSet(PORTD,6); // turn led on
}

static void loop() {
    BitToggle(PORTD,6); // toggle led

    Seg7_write(counter);

    if(++counter>9)
        counter=0;

    _delay_ms(250);

    while (BitGet(PIND,7))   _delay_us(50); // wait button press
    while (!BitGet(PIND,7))   _delay_us(50); // wait button release
}

int main(void) {
    setup();
    while (1) loop();
    return 0;
}
