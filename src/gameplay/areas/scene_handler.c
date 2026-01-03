#include "scene_handler.h"
#include "stdio.h"
#include "dialog_handler.h"
#include "hud.h"
#include "battle.h"
#include "delay.h"
#include "dialogs.h"
#include "characters.h"
#include "vga.h"
#include "drawing.h"
#include "itens.h"
#include "player.h"

Scene_t* active_scene = 0;

void update_graphics(){
    vga_fill(0);
    char_clear();
    vga_draw_img(0,30, images[active_scene->background_image].start);
    draw_hud();
    if(active_scene->type == SCENE_SHOP){
        vga_draw_rect_fill(150, 186, 160, 44, get_color(255, 255, 255), 3, get_color(0,0,0));
    }
    if(vga_is_double_buffered()){
        vga_switch_buffer();
    }
    if(char_is_double_buffered()){
        char_switch_buffer();
    }
    
}   

void clear_text(short x, short y, short len){
    // De allocate text used
    for(int i = 0; i < len; i++){
        char_printf(x-2, y + (2*i), "                             ");
    } 
}


void set_active_scene(Scene_t* new_scene){
    active_scene = new_scene;
}


// GENERAL TODO: Correct the rendering errors after returning from dialogs or nested lists
#define OPTIONS_X 5
#define OPTIONS_Y 48

void update_scene(){
    printf("DEBUG: scene_type: %d\n", active_scene->type);
    update_graphics();
    switch (active_scene->type){
        case SCENE_NONE:
            printf("ERROR: ACTUAL SCENE IS A NULL SCENE\nPLEASE CHECK CONFIGURATION.\n");
            break;



        case SCENE_CUTSCENE:
            // Call render for a cutscene, eventually
            start_dialog(&dialogs[active_scene->data.cutscene.dialog]);       // Call a dialog
            active_scene = scenes[active_scene->adjacent[0]];               // After that go to next scene
            break;




        case SCENE_VILLAGE: {
            // Call render for a village, eventually
            const char* village_option[5];
            char total_options = 0;
            if(active_scene->data.village.npc >= 0){    // If theres a NPC at village
                village_option[total_options] = (const char*)("Conversar");
                total_options++;
            }

            for(short i = 0; i < 4; i++){
                if(active_scene->adjacent_options[i] != 0){
                    village_option[total_options] = active_scene->adjacent_options[i];
                    total_options++;
                }
            }
            short index = -1;
            while(index == -1){
                index = draw_list(OPTIONS_X, OPTIONS_Y, village_option, total_options, total_options);
            }
            if(active_scene->data.village.npc >= 0){    // If theres a NPC at village
                if(index == 0){
                    clear_text(OPTIONS_X, OPTIONS_Y, total_options);
                    short talk_index = 0;
                    while(talk_index != -1){
                        talk_index = draw_list(OPTIONS_X, OPTIONS_Y, active_scene->data.village.dialog_options, active_scene->data.village.num_dialogs, active_scene->data.village.num_dialogs);
                        if(talk_index >= 0){
                            clear_text(OPTIONS_X, OPTIONS_Y, active_scene->data.village.num_dialogs);
                            start_dialog(&dialogs[active_scene->data.village.dialog[talk_index]]);
                            update_graphics();
                        }
                    }
                } else {
                    active_scene = scenes[active_scene->adjacent[index-1]];
                    clear_text(OPTIONS_X, OPTIONS_Y, total_options);
                }
            } else {
                active_scene = scenes[active_scene->adjacent[index]];
                clear_text(OPTIONS_X, OPTIONS_Y, total_options);
            }
        }
            break;




        case SCENE_SHOP: {
            // Call render for a shop, eventually
            if(active_scene->data.shop.npc < 0){
                printf("ERROR: ACTUAL SCENE HAS A NULL SHOPOWNER\nPLEASE CHECK SCENE CONFIGURATION.\n");
            }
            const char* shop_options[] = {"Conversar", "Comprar", "Vender", "Sair"};
            
            short index = 0;
            while(index != 3 && index != -1){
                index = draw_list(OPTIONS_X, OPTIONS_Y, shop_options, 4, 4);
                clear_text(OPTIONS_X, OPTIONS_Y, 4);
                if(index == 0){                 // Talk
                    short talk_index = 0;
                    while(talk_index != -1){
                        talk_index = draw_list(OPTIONS_X, OPTIONS_Y, active_scene->data.shop.dialog_options, active_scene->data.shop.num_dialogs, active_scene->data.shop.num_dialogs);
                        if(talk_index != -1){
                            clear_text(OPTIONS_X, OPTIONS_Y, active_scene->data.shop.num_dialogs);
                            start_dialog(&dialogs[active_scene->data.shop.dialog[talk_index]]);
                            update_graphics();
                        }
                    }
                    clear_text(OPTIONS_X, OPTIONS_Y, 4);
                } else if(index == 1){          // Buy
                    // TODO: IMPLEMENT BUY MECHANICS
                    short buy_index = 0;
                    while(buy_index != -1){
                        char* n = "";
                        const char* item_options[4] = {n,n,n,n};
                        for(int i = 0; i < active_scene->data.shop.num_itens; i++){
                            item_options[i] = itens[active_scene->data.shop.itens[i]].name;
                            char_printf(OPTIONS_X + 23, OPTIONS_Y + i*2, "$%d", itens[active_scene->data.shop.itens[i]].value);
                        }
                        buy_index = draw_list(OPTIONS_X, OPTIONS_Y, item_options, active_scene->data.shop.num_itens, active_scene->data.shop.num_itens);
                        for(int i = 0; i < MAX_ITENS; i++){
                            if(player.itens[i] == ITEM_empty){
                                player.itens[i] = active_scene->data.shop.itens[i];
                                player.money -= itens[active_scene->data.shop.itens[i]].value;
                                draw_hud(); // Show different money
                                break;
                            }
                        }
                    }
                    clear_text(OPTIONS_X, OPTIONS_Y, 4);
                } else if(index == 2){          // Sell
                    // TODO: IMPLEMENT SELL MECHANICS
                } else if(index == 3 || index == -1) {         // Exit
                    active_scene = scenes[active_scene->adjacent[0]];
                }
            }
            
        }
            break;



        case SCENE_DUNGEON: {
            // Call render for a dungeon, eventually
            if(millis() % 100 < active_scene->data.dungeon.enemy_chance){   // I should implement RNG function later...
                start_battle(active_scene->data.dungeon.enemies);           // Give this problem to the battle handler
                update_graphics();
            }
            
            short index = -1;
            while(index == -1){
                index = draw_list(OPTIONS_X, OPTIONS_Y, active_scene->adjacent_options, active_scene->num_adjacent, active_scene->num_adjacent);
                active_scene = scenes[active_scene->adjacent[index]];
                clear_text(OPTIONS_X, OPTIONS_Y, active_scene->num_adjacent);
            }
        }
            break;



        case SCENE_BOSS:{
            // Call render for a boss, eventually
            if(!active_scene->hasVisited){
                if(active_scene->data.boss.start_dialog >= 0){
                    start_dialog(&dialogs[active_scene->data.boss.start_dialog]);
                    update_graphics();
                }

                start_battle(active_scene->data.boss.enemies);
                update_graphics();

                if(active_scene->data.boss.defeat_dialog >= 0){
                    start_dialog(&dialogs[active_scene->data.boss.defeat_dialog]);
                    update_graphics();
                }
            }
            short index = -1;
            while(index == -1){
                index = draw_list(OPTIONS_X, OPTIONS_Y, active_scene->adjacent_options, active_scene->num_adjacent, active_scene->num_adjacent);
                active_scene = scenes[active_scene->adjacent[index]];
            }
        }
            break;


        default:
            break;
    }
    active_scene->hasVisited = 1;
}