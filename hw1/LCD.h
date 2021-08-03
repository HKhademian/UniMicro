#pragma once

#include <avr/io.h>
#include <util/delay.h>

static void LCD_cmd(unsigned char cmd);
static void LCD_init(void);
static void LCD_write(char ch);
static void LCD_writestr(const char* text);

#define LCD_CMD_CLEAR       0x01
#define LCD_CMD_BLINKING    0x0F
#define LCD_CMD_MOV_RIGHT   0x14

#ifndef LCD_DATA
    #define LCD_DATA_DDR    DDRC
    #define LCD_DATA        PORTC
#endif
#ifndef LCD_CTRL
    #define LCD_CTRL_DDR    DDRD
    #define LCD_CTRL        PORTD
    // #define en LCD_CTRL.2                        
    // #define rw LCD_CTRL.1                     
    // #define rs LCD_CTRL.0
#endif

static void LCD_init(void) {
    LCD_DATA_DDR=0xFF;
    LCD_CTRL_DDR=0xFF;
    
    LCD_cmd(0x38);         // 8-bit mode                   
    _delay_ms(1);
    LCD_cmd(0x01);         // clear the screen                     
    _delay_ms(1);
    LCD_cmd(0x0E);         // turn on the cursor                
    _delay_ms(1);
    LCD_cmd(0x80);         // move cursor to the first place of the first row   
    _delay_ms(1);
    return;
}

static void LCD_cmd(unsigned char cmd) {
    LCD_DATA=cmd;
    LCD_CTRL=0x04;              // Register Select = 0, Read/Write = 0, Enable = 1
    _delay_ms(1);
    LCD_CTRL=0x00;              // Enable = 0
    _delay_ms(50);
    return;
}

static void LCD_write(char ch) {
    LCD_DATA= ch;
    LCD_CTRL=0x05;               // Register Select = 1, Read/Write = 0, Enable = 1
    _delay_ms(1);
    LCD_CTRL=0x01;				 // Enable = 0
    _delay_ms(50);
    return ;
}

static void LCD_writestr(const char* text) {
    for(const char *ch=text; *ch; ch++) {
        LCD_write(*ch);
    }
}
