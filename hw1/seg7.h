#pragma once

#include <avr/io.h>

#ifndef SEQ7_PORT
    #define SEQ7_PORT   PORTA
    #define SEQ7_PIN    PINA
    #define SEQ7_DDR    DDRA
#endif

static const unsigned char SEQ7_MAP[] = {
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
    0x77, /* A */ 
    0x7C, /* B */ 
    0x39, /* C */ 
    0x5E, /* D */ 
    0x79, /* E */ 
    0x71, /* F */ 
    0b00000000, /* 16=OFF */ 
    0b01110110, /* 17=H */ 
    0b01000000, /* 18=- */ 
};

static void Seg7_init() {
    SEQ7_DDR = 0xFF;
    SEQ7_PORT = 0x00;
}

static void Seg7_write(unsigned char num) {
    SEQ7_PORT = SEQ7_MAP[num];
} 

static void Seg7_writedot(unsigned char dot) {
    if(dot)        SEQ7_PORT |= 0x80;
    else           SEQ7_PORT &= 0x7F;
} 

static void Seg7_toggledot() {
    SEQ7_PORT ^= 0x80;
} 
