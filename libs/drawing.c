#include "drawing.h"
#include "math.h"
#include "stdio.h"

#include "strings.h"
#include <stdint.h>

void plotLineLow(short x0, short y0, short x1, short y1, Color color){
    short dx = x1 - x0;
    short dy = y1 - y0;
    short yi = 1;
    if (dy < 0){
        yi = -1;
        dy = -dy;
    }
    short D = (2 * dy) - dx;
    short y = y0;

    for(int x = x0; x <= x1; x++) {
        vga_set_pixel(x, y, color);
        if (D > 0){
            y = y + yi;
            D = D + (2 * (dy - dx));
        }else{
            D = D + 2*dy;
        }
    }       
}

void plotLineHigh(short x0, short y0, short x1, short y1, Color color){
    short dx = x1 - x0;
    short dy = y1 - y0;
    short xi = 1;
    if (dx < 0){
        xi = -1;
        dx = -dx;
    }
    short D = (2 * dx) - dy;
    short x = x0;

    for(int y = y0; y <= y1; y++) {
        vga_set_pixel(x, y, color);
        if (D > 0){
            x = x + xi;
            D = D + (2 * (dx - dy));
        }else{
            D = D + 2*dx;
        }
    }
}

void vga_draw_line(short x0, short y0, short x1, short y1, Color color){
    if(abs_int(y1 - y0) < abs_int(x1 - x0)){
        if(x0 > x1){
            plotLineLow(x1, y1, x0, y0, color);
        }else{
            plotLineLow(x0, y0, x1, y1, color);
        }
    }else{
        if(y0 > y1){
            plotLineHigh(x1, y1, x0, y0, color);
        }else{
            plotLineHigh(x0, y0, x1, y1, color);
        }
    }
}

void vga_draw_rect(short x0, short y0, short width, short height, Color color){
    vga_draw_line(x0, y0, x0 + width, y0, color);                   // Upper Horizontal
    vga_draw_line(x0, y0 + height, x0 + width, y0 + height, color); // Lower Horizontal
    vga_draw_line(x0, y0, x0, y0 + height, color);                  // Left Vertical
    vga_draw_line(x0 + width, y0, x0 + width, y0 + height, color);  // Right Vertical
}

// TODO: Optimize this
void vga_draw_rect_fill(short x0, short y0, short width, short height, Color border_color, short border_weight, Color fill_color){
    
    for(short y = y0; y < y0 + height; y++){
        for(short x = x0; x < x0 + width; x++){
            if(x >= x0 + border_weight && x < x0 + width - border_weight && y >= y0 + border_weight && y < y0 + height - border_weight){
                vga_set_pixel(x, y, fill_color);
            } else {
                vga_set_pixel(x, y, border_color);
            }
            
        }
    }
    
}

void vga_draw_img(short x0, short y0, const char* img_ptr){
    
    uint32_t offset;
    int32_t width, height;

    memcpy(&offset, (void*)(img_ptr+10), sizeof(offset));
    memcpy(&width, (void*)(img_ptr+18), sizeof(width));
    memcpy(&height, (void*)(img_ptr+22), sizeof(height));
    const char* img_data_ptr = img_ptr + offset;
    for(uint16_t y = 0; y < height; y++){
        int offset_counter = 0;
        for(uint16_t x = 0; x < width; x++){
            char b = *(img_data_ptr++); 
            char g = *(img_data_ptr++);
            char r = *(img_data_ptr++);

            Color color = get_color(r,g,b);
            vga_set_pixel(x0 + x, y0 + (height-y)-1, color);
            offset_counter += 3;
        }
        // A esse ponto, nem me questione mais
        // Isso resolve um bug quando não é necessário os bytes de padding
        img_data_ptr += (4 - (offset_counter % 4 ? (offset_counter % 4) : 4));
    }
}
