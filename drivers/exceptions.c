#include "exceptions.h"
#include "vga.h"

void dump_core(unsigned int* reg){
    vga_fill(get_color(0,0,255));
    char_clear();
    char_printf(25,26, "The Computer Pooped its pants :("); // Read the ICCIAR from the CPU Interface in the GIC);
    char_printf(25,28, "Sorry for that."); // Read the ICCIAR from the CPU Interface in the GIC);
    for(int i = 0; i <= 12; i++){
        char_printf(4, 16 + i*2, "R%d: 0x%p", i, reg[i+1]);
    }
    char_printf(4, 42, "LR: 0x%p", reg[14]-4);
    char_printf(4, 44, "CPSR: 0x%p", reg[0]);
}