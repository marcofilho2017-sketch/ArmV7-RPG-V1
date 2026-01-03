#include "strings.h"

#include "math.h"

void print_formatted(char* text, void(*put_function)(char), va_list* ap_ptr){
    va_list ap = *ap_ptr;

    char c = *text;
    while(c != '\0'){
        if(c == '%'){
            text++;
            char modifier = *text;
            switch(modifier){
                case 'd':
                case 'u': {
                        int value_int = va_arg(ap, int);
                        char output[16];
                        int total = 0;
                        do{
                            char mod = value_int % 10;
                            output[total] = mod + 0x30;
                            value_int /= 10;
                            total++;
                        }while(value_int);

                        for(int i = total-1; i >= 0; i--){
                            put_function(output[i]);
                        }  
                    }
                    break;
                case 'l': {
                        long value_long = va_arg(ap, long);
                        char output_long[16];
                        int total_long = 0;
                        do{
                            char mod = value_long % 10;
                            output_long[total_long] = mod + 0x30;
                            value_long /= 10;
                            total_long++;
                        }while(value_long);

                        for(int i = total_long-1; i >= 0; i--){
                            put_function(output_long[i]);
                        }
                    }
                    break;
                case 'f':
                    break;
                case 's': {
                        char* string = va_arg(ap, char*);
                        char c = *string;
                        while(c != '\0'){
                            put_function(*string);
                            c = *string;
                            string++;
                        }
                    }
                    break;
                case 'x':
                case 'p': {
                        int value_hex = va_arg(ap, int);
                        // Eu não me orgulho disso, mas parece ser mais rapido (provavelmente não é)
                        // TODO: Tentar fazer isso sem necessitar de acesso à memoria
                        char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
                        char firstNonZero = (modifier == 'p');
                        for(short i = 7; i >= 0; i--){
                            char c = digits[(value_hex >> (i * 4)) & 0xF];
                            if(c != '0'){
                                firstNonZero = 1;
                            }
                            if(firstNonZero || i == 0){
                                put_function(c);
                            }
                        }
                        break;
                    }
                case '%':
                    put_function('%');
                    break;
            }
        } else {
            put_function(c);
        }

        text++;
        c = *text;
    }
}


char strcmp(char* string_a, char* string_b){
    int i = 0;
    while(string_a[i] != '\0' && string_b[i] != '\0'){
        if(string_a[i] != string_b[i]){
            return 0;
        }
        i++;
    }
    if(string_a[i] != '\0' || string_b[i] != '\0'){
        return 0;
    }
    return 1;
}

void strcpy(char* destination, char* source){
    int i = 0;
    do{
        destination[i] = source[i];
        i++;
    } while(source[i] != '\0');
}

int strlen(char* str){
    int len = 0;
    while(*str != '\0'){
        len++;
        str++;
    }
    return len;
}

void memcpy(void* destination, void* source, int num_bytes){
    for(int i = 0; i < num_bytes; i++){
        ((char*)destination)[i] = ((char*)source)[i];
    }
}

int parse_int(char* str, int max_length){
    int i = 0;
    char num[16];
    while(i < max_length && str[i] != '\0' && str[i] != ' '){
        num[i] = str[i] - '0';
        i++;
    }
    int total = 0;
    for(int j = 0; j < i; j++){
        total += num[j] * pow_int(10, i-j-1);
    }
    return total;
}