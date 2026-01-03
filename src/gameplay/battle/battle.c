#include "battle.h"
#include "vga.h"
#include "drawing.h"
#include "image.h"
#include "ps2.h"
#include "keybinds.h"

void start_battle(unsigned int enemies[4]){
    vga_draw_img(0, 30, images[IMAGE_maxwell].start);
    vga_switch_buffer();
    while(!keyboard_is_pressed(RETURN_BUTTON)){;}
}