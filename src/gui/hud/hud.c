
#include "hud.h"
#include "player.h"
#include "drawing.h"
#include "image.h"
#include "math.h"
#include "ps2.h"
#include "delay.h"
#include "stdio.h"
#include "keybinds.h"
#include "itens.h"
#include "characters.h"

void draw_hud(){
    draw_statusbar();
    vga_draw_rect_fill(14, 186, 126, 44, get_color(255, 255, 255), 3, get_color(0,0,0));
    //draw_enemy_status(enemies);
    //draw_battle_options();
    //vga_draw_rect_fill(0,30, 320, 152, get_color(255,255,255), 0, get_color(255,255,255));
}

void draw_statusbar(){
    vga_draw_rect_fill(3, 2, 315, 24, get_color(255,255,255), 3, get_color(0,0,0));
    char_printf(3, 2, player.name);                                     // Nome jogador
    char_printf(3, 4, "Dinheiro: %d", player.money);                    // Dinheiro
    char_printf(23, 2, "Arma: %s", itens[player.weapon].name);                 // Arma
    char_printf(19, 4, "Armadura: %s", itens[player.armor].name);             // Armadura
    char_printf(46, 2, "Vida: |");                                      // Vida
    for(short i = 0; i < 16; i++){
        if(map_int(player.life, 0, player.max_life, 0, 16) > i){
            char_put('#');
        } else {
            char_put(' ');
        }
    }
    char_printf(69, 2, "| %d/%d  ", player.life, player.max_life);
    char_printf(46, 4, "Mana: |");                                      // Mana
    for(short i = 0; i < 16; i++){
        if(map_int(player.mana, 0, player.max_mana, 0, 16) > i){
            char_put('#');
        } else {
            char_put(' ');
        }
    }
    char_printf(69, 4, "| %d/%d  ", player.mana, player.max_mana);
}


// Battle stuff start
void draw_enemy_status(Character_t enemy[4]){
    short x = 128;
    short y = 188;
    vga_draw_rect_fill(x, y, 186, 44, get_color(255,255,255), 3, get_color(0,0,0));
    short char_x = (x / 4) + 8;
    short char_y = (y / 4) + 2;
    
    for(int j = 0; j < 4; j++){
        if(enemy[j].life > 0){
            char_printf(char_x, char_y + (2*j), " |");                                      
            for(short i = 0; i < 24; i++){
                if(map_int(enemy[j].life, 0, enemy[j].max_life, 0, 24) > i){
                    char_put('#');
                } else {
                    char_put(' ');
                }
            }
            char_printf(char_x + 26, char_y + (2*j), "| %d/%d    ", enemy[j].life, enemy[j].max_life);
        } else {
            char_printf(char_x, char_y + (2*j), "                                   ");
        }
        
    }   
    
}

// Battle stuff end

// !!! Blocking code below !!!
// The code below blocks any other function from working, so call those last

short draw_list(const unsigned short x, const unsigned short y, const char* names[], const short length, const short max_names){
    char isPressed = 1;
    short index = 0;
    short offset = 0;

    // Render once
    for(int i = 0; i < max_names; i++){
        char_printf(x, y + (2*i), "  %s ", names[offset+i]);
    }   
    char_printf(x , y + (2*index), ">");

    while(1){
        // Control things (update shit)
        if((keyboard_is_pressed(UP_BUTTON) || keyboard_is_pressed(DOWN_BUTTON)) && !isPressed){
            // Change values
            if(keyboard_is_pressed(DOWN_BUTTON)){
                if(index >= max_names - 1){
                    //index = max_names - 1;  // Clamp index
                    offset++;
                    if(offset > (length-max_names)){offset = (length-max_names);}
                } else {
                    index++;
                }
            } else if(keyboard_is_pressed(UP_BUTTON)){
                if(index <= 0){
                    //index = 0;  // Clamp index
                    offset--;
                    if(offset < 0){offset = 0;}
                } else {
                    index--;
                }
            }

            // Render shit, in fact
            for(int i = 0; i < max_names; i++){
                char_printf(x, y + (2*i), "  %s ", names[offset+i]);
            }   
            char_printf(x , y + (2*index), ">");
        }

        // Process commands
        if(keyboard_is_pressed(RETURN_BUTTON) && !isPressed){
            return -1;
        }  

        if(keyboard_is_pressed(CONFIRM_BUTTON) && !isPressed){
            return index + offset;
        }

        isPressed = keyboard_is_pressed(UP_BUTTON) || keyboard_is_pressed(DOWN_BUTTON) || keyboard_is_pressed(RETURN_BUTTON) || keyboard_is_pressed(CONFIRM_BUTTON);
        delay(50);
    }
}

void draw_itens_menu(){
    const short x = 100;
    const short y = 45;
    
    unsigned short index = 0;
    unsigned short offset = 0;

    draw_hud();     // Redraw hud
    vga_draw_rect_fill(x, y, 125, 125, get_color(255,255,255), 3, get_color(0,0,0));
    short char_x = (x / 4) + 2;
    short char_y = (y / 4) + 2;

    const char* item_names[MAX_ITENS];

    for(int i = 0; i < MAX_ITENS; i++){
        if(itens[player.itens[i]].type == ITEM_TYPE_NONE){
            item_names[i] = "------";
        } else {
            item_names[i] = itens[player.itens[i]].name;
        }
    }

    draw_list(char_x, char_y, item_names, MAX_ITENS, 14);
}
void draw_weapons_menu();
void draw_armor_menu();
void draw_main_menu();

void draw_options_menu(){
    const char* options[] = {"Itens", "Armas", "Armaduras", "Feiticos"}; 
    short index = 0;
    do{
        draw_hud();     // Redraw hud
        vga_draw_rect_fill(100, 100, 66, 44, get_color(255,255,255), 3, get_color(0,0,0));
        index = draw_list(27,27, options, 4, 4);

        switch(index){
            case 0:
                draw_itens_menu();
                break;
            case 1:
                printf("Open weapons\n");
                break;
            case 2:
                printf("Open shields\n");
                break;
            case 3:
                printf("Open Spells\n");
                break;
            default:
                break;
        }
    } while(index != -1);
}


void draw_battle_options(){
    short x = 6;
    short y = 188;
    const char* options[] = {"Atacar", "Defender", "Feitico", "Item"};
    vga_draw_rect_fill(x, y, 118, 44, get_color(255,255,255), 3, get_color(0,0,0));
    short index = draw_list(6, 49, options, 4, 4);
    printf("%s\n", options[index]);
    switch(index){
        case 0:             // Attack
            break;
        case 1:             // Defend
            break;
        case 2:             // Spell
            break;
        case 3:             // Item
            break;
    }

}