#ifndef ITEM_H
#define ITEM_H

#include "quest.h"
#include "spell.h"

typedef enum {
    ITEM_TYPE_NONE,          // Indicates a placeholder for nothing
    ITEM_TYPE_WEAPON,        // If used, it will be equipped as weapon
    ITEM_TYPE_ARMOR,         // If used, it will be equipped as armor
    ITEM_TYPE_CONSUMABLE,    // If used, it will be consumed and applied a effect, if theres any
    ITEM_TYPE_KEY,           // Its a key item for a speficic quest or use (Cannot be sold)
} ItemType_t;


typedef union {
    Effect_t effect;
    Quest_t* quest;
} ItemData_t;

typedef struct {
    const ItemType_t type;
    const ItemData_t data;            
    const char* name;
    const char* description;
    const short value;
} Item_t;

#endif