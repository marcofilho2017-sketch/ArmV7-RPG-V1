#ifndef CHARACTER_H
#define CHARACTER_H

#include "item.h"
#include "spell.h"

#define MAX_ITENS 32
#define MAX_SPELLS 4
#define MAX_EFFECTS 4

typedef struct {
	float attack_chance;    // Chance of attacking
	float defense_chance;   // Chance of defending
	float spell_chance;     // Chance of using a spell
	float item_chance;      // Chance of using a item (if it has one)
} Personality_t;


typedef struct {
    char* name;                             // Character name
    char* description;                      // A brief character description
    unsigned short max_life;                // Maximum life
    unsigned short max_mana;                // Maximum mana
    unsigned short life;                    // Actual life
    unsigned short mana;                    // Actual mana
    unsigned short xp;                      // Xp that this character has/gives when he dies
    unsigned char level;                    // Level of the character (only revelent to player, otherwhise cosmetic)
    unsigned short money;                   // Money that the character has within it
    unsigned short attack;                  // Base attack
    unsigned short defense;                 // Base defense
    unsigned int weapon;                    // Index of item actively used as weapon
    unsigned int armor;                     // Index of item actively used as armor
    unsigned int itens[MAX_ITENS];          // Index of total itens that can be stored
    Personality_t personality;              // Personality of this character
    Effect_t* effects[MAX_EFFECTS];         // Independent Effects applied to character
    unsigned int spells[MAX_SPELLS];             // Spells usable by the character
    char isBoss;                            // Explicits if the character should be treated as a boss
} Character_t;


#endif