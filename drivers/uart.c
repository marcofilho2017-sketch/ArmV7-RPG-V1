#include "uart.h"

#include <stdarg.h>
#include "strings.h"
#include "interrupts.h"

volatile char* UART_DATA_ptr = (char*) 0xFF201000;
volatile short* UART_AVAIL_ptr = (short*) 0xFF201002;
volatile short* UART_CONTROL_ptr = (short*) 0xFF201004;
volatile short* UART_AVAIL_WRITE_ptr = (short *) 0xFF201006;

char uart_echo = 1; // Echo Enabled by default

void uart_nothing(char c){}

void (*uart_callback)(char) = uart_nothing;

void uart_set_callback(void* callback){
    uart_callback = callback;
}

void put(char s){
    while(*UART_AVAIL_WRITE_ptr == 0){} // Wait till data is transmitted (This can slow down the main program);
    if(s == 0x7F){
        *UART_DATA_ptr = '\b';
        *UART_DATA_ptr = ' ';
        *UART_DATA_ptr = '\b';
    } else {
        *UART_DATA_ptr = s;
    }
    
    
}

void puts(char* s){
    while(*s != '\0'){
        *UART_DATA_ptr = *s;
    }
}


// TODO: add support to all kinds of values
void printf(char* text, ...){
    va_list ap;
    va_start(ap, text);
    print_formatted(text, &put, &ap);
    va_end(ap);
}

void write(char c){
    *UART_DATA_ptr = c;
}

char read_char(){
    char avail = *UART_AVAIL_ptr;
    char c = 0;

    if(avail > 0){
        c = *UART_DATA_ptr;
    }

    return c;
}

void uart_enable_echo(){
    uart_echo = 1;
}

void uart_disable_echo(){
    uart_echo = 0;
}

void uart_ISR(){
    char c = read_char();       // Read input char
    if(uart_echo){
        put(c);       // If echo enabled, do the thing
    }
    uart_callback(c);
}

void uart_begin(){
    *UART_CONTROL_ptr = 1;      // Enable Read Interrupts
    config_interrupt(80, uart_ISR);
}