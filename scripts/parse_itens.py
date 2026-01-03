# Script for parsing item data from the itens.json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/itens.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)
#[dialog_name][text_id][character/text]

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "itens.h", "w") as itens_h:
        itens_h.write("""#pragma once\n#include "item.h"\n\n""")
        itens_h.write("typedef enum {\n")
        itens_h.write("    ITEM_empty,\n")
        for item_id in data:
            itens_h.write("    ITEM_" + item_id + ",\n")
        itens_h.write("    ITEM_COUNT\n")
        itens_h.write("} itemID;\n\n")
        itens_h.write("extern const Item_t itens[ITEM_COUNT];")

    # Write .c file
    with open(export_path + "itens.c", "w") as itens_c:
        itens_c.write("""#include "itens.h"\n\n""")

        # Create Empty item
        itens_c.write("""
static const Item_t empty_item = {
    .type = ITEM_TYPE_NONE,
    .name = "--------",
    .description = "",
    .value = 0,
    .data.effect.attack = 0,
    .data.effect.defense = 0,
    .data.effect.duration = 0,
    .data.effect.life = 0
};

""")
        for item_id in data:
            # Declare item existence
            itens_c.write("static const Item_t " + item_id + " = {\n")
            # Define its type
            match data[item_id]["type"]:
                case "weapon":
                    item_type = "ITEM_TYPE_WEAPON"
                case "armor":
                    item_type = "ITEM_TYPE_ARMOR"
                case "consumable":
                    item_type = "ITEM_TYPE_CONSUMABLE"
                case "key":
                    item_type = "ITEM_TYPE_KEY"
                case _:
                    item_type = "ITEM_TYPE_NONE"
            itens_c.write("    .type = " + item_type + ",\n")
            
            # Write name
            itens_c.write("    .name = \"" + data[item_id]["name"] + "\",\n")
            
            # Write description
            itens_c.write("    .description = \"" + data[item_id]["description"].replace("\n", "\\n") + "\",\n")
            
            # Write value
            item_value = 0
            if "value" in data[item_id]:
                item_value = data[item_id]["value"]
            itens_c.write("    .value = " + str(item_value) + ",\n")
            
            # Write Item Data
            if data[item_id]["type"] != "key":
                if "effect" in data[item_id]:
                    itens_c.write("    .data.effect.attack = " + str(data[item_id]["effect"]["attack"]) + ",\n") if "attack" in data[item_id]["effect"] else ""
                    itens_c.write("    .data.effect.defense = " + str(data[item_id]["effect"]["defense"]) + ",\n") if "defense" in data[item_id]["effect"] else ""
                    itens_c.write("    .data.effect.duration = " + str(data[item_id]["effect"]["duration"]) + ",\n") if "duration" in data[item_id]["effect"] else ""
                    itens_c.write("    .data.effect.life = " + str(data[item_id]["effect"]["life"]) + ",\n") if "life" in data[item_id]["effect"] else ""
            itens_c.write("};\n\n")

            
        itens_c.write("const Item_t itens[] = {\n")
        itens_c.write("    empty_item,")
        for item_id in data:
            itens_c.write("\n    " + item_id + ",")
        itens_c.write("\n};\n")