#ifndef VGA_H
#define VGA_H

typedef unsigned short Color;

// maximum is 255, gets scaled accordingly
#define get_color(r, g, b) ((r >> 3) << 11) + ((g>>2) << 5) + (b>>3)
#define VGA_BUFFER_SIZE 1<<17
#define CHAR_BUFFER_SIZE 1<<13

// VGA buffer handling 
void vga_set_backbuffer(short* new_buffer);
void vga_switch_buffer();
void vga_fill(Color color);
char vga_is_double_buffered();


typedef struct{
    unsigned char x;
    unsigned char y;
} CharCursor_t;

// Char Buffer Handling
void char_set_backbuffer(char* new_buffer);
void char_switch_buffer();
void vga_set_pixel(short x, short y, Color color);
void char_set_cursor(unsigned char x, unsigned char y);
CharCursor_t char_get_cursor();
void char_put(char c);
void char_printf(int x, int y, char* text, ...);
void char_clear();
char char_is_double_buffered();


#endif