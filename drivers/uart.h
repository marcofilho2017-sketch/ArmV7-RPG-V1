#ifndef UART_H
#define UART_H

void uart_set_callback(void* callback);
void put(char s);
void puts(char* s);
void printf(char* text, ...);
void write(char c);
void uart_enable_echo();
void uart_disable_echo();
void uart_begin();

#endif