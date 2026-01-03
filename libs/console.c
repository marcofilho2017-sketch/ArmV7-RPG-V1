#include "console.h"

#include "uart.h"
#include "strings.h"

char console_buffer[256];
int console_buffer_index = 0;

typedef struct{
    char name[32];                                  // how the command is called
    char num_param;                                 // how many parameters there are (will call a error if theres too little)
    void (*function_call)(char param[8][16]);      // The function called to parse the specific command
} command_t;

short console_total_commands = 0;  // Total commands added until now
command_t console_commands[64];        // We can add up to 64 different commands (can make it more later)

// This will be a custom code for every project, I got no idea on how to parse it better
void console_parse(){    
    char command[32];           // Function name with up to 32 characters
    char param[8][16];          // Up to 8 parameters, with 16 characters each
    
    if(console_buffer[0] == '/'){                               // If valid command starter
        int index = 1;
        int param_index = 0;
        while(console_buffer[index] != ' ' && console_buffer[index] != '\n' && console_buffer[index] != '\0' && param_index < 32){                    // Parse the command
            command[param_index] = console_buffer[index];
            index++;
            param_index++;
        }
        command[param_index] = '\0';
        char valid_command = 0;
        char command_index = 0;
        int num_param = 0;
        for(int i = 0; i < console_total_commands; i++){
            if(strcmp(console_commands[i].name, command)){
                valid_command = 1;
                command_index = i;
                num_param = console_commands[i].num_param;
            }
        }
        if(valid_command){
            for(int i = 0; i < num_param; i++){
                if(console_buffer[index] == '\n' || console_buffer[index] == '\0'){
                    printf("Not enough parameters. Expected: %d Got: %d\n", num_param, i);
                    valid_command = 0;
                    break;
                }
                index++;                            // Offset the last space character;
                param_index = 0;
                while((console_buffer[index] != ' ' || i == (num_param-1)) && console_buffer[index] != '\n' && console_buffer[index] != '\0' && param_index < 32){                    // Parse the command
                    param[i][param_index] = console_buffer[index];
                    index++;
                    param_index++;
                }
                param[i][param_index] = '\0';
            }
            if(valid_command){
                console_commands[(int)command_index].function_call(param);
            }
            
        } else {
            printf("Invalid command.\n");
        }
        
    } else {
        printf("Invalid.\n");
    }
    

    // After parsing, empty the buffer
    for(short i = 0; i <= console_buffer_index; i++){
        console_buffer[i] = 0;
    }
    console_buffer_index = 0; // Reset index to beginning
}

// TODO: add a description parameter for a function eventually
void console_print_help(){
    for(int i = 1; i < console_total_commands; i++){
        printf("/%s | num param: %d\n", console_commands[i].name, console_commands[i].num_param);
    }
}

void console_callback(char c){
    
    if(c == 0x7F){              // Backspace
        console_buffer_index--;
        console_buffer[console_buffer_index] = ' ';
    } else if(c == '\n'){       // Enter
        console_buffer[console_buffer_index] = '\0';
        console_parse();
    } else {                    // Anything else
        console_buffer[console_buffer_index] = c;
        console_buffer_index++;
    }
}

void console_add_command(char* name, char num_param, void(*function_call)(char[8][16])){
    strcpy(console_commands[console_total_commands].name, name);
    console_commands[console_total_commands].num_param = num_param;
    console_commands[console_total_commands].function_call = function_call;
    console_total_commands++;
}

void console_begin(){
    uart_begin();
    uart_set_callback(console_callback);
    console_add_command("help", 0, console_print_help);
    uart_enable_echo();
}