#ifndef NPC_H
#define NPC_H

#include "character.h"
#include "dialog_handler.h"

typedef struct {
    const Character_t c;
    const Dialog_t dialog;
    const unsigned char total_dialogs;        
} npc_t;


#endif