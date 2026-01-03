// Drivers to initialize
#include "stdio.h"
#include "delay.h"
#include "console.h"
#include "drawing.h"
#include "ps2.h"

// Config debug commands
#include "debug.h"

// Engine includes
#include "main_menu.h"
#include "dialog_handler.h"
#include "dialogs.h"
#include "vga.h"
#include "image.h"
#include "hud.h"
#include "exceptions.h"
#include "scene_handler.h"

short buf_a[VGA_BUFFER_SIZE];
short buf_b[VGA_BUFFER_SIZE];

void main() {
    // Initialize drivers
    console_begin();
    delay_begin();
    keyboard_begin();
    debug_setup();
    enable_irq();

    // Set up buffer
    vga_set_backbuffer(buf_a);
    // Empty screen
    vga_fill(get_color(0,0,0));
    char_clear();
    vga_switch_buffer();

    vga_set_backbuffer(buf_b);
    // Indicate everything is ok!
    printf("ZephOS V1.0\nAll Rights Reserved - ZephCO\n");
    
    // Run game
    main_menu();
   
   
    while(1){
        debug_check_flags();            // Try to check debug stuff
        update_scene();
        
    }
    
}