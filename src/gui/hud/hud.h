#ifndef HUD_H
#define HUD_H

#include "character.h"

void draw_hud();
void draw_statusbar();
void draw_enemy_status(Character_t enemy[4]);
void draw_battle_options();

void draw_options_menu();
void draw_main_menu();
short draw_list(const unsigned short x, const unsigned short y, const char* names[], const short length, const short max_names);
#endif