#include "dialog_handler.h"
#include "textbox.h"
#include "ps2.h"
#include "delay.h"
#include "keybinds.h"

void start_dialog(const Dialog_t* dialog){
    int index = 0;
    int keyPressed = 0;
    while(index < dialog->size){
        draw_textbox(dialog->text[index].character, dialog->text[index].text);
        keyPressed = keyboard_is_pressed(CONFIRM_BUTTON);
        while(!keyboard_is_pressed(CONFIRM_BUTTON) || keyPressed){
            keyPressed = keyboard_is_pressed(CONFIRM_BUTTON);
            delay(50);
        }
        index++;
    }
    clear_textbox();
}