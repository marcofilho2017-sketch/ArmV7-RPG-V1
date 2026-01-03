#include "main_menu.h"

#include "hud.h"
#include "vga.h"

#include "player.h"
#include "itens.h"
#include "scenes.h"
#include "scene_handler.h"

// TODO for main menu
// - Have a cool menu screen
// - Have a continue, new game, and maybe options??
// - Have my name there
// - Be responsible for loading and setting up the whole game start
//

void main_menu(){

    char_clear();
    vga_fill(get_color(0,0,0));
    char_printf(24, 59, "Unnamed engine by Zephyrus 2025 - All Rights Reserved");
    char_printf(32, 18, "Jogo Sem Nome");
    const char* options[] = {"Novo jogo", "Continuar"};
    short index = -1;
    while(index == -1){
        index = draw_list(33, 30, options, 2, 2);
        
        // TODO: Remove this when SAVING is implemented
        if(index == 1){
            char_printf(16, 36, "Sistema de SAVE nao implementado ainda, foi mal :/");
            index = -1;
        }
    }

    // Prepare a New Game
    if(index == 0){

        // Set Starting Scene
        // Give control to main game loop

        for(short i = 0; i < MAX_ITENS; i++){player.itens[i] = ITEM_empty;}    // Clear character Inventory
        player.weapon = ITEM_empty;                                            // Set starting weapon and armor   
        player.armor = ITEM_empty;
        set_active_scene(scenes[SCENE_start_scene]);                           // Set initial scene
    }
    
    // Let main program update the rest
}