#pragma once

#include <avr/io.h>
#include <util/delay.h>

#ifndef KEYPAD_PORT
    #define KEYPAD_PORT PORTB
    #define KEYPAD_PIN  PINB
    #define KEYPAD_DDR  DDRB
#endif

static void KeyPad_init() {
    KEYPAD_DDR  = 0x0F; // [0 1 2 3] for select row (output)  [4 5 6 7] for reading column (input)
    KEYPAD_PORT = 0xFF; // [0:4] row is deactive     [5:7] is pullup
}

static unsigned char KeyPad_read(unsigned char wait) {
    do{
        for(unsigned char row = 0; row<4; row++) {
            // KEYPAD_PORT = 0xFF ^ (1<<row);
            KEYPAD_PORT = ~(1<<row); // make row active (write 0)
            _delay_us(1);

            unsigned char cols = ((~KEYPAD_PIN)>>4)&0x0F;
            
            for(unsigned char col=0; cols; col++,cols>>=1) {
               
                if(cols&0x01){
                    // wait till key release
                    while((KEYPAD_PIN&0xF0)!=0xF0) _delay_us(10);
                    
                    KEYPAD_PORT = 0xFF; // reset state
                    return row*4+col;
                }

            }
        }
    } while (wait);

    KEYPAD_PORT = 0xFF; // reset state
    return -1; // -1==0xFF=no key pressed (in wait=false mode)
}
