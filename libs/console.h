#ifndef CONSOLE_H
#define CONSOLE_H

void console_begin();
void console_add_command(char* name, char num_param, void(*function_call)(char[8][16]));
#endif