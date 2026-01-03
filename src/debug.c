#include "debug.h"

#include "stdio.h"
#include "delay.h"
#include "image.h"
#include "console.h"
#include "drawing.h"
#include "strings.h"
#include "dialogs.h"

char dialog_flag = 0;

void print_time(){
    printf("actual time: %l\n", millis());
}

void print_sum(char param[8][16]){
    int a = parse_int(param[0], 32);
    int b = parse_int(param[1], 32);
    printf("%d + %d = %d\n", a, b, a+b);
}

void fill(char param[8][16]){
    int r = parse_int(param[0], 32);;
    int g = parse_int(param[1], 32);;
    int b = parse_int(param[2], 32);;
    
    vga_fill(get_color(r,g,b));
}

void draw_rect(char param[8][16]){
    int x0 = parse_int(param[0], 32);
    int y0 = parse_int(param[1], 32);
    int width = parse_int(param[2], 32);
    int height = parse_int(param[3], 32);

    int r = parse_int(param[4], 32);;
    int g = parse_int(param[5], 32);;
    int b = parse_int(param[6], 32);;

    vga_draw_rect(x0, y0, width, height, get_color(r,g,b));
}

void draw_line(char param[8][16]){
    int x0 = parse_int(param[0], 32);
    int y0 = parse_int(param[1], 32);
    int x1 = parse_int(param[2], 32);
    int y1 = parse_int(param[3], 32);

    int r = parse_int(param[4], 32);;
    int g = parse_int(param[5], 32);;
    int b = parse_int(param[6], 32);;

    vga_draw_line(x0, y0, x1, y1, get_color(r,g,b));
}

void img_test(char param[8][16]){
    int x0 = parse_int(param[0], 32);
    int y0 = parse_int(param[1], 32);
    int img_id = parse_int(param[2], 32);
    if(img_id >= IMAGE__COUNT){
        printf("ID de imagem invalido - valor maximo: %d \n", IMAGE__COUNT);
    } else {
        vga_draw_img(x0, y0, (char*) images[img_id].start);
    }
    
}

void write_text(char param[8][16]){
    int x = parse_int(param[0], 32);
    int y = parse_int(param[1], 32);
    char_printf(x, y, "%s", param[2]);
}

// TODO: eventually, add string parameters to compare to
void run_dialog(char param[8][16]){
    dialog_flag = parse_int(param[0], 32);
}



void debug_check_flags(){
    if(dialog_flag){
        start_dialog(&dialogs[dialog_flag]);
    }
}

void debug_setup(){
    console_add_command("get_time", 0, print_time);
    console_add_command("sum", 2, print_sum);
    console_add_command("fill", 3, fill);
    console_add_command("rect", 7, draw_rect);
    console_add_command("line", 7, draw_line);
    console_add_command("img", 3, img_test);
    console_add_command("text", 3, write_text);
    console_add_command("dialog", 1, write_text);
}