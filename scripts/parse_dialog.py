# Script for parsing dialogue data from the dialog json file

import json
import os



name_len_limit = 32         #
text_len_limit = 64         # Maximum Characters allowed for each text

current_directory = os.getcwd().replace("\\", "/")

file_path = current_directory + "/src/campaign/text.json"
export_path = current_directory + "/build/campaign_data/"

if not os.path.exists(export_path):
    os.mkdir(export_path)
#[dialog_name][text_id][character/text]

with open(file_path, "r") as file:
    data  = json.load(file)

    # Write .h file
    with open(export_path + "dialogs.h", "w") as dialog_h:
        dialog_h.write("""#pragma once\n#include "dialog_handler.h"\n\n""")
        dialog_h.write("typedef enum {\n")
        for dialog in data:
            dialog_h.write("    DIALOG_" + dialog + ",\n")
        dialog_h.write("    DIALOG_COUNT\n")
        dialog_h.write("} dialogID;\n\n")
        dialog_h.write("extern const Dialog_t dialogs[DIALOG_COUNT];")

    # Write .c file
    with open(export_path + "dialogs.c", "w") as dialog_c:
        dialog_c.write("""#include "dialogs.h"\n\n""")
        for dialog in data:
            dialog_c.write("static const Text_t " + dialog + "_lines[]  = {\n")
            for text in data[dialog]:
                dialog_c.write('    {"' + text[0] + '", "' + text[1].replace("\n", "\\n") + '"},\n')
            dialog_c.write("};\n\n")

        dialog_c.write("const Dialog_t dialogs[] = {\n")
        for dialog in data:
            dialog_c.write("    {" + dialog + "_lines, " + str(len(data[dialog])) + "},\n")
        dialog_c.write("};\n")