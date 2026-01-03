#include "timer.h"
#include "interrupts.h"

volatile short* TIMER0_STATUS_ptr = (short*) 0xFF202000;
volatile short* TIMER0_CONTROL_ptr = (short*) 0xFF202004;
volatile short* TIMER0_COUNTER_START_LOW_ptr = (short*) 0xFF202008;
volatile short* TIMER0_COUNTER_START_HIGH_ptr = (short*) 0xFF20200C;
volatile short* TIMER0_COUNTER_LOW_ptr = (short*) 0xFF202010;
volatile short* TIMER0_COUNTER_HIGH_ptr = (short*) 0xFF2020014;

volatile short* TIMER1_STATUS_ptr = (short*) 0xFF202020;
volatile short* TIMER1_CONTROL_ptr = (short*) 0xFF202024;
volatile short* TIMER1_COUNTER_START_LOW_ptr = (short*) 0xFF202028;
volatile short* TIMER1_COUNTER_START_HIGH_ptr = (short*) 0xFF20202C;
volatile short* TIMER1_COUNTER_LOW_ptr = (short*) 0xFF202030;
volatile short* TIMER1_COUNTER_HIGH_ptr = (short*) 0xFF2020034;

enum Timer_Control_bits{
    ITO,
    CONT,
    START,
    STOP
};

enum Timer_Status_bits{
    TO,
    RUN
};

void (*timer0_callback)();
void (*timer1_callback)();

void timer_enable_interrupts(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) |= (1<<ITO);
}   

void timer_disable_interrupts(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) &= !(1<<ITO); 
}

void timer_start(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) |= (1<<START);
}

void timer_stop(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) &= !(1<<START);
}

void timer_set_continous(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) |= (1<<CONT);
}

void timer_set_oneshot(Timer timer){
    *(TIMER0_CONTROL_ptr + (0x10 * timer)) &= !(1<<CONT);
}

void timer0_ISR(){
    *TIMER0_STATUS_ptr = 0;
    timer0_callback();
}

void timer1_ISR(){
    *TIMER1_STATUS_ptr = 0;
    timer1_callback();
}

void timer_set_interval(Timer timer, unsigned int interval, void(*callback)() ){
    switch (timer){
    case TIMER0:
        config_interrupt(72, timer0_ISR);
        timer0_callback = callback;
        break;
    case TIMER1:
        config_interrupt(74, timer1_ISR);
        timer1_callback = callback;
        break;
    default:
        break;
    }
    
    timer_stop(timer);                              // Stop timer
    *(volatile short*)(TIMER0_CONTROL_ptr + (0x10 * timer)) = interval & 0xFFFF;         // Write low byte
    *(volatile short*)(TIMER0_CONTROL_ptr + (0x10 * timer) + 4) = interval >> 16;            // Write high byte
    timer_start(timer);                             // Start it again
}