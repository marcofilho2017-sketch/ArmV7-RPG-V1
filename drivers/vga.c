#include "vga.h"
#include "stdarg.h"
#include "strings.h"

volatile short** VGA_FRONT_BUFFER_ptr = (volatile short**) 0xFF203020;
volatile short** VGA_BACK_BUFFER_ptr = (volatile short**) 0xFF203024;
volatile short* VGA_RESOLUTION_X_ptr = (short*) 0xFF203028;
volatile short* VGA_RESOLUTION_Y_ptr = (short*) 0xFF20302A;
volatile short* VGA_STATUS_ptr = (short*) 0xFF20302C;

volatile char** CHAR_FRONT_BUFFER_ptr = (volatile char**) 0xFF203030;
volatile char** CHAR_BACK_BUFFER_ptr = (volatile char**) 0xFF203034;
volatile short* CHAR_RESOLUTION_X_ptr = (short*) 0xFF203038;
volatile short* CHAR_RESOLUTION_Y_ptr = (short*) 0xFF20303A;

// VGA buffer handling 

void vga_set_backbuffer(short* new_buffer){
    *VGA_BACK_BUFFER_ptr = new_buffer;
}

void vga_switch_buffer(){
    *VGA_FRONT_BUFFER_ptr = (short*) 1;
    while(*VGA_STATUS_ptr & 0x01){
        ;   // Wait
    }
}

char vga_is_double_buffered(){
    if(*VGA_BACK_BUFFER_ptr == *VGA_FRONT_BUFFER_ptr){
        return 0;
    }
    return 1;
}

// Char Buffer Handling

void char_set_backbuffer(char* new_buffer){
    *CHAR_BACK_BUFFER_ptr = new_buffer;
}

void char_switch_buffer(){
    while(*VGA_STATUS_ptr && 0x1){;}
    *CHAR_FRONT_BUFFER_ptr = (char*) 1;
}

char char_is_double_buffered(){
    if(*CHAR_FRONT_BUFFER_ptr == *CHAR_BACK_BUFFER_ptr){
        return 0;
    }
    return 1;
}

// Variables used for saving where last char/pixel was put in

unsigned short pixel_x = 0;
unsigned short pixel_y = 0;

unsigned char char_x = 0;
unsigned char char_y = 0;

void vga_set_pixel(short x, short y, Color color){
    short* address = (short*) *VGA_BACK_BUFFER_ptr + (y << 9) + (x << 0);
    *address = color;
}

void vga_fill(Color color){
    short width  = *VGA_RESOLUTION_X_ptr;
    short height = *VGA_RESOLUTION_Y_ptr;
    for(short y = 0; y < height; y++){
        short* address = (short*) (*VGA_BACK_BUFFER_ptr + (y << 9));
        for(short x = 0; x < width; x++){
            *address = color;
            address++;
        }
    }
}

void char_set_cursor(unsigned char x, unsigned char y){
    char_x = x;
    char_y = y;
}

CharCursor_t char_get_cursor(){
    CharCursor_t c;
    c.x = char_x;
    c.y = char_y;
}

void char_put(const char c){
    volatile char* char_addr = *CHAR_FRONT_BUFFER_ptr + (char_y << 7) + char_x;
    *char_addr = c;
    char_x++;
}

void char_printf(int x, int y, char* text, ...){
    char_x = x;
    char_y = y;
    
    va_list ap;
    va_start(ap, text);
    print_formatted(text, &char_put, &ap);
    va_end(ap);
}

void char_clear(){
    for(short y = 0; y < 60; y++){
        char_set_cursor(0,y);
        for(short x = 0; x < 80; x++){
            char_put(' ');
        }
    }
}

