# Script for parsing dialogue data from the dialog json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/characters.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)
#[dialog_name][text_id][character/text]

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "characters.h", "w") as character_h:
        character_h.write("""#pragma once\n#include "character.h"\n\n""")
        character_h.write("typedef enum {\n")
        for character_id in data:
            character_h.write("    CHARACTER_" + character_id + ",\n")
        character_h.write("    CHARACTER_COUNT\n")
        character_h.write("} characterID;\n\n")
        character_h.write("extern const Character_t* characters[CHARACTER_COUNT];")

    # Write .c file
    with open(export_path + "characters.c", "w") as character_c:
        character_c.write("""#include "characters.h"\n#include "itens.h"\n#include "spells.h"\n\n""")
        for character_id in data:
            char = data[character_id]

            character_c.write("static const Character_t " + character_id + " = {\n")
            character_c.write("    .name = \"" + char["name"] +"\",\n")
            character_c.write("    .description = \"" + char["description"] +"\",\n")

            #configurar os valores (mais facil...)
            max_life = char["max_life"] if "max_life" in char else  0
            max_mana = char["max_mana"] if "max_mana" in char else  0
            xp = char["xp"] if "xp" in char else  0
            level = char["level"] if "level" in char else  0
            money = char["money"] if "money" in char else  0
            attack = char["attack"] if "attack" in char else  0
            defense = char["defense"] if "defense" in char else  0
            itens = char["itens"] if "itens" in char else  0
            weapon = char["weapon"] if "weapon" in char else  0
            armor = char["armor"] if "armor" in char else  0
            if "personality" in char:
                p = char["personality"]
                attack_chance = p["attack_chance"] if "attack_chance" in p else  0
                defense_chance = p["defense_chance"] if "defense_chance" in p else  0
                spell_chance = p["spell_chance"] if "weapon_chance" in p else  0
                item_chance = p["item_chance"] if "attack_chance" in p else  0
            else:
                attack_chance = 0
                defense_chance = 0
                spell_chance = 0
                item_chance = 0

            spells = char["spells"] if "spells" in char else  0
            isboss = char["isBoss"] if "isBoss" in char else  0
            
            # now put the data itself
            character_c.write("    .max_life = " + str(max_life) +",\n")
            character_c.write("    .max_mana = " + str(max_mana) +",\n")
            character_c.write("    .xp = " + str(xp) +",\n")
            character_c.write("    .level = " + str(level) +",\n")
            character_c.write("    .attack = " + str(attack) +",\n")
            character_c.write("    .defense = " + str(defense) +",\n")
            
            if itens != 0:
                for i in range(0, len(itens)):
                    character_c.write("    .itens[" + str(i) + "] = ITEM_" + str(itens[i]) +",\n")
            
            if weapon != 0:
                character_c.write("    .weapon = ITEM_" + str(weapon) +",\n")
            if armor != 0:
                character_c.write("    .armor = ITEM_" + str(armor) +",\n")
            
            character_c.write("    .personality.attack_chance = " + str(attack_chance) +",\n")
            character_c.write("    .personality.defense_chance = " + str(defense_chance) +",\n")
            character_c.write("    .personality.spell_chance = " + str(spell_chance) +",\n")
            character_c.write("    .personality.item_chance = " + str(item_chance) +",\n")

            if spells != 0:
                for i in range(0, len(spells)):
                    character_c.write("    .spells[" + str(i) +"] = SPELL_" + spells[i] + ",\n")
            
            character_c.write("    .isBoss = " + str(int(isboss)) +"\n")
            
            character_c.write("};\n\n")
            



        character_c.write("const Character_t* characters[] = {\n")
        for char_id in data:
            character_c.write("    &" + char_id + ",\n")
        character_c.write("};\n")