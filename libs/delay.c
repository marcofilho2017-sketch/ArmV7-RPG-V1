#include "delay.h"
#include "timer.h"

unsigned long millis_time = 0;

void delay_callback(){
    millis_time++;
}

unsigned long millis(){
    return millis_time;
}

void delay_begin(){
    millis_time = 0;
    timer_disable_interrupts(TIMER0);
    timer_set_interval(TIMER0, 100000, delay_callback);
    timer_set_continous(TIMER0);
    timer_enable_interrupts(TIMER0);
}

// Forces whole code to wait 
void delay(unsigned long wait_time){
    unsigned long start_time = millis();
    while(millis() < start_time + wait_time){
        // Apparently, without this NOP the while doesnt check conditions
        // Please keep it here, he's funny
        __asm__ __volatile__ ("nop");   // Do Nothing
    }
}
