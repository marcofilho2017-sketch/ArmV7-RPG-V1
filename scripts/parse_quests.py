# Script for parsing item data from the quests.json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/quests.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "quests.h", "w") as quests_h:
        quests_h.write("""#pragma once\n#include "quest.h"\n\n""")
        quests_h.write("typedef enum {\n")
        for quest_id in data:
            quests_h.write("    QUEST_" + quest_id + ",\n")
        quests_h.write("    QUEST_COUNT\n")
        quests_h.write("} questID;\n\n")
        quests_h.write("extern Quest_t quests[QUEST_COUNT];")

    # Write .c file
    with open(export_path + "quests.c", "w") as quests_c:
        quests_c.write("""#include "quests.h"\n#include "itens.h"\n#include "characters.h"\n\n""")
        for quest_id in data:
            # Declare quest existence
            quests_c.write("Quest_t " + quest_id + " = {\n")
            quests_c.write("    .name = \"" + data[quest_id]["name"] + "\",\n")
            quests_c.write("    .description = \"" + data[quest_id]["description"].replace("\n", "\\n") + "\",\n")
            quests_c.write("    .status = QUEST_STATUS_INACTIVE,\n")
            match data[quest_id]["condition"]:
                case "get_item":
                    quests_c.write("    .condition = CONDITION_GET_ITEM,\n")
                    quests_c.write("    .condition_param = {ITEM_" + data[quest_id]["condition_param"][0] +"},\n")
                case "give_item":
                    quests_c.write("    .condition = CONDITION_GIVE_ITEM,\n")
                    quests_c.write("    .condition_param = {ITEM_" + data[quest_id]["condition_param"][0] + ", CHARACTER_" + data[quest_id]["condition_param"][1] +"},\n")
                case "kill_enemies":
                    quests_c.write("    .condition = CONDITION_KILL_ENEMIES,\n")
                    quests_c.write("    .condition_param = {CHARACTER_" + data[quest_id]["condition_param"][0] + ", " + data[quest_id]["condition_param"][1] +"},\n")
                case "kill_enemy":
                    quests_c.write("    .condition = CONDITION_KILL_ENEMY,\n")
                    quests_c.write("    .condition_param = {CHARACTER_" + data[quest_id]["condition_param"][0] +"},\n")
                case "talk_to_npc":
                    quests_c.write("    .condition = CONDITION_TALK_TO_NPC,\n")
                    quests_c.write("    .condition_param = {CHARACTER_" + data[quest_id]["condition_param"][0] +"},\n")
                case "get_to_location":
                    quests_c.write("    .condition = CONDITION_GET_TO_LOCATION,\n")
                    quests_c.write("    .condition_param = {SCENE_" + data[quest_id]["condition_param"][0] +"},\n")
                case _:
                    print("manda problema")
                    raise ValueError("Wrong key used for condition: " + data[quest_id]["condition"] + "\nQuestID: " + quest_id)
            
            if "dialog_start" in data[quest_id]:
                quests_c.write("    .dialog_start = &dialogs[DIALOG_" + data[quest_id]["dialog_start"] + "],\n")
            else:
                quests_c.write("    .dialog_start = 0,\n")
            
            if "dialog_end" in data[quest_id]:
                quests_c.write("    .dialog_end = &dialogs[DIALOG_" + data[quest_id]["dialog_end"] + "]\n")
            else:
                quests_c.write("    .dialog_end = 0\n")
            quests_c.write("};\n\n")

            
        quests_c.write("const Quest_t* quests[] = {")
        for quest_id in data:
            quests_c.write("\n    " + quest_id + ",")
        quests_c.write("\n};\n")