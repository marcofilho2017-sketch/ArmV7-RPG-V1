#ifndef TIMER_H
#define TIMER_H

typedef enum{
    TIMER0 = 0,
    TIMER1 = 1,
} Timer;

void timer_enable_interrupts(Timer timer);
void timer_disable_interrupts(Timer timer);
void timer_start(Timer timer);

void timer_stop(Timer timer);
void timer_set_continous(Timer timer);

void timer_set_oneshot(Timer timer);
void timer_set_interval(Timer timer, unsigned int interval, void(*callback)());


#endif