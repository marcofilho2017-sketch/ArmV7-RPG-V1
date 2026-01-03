#ifndef SPELL_H
#define SPELL_H

typedef struct {
    const char attack;              // Attack status given or lost because of this spell
    const char defense;             // Defense status givens or lost because of this spell
    const unsigned char duration;    // How many turns this spell is active on
    const char life;                // How much life this spell gives or takes each turn
} Effect_t;

typedef struct {
	Effect_t effect;            // Effect given to the character
	unsigned short cost;        // Mana cost to use the spell
    const char isFriendly;      // Can this spell be used on the caster?
	const char* name;           // Spell name
    const char* description;    // Spell description
} Spell_t;

#endif
