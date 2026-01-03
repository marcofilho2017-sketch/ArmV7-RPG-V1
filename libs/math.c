#include "math.h"

int abs_int(int a){
    if(a < 0){
        a = -a;
    }
    return a;
}

int pow_int(int base, int exp){
    int result = 1;
    while (exp--) result *= base;
    return result;
}

int map_int(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
