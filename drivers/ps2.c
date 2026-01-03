#include "ps2.h"
#include "stdio.h"

volatile char* KEYBOARD_DATA_ptr = (char*) 0xFF200100;
volatile char* KEYBOARD_VALID_ptr = (char*) 0xFF200101;
volatile short* KEYBOARD_RAVAIL_ptr = (short*) 0xFF200102;
volatile short* KEYBOARD_CONTROL_ptr = (short*) 0xFF200104;

volatile int keyboard_active_keys[8] = {0,0,0,0,0,0,0,0};
volatile int keyboard_special_keys[8] = {0,0,0,0,0,0,0,0};

void (*keyboard_callback)(void) = (void*) 0;

volatile char keyboard_is_pressed(char key){
    int reg = key / 32;
    int offset = key % 32;
    return (keyboard_active_keys[reg] & (1 << offset)) >> offset;
}

char read_key(){
    while(*KEYBOARD_RAVAIL_ptr == 0){
        // Apparently, without this NOP the while doesnt check conditions
        // Please keep it here, he's funny
        __asm__ __volatile__ ("nop");   // Do Nothing
    }
    char c = *KEYBOARD_DATA_ptr;
    return c;
}

void keyboard_parse(char key){
    switch(key){
        case KEY_A:
            printf("a");
            break;
        case KEY_B:
            printf("b");
            break;
        case KEY_C:
            printf("c");
            break;
        case KEY_D:
            printf("d");
            break;
        case KEY_E:
            printf("e");
            break;
        case KEY_F:
            printf("f");
            break;
        case KEY_G:
            printf("g");
            break;
        case KEY_H:
            printf("h");
            break;
        case KEY_I:
            printf("i");
            break;
        case KEY_J:
            printf("j");
            break;
        case KEY_K:
            printf("k");
            break;
        case KEY_L:
            printf("l");
            break;
        case KEY_M:
            printf("m");
            break;
        case KEY_N:
            printf("n");
            break;
        case KEY_O:
            printf("o");
            break;
        case KEY_P:
            printf("p");
            break;
        case KEY_Q:
            printf("q");
            break;
        case KEY_R:
            printf("r");
            break;
        case KEY_S:
            printf("s");
            break;
        case KEY_T:
            printf("t");
            break;
        case KEY_U:
            printf("u");
            break;
        case KEY_V:
            printf("v");
            break;
        case KEY_W:
            printf("w");
            break;
        case KEY_X:
            printf("x");
            break;
        case KEY_Y:
            printf("y");
            break;
        case KEY_Z:
            printf("z");
            break;
        case KEY_ENTER:
            printf("\n");
            break;
        case KEY_BACKSPACE:
            printf("\b");
            break;
        case KEY_SPACE:
            printf(" ");
            break;
    }
}

void keyboard_ISR(){
    while(*KEYBOARD_RAVAIL_ptr > 0){
        char c = read_key();
        if(c == 0xE0){                                          // Special Key
            c = read_key();                                     // Read new key
            if(c == 0xF0){                                      // Break key
                c = read_key();                                 // Read new key
                int reg = c / 32;
                int offset = c % 32;
                keyboard_special_keys[reg] &= ~(1 << offset);
            } else {        
                int reg = c / 32;
                int offset = c % 32;
                keyboard_special_keys[reg] |= (1 << offset);
            }
        } else {
            if(c == 0xF0){                                      // Break key
                c = read_key();                                 // Read new key
                int reg = c / 32;
                int offset = c % 32;
                keyboard_active_keys[reg] &= ~(1 << offset);
            } else {                                            // Make key
                int reg = c / 32;
                int offset = c % 32;
                keyboard_active_keys[reg] |= (1 << offset);
            }
        }
        
    }
    if(*keyboard_callback != 0){    // Check if pointer points to any address
        keyboard_callback();
    }    
}

void keyboard_begin(){
    config_interrupt(79, keyboard_ISR);
    *KEYBOARD_CONTROL_ptr = 1; // Enable interrupts
}

void keyboard_set_callback(void* function){
    keyboard_callback = function;
}
