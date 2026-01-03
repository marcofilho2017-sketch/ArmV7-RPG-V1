# Script for parsing item data from the itens.json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/spells.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)
#[dialog_name][text_id][character/text]

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "spells.h", "w") as spell_h:
        spell_h.write("""#pragma once\n#include "spell.h"\n\n""")
        spell_h.write("typedef enum {\n")
        for spell_id in data:
            spell_h.write("    SPELL_" + spell_id + ",\n")
        spell_h.write("    SPELLS_COUNT\n")
        spell_h.write("} spellID;\n\n")
        spell_h.write("extern const Spell_t spells[ITEM_COUNT];")

    # Write .c file
    with open(export_path + "spells.c", "w") as spell_c:
        spell_c.write("""#include "itens.h"\n\n""")
        for spell_id in data:
            # Declare item existence
            spell_c.write("static const Spell_t " + spell_id + " = {\n")
            spell_c.write("};\n\n")

            
        spell_c.write("const Spell_t spells[] = {\n")
        for spell_id in data:
            spell_c.write("\n    " + spell_id + ",")
        spell_c.write("\n};\n")