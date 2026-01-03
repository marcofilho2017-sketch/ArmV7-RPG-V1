#ifndef DELAY_H
#define DELAY_H

unsigned long millis();                 // Get actual millis value

void delay_begin();                     // Sets up necessary timers

void delay(unsigned long wait_time);    // Forces whole code to wait 

#endif