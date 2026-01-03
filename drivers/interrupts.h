#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void config_interrupt(int N, void (*ISR)());
void gic_init(void);
void enable_irq();
void irq_handler(void);


#endif