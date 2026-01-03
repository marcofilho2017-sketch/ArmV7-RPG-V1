#include "textbox.h"
#include "vga.h"
#include "image.h"
#include "drawing.h"

#define TEXTBOX_X 20
#define TEXTBOX_Y 160

#define TEXTBOX_TEXT_X 8
#define TEXTBOX_TEXT_Y 43
#define TEXTBOX_TEXT_WIDTH 64

// General textbox notes:
// Actual max width is 64 characters
// Actual max height is 9 lines
// Actual max total characters is 64*9 = 576 characters

void textbox_print_limited(int x, int y, int max_width, const char* text){
    char_set_cursor(x, y);
    while(*text != '\0'){
        if(*text == '\n'){// || char_get_cursor().x > (x + max_width)){
            y++;
            char_set_cursor(x, y);
        } else{
            char_put(*text);
        }
        text++;
    }
}

void draw_textbox(const char* character, const char* text){
    clear_textbox();
    //vga_draw_img(TEXTBOX_X, TEXTBOX_Y, images[IMAGE_textbox].start);
    vga_draw_rect_fill(TEXTBOX_X, TEXTBOX_Y, 280, 60, get_color(255,255,255), 3, get_color(0,0,0));
    //vga_draw_line(TEXTBOX_X, TEXTBOX_Y + 9, TEXTBOX_X + 277, TEXTBOX_Y + 9, get_color(255,255,255));
    vga_draw_line(TEXTBOX_X, TEXTBOX_Y + 10, TEXTBOX_X + 277, TEXTBOX_Y + 10, get_color(255,255,255));
    textbox_print_limited(TEXTBOX_TEXT_X+4, TEXTBOX_TEXT_Y-2, TEXTBOX_TEXT_WIDTH, character);
    textbox_print_limited(TEXTBOX_TEXT_X, TEXTBOX_TEXT_Y, TEXTBOX_TEXT_WIDTH, text);
    if(vga_is_double_buffered()){
        vga_switch_buffer();
    }
}

void clear_textbox(){
    for(int y = 0; y < 15; y++){
        char_set_cursor(TEXTBOX_TEXT_X, TEXTBOX_TEXT_Y - 2 + y);
        for(int x = 0; x < TEXTBOX_TEXT_WIDTH+3; x++){
            char_put(' ');
        }
    }
}