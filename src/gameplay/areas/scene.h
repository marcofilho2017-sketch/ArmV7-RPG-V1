#ifndef SCENE_H
#define SCENE_H

#include "character.h"
#include "item.h"
#include "dialogs.h"
#include "image.h"

// Affects mostly encounter behaviour 
typedef enum {
    SCENE_NONE,
    SCENE_CUTSCENE,
    SCENE_VILLAGE,
    SCENE_SHOP,
    SCENE_DUNGEON,
    SCENE_BOSS,
} SceneType_t;


typedef union {
    struct {
        unsigned int dialog;
    } cutscene;
    
    struct {
        int npc;           // NPC you could talk to here
        const char* dialog_options[4];    // Text that comes before the dialog
        unsigned int dialog[4];        // Dialog you can have with the NPC
        char num_dialogs;
    } village;

    struct {
        unsigned int npc;           // NPC handling the shop
        unsigned int itens[4];           // Itens being sold
        const char* dialog_options[4];    // Text that comes before the dialog
        unsigned int dialog[4];        // Dialogs that you can have at this store
        char num_dialogs;
        char num_itens;
    } shop;

    struct {
        unsigned int enemies[4];
        short enemy_chance;
        char num_enemies;
    } dungeon;
    
    struct {
        unsigned int enemies[4];
        int start_dialog;             // Dialog that comes before the fight
        int defeat_dialog;            // Dialog that comes after the fight
        char num_enemies;
    } boss;
} SceneData_t;


typedef struct Scene_t{
    SceneType_t type;                   // The type of this scene
    unsigned int background_image;  
    const char* adjacent_options[4];  
    unsigned int adjacent[4];               // A scene can mention other 4 scenes that area achiavable
    char num_adjacent;
    SceneData_t data;                   // Data held here
    char hasVisited;                    // flag to check if player has visited here before
} Scene_t;

#endif