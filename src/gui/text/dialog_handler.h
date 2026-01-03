#ifndef DIALOG_HANDLER_H
#define DIALOG_HANDLER_H

typedef struct{
    const char* character;
    const char* text;
} Text_t;

typedef struct{
    Text_t* text;
    unsigned short size;
} Dialog_t;

void start_dialog(const Dialog_t* dialog);

#endif