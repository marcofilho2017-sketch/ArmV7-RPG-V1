# Script for parsing item data from the itens.json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/scenes.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)
#[dialog_name][text_id][character/text]

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "scenes.h", "w") as scenes_h:
        scenes_h.write("""#pragma once\n#include "scene.h"\n\n""")
        scenes_h.write("typedef enum {\n")
        for scene_id in data:
            scenes_h.write("    SCENE_" + scene_id + ",\n")
        scenes_h.write("    SCENE_COUNT\n")
        scenes_h.write("} sceneID;\n\n")
        scenes_h.write("extern Scene_t* scenes[SCENE_COUNT];")

    # Write .c file
    with open(export_path + "scenes.c", "w") as scenes_c:
        scenes_c.write("""#include "scenes.h"\n#include "image.h"\n#include "characters.h"\n#include "dialogs.h"\n#include "itens.h"\n\n""")
        for scene_id in data:
            scene = data[scene_id]
            # Declare item existence
            scenes_c.write("Scene_t " + scene_id + " = {\n")
            # Define its type
            match scene["type"]:
                case "cutscene":
                    scene_type = "SCENE_CUTSCENE"
                case "village":
                    scene_type = "SCENE_VILLAGE"
                case "shop":
                    scene_type = "SCENE_SHOP"
                case "dungeon":
                    scene_type = "SCENE_DUNGEON"
                case "boss":
                    scene_type = "SCENE_BOSS"
                case _:
                    scene_type = "SCENE_NONE"
            scenes_c.write("    .type = " + scene_type + ",\n")
            
            scenes_c.write("    .background_image = IMAGE_" + scene["image"].split('.')[0] + ",\n")
            for i in range(0, len(scene["adjacent"])):
                scenes_c.write("    .adjacent_options[" + str(i) + "] = \"" + list(scene["adjacent"][i].keys())[0] + "\",\n")
                scenes_c.write("    .adjacent[" + str(i) + "] = SCENE_" + list(scene["adjacent"][i].values())[0] + ",\n")
            
            scenes_c.write("    .num_adjacent = " + str(len(scene["adjacent"])) + ",\n")
            scenes_c.write("    .hasVisited = 0,\n")     
            
            match scene["type"]:
                case "cutscene":
                    scenes_c.write("    .data.cutscene.dialog = DIALOG_" + scene["dialog"] + ",\n")
                case "village":
                    npc = "CHARACTER_" + scene["npc"] if "npc" in scene else 0
                    scenes_c.write("    .data.village.npc = " + npc + ",\n")
                    if "dialog" in scene:
                        for i in range(0, len(scene["dialog"])):
                            scenes_c.write(f"    .data.village.dialog_options[{i}] = \"{list(scene["dialog"][i].keys())[0]}\" ,\n")
                            scenes_c.write(f"    .data.village.dialog[{i}] = DIALOG_{list(scene["dialog"][i].values())[0]} ,\n")
                    scenes_c.write(f"    .data.village.num_dialogs = {len(scene["dialog"])},\n")
                case "shop":
                    npc = "CHARACTER_" + scene["npc"] if "npc" in scene else 0
                    scenes_c.write("    .data.shop.npc = " + str(npc) + ",\n")
                    if "dialog" in scene:
                        for i in range(0, len(scene["dialog"])):
                            scenes_c.write(f"    .data.shop.dialog_options[{i}] = \"{list(scene["dialog"][i].keys())[0]}\" ,\n")
                            scenes_c.write(f"    .data.shop.dialog[{i}] =  DIALOG_{list(scene["dialog"][i].values())[0]} ,\n")
                    scenes_c.write(f"    .data.shop.num_dialogs = {len(scene["dialog"])},\n")
                    for i in range(0, len(scene["itens"])):
                        scenes_c.write(f"    .data.shop.itens[{i}] = ITEM_{scene["itens"][i]},\n")
                    scenes_c.write(f"    .data.shop.num_itens = {len(scene["itens"])},\n")
                case "dungeon":
                    for i in range(0, len(scene["enemies"])):
                        scenes_c.write(f"    .data.dungeon.enemies[{i}] = CHARACTER_{scene["enemies"][i]},\n")
                    scenes_c.write(f"    .data.dungeon.num_enemies = {len(scene["enemies"])},\n")
                    scenes_c.write(f"    .data.dungeon.enemy_chance = {str(scene["chance"])},\n")
                case "boss":
                    scene_type = "SCENE_BOSS"
                case _:
                    scene_type = "SCENE_NONE"

            scenes_c.write("};\n\n")

            
        scenes_c.write("Scene_t* scenes[] = {")
        for scene_id in data:
            scenes_c.write("\n    &" + scene_id + ", ")
        scenes_c.write("\n};\n")