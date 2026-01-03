#ifndef DRAWING_H
#define DRAWING_H

#include "vga.h"

void vga_draw_line(short x0, short y0, short x1, short y1, Color color);
void vga_draw_rect(short x0, short y0, short width, short height, Color color);
void vga_draw_rect_fill(short x0, short y0, short width, short height, Color border_color, short border_weight, Color fill_color);
void vga_draw_img(short x0, short y0, const char* img_ptr);

#endif