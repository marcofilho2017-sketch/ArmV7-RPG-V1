#ifndef STRINGS_H
#define STRINGS_H
#include <stdarg.h>

void print_formatted(char* text, void(*put_function)(char), va_list* ap_ptr);
char strcmp(char* string_a, char* string_b);
void strcpy(char* destination, char* source);
int strlen(char* str);
void memcpy(void* destination, void* source, int num_bytes);
int parse_int(char* str, int max_length);

#endif