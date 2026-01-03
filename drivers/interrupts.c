#include "interrupts.h"
#include "stdio.h"

void (*ISR_fn[256])();

void config_interrupt(int N, void (*ISR)()) {
    int reg_offset, index, value, address;
    /* Configure the Interrupt Set-Enable Registers (ICDISERn).
    * reg_offset = (integer_div(N / 32) * 4
    * value = 1 << (N mod 32) */
    reg_offset = (N >> 3) & 0xFFFFFFFC;
    index = N & 0x1F;
    value = 0x1 << index;
    address = 0xFFFED100 + reg_offset;
    /* Now that we know the register address and value, set the appropriate bit */
    *(int *)address |= value;
    /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
    * reg_offset = integer_div(N / 4) * 4
    * index = N mod 4 */
    reg_offset = (N & 0xFFFFFFFC);
    index = N & 0x3;
    address = 0xFFFED800 + reg_offset + index;
    /* Now that we know the register address and value, write to (only) the
    * appropriate byte */
    *(char *)address = (char)1;

    ISR_fn[N] = ISR;        // Set pointer to adequate handler
}

void gic_init(void) {
    
    // Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all
    // priorities
    *((int *) 0xFFFEC104) = 0xFFFF;
    // Set CPU Interface Control Register (ICCICR). Enable signaling of
    // interrupts
    *((int *) 0xFFFEC100) = 1;
    // Configure the Distributor Control Register (ICDDCR) to send pending
    // interrupts to CPUs
    *((int *) 0xFFFED000) = 1;
}

void enable_irq() {
    asm volatile("mrs r0, CPSR");
    asm volatile("bic r0, r0, #0x80"); // limpa I-bit (IRQ disable)
    asm volatile("msr CPSR_c, r0");
}

void irq_handler(void) {
    int irq = *(volatile int *)(0xFFFEC10C); // Read the ICCIAR from the CPU Interface in the GIC
    
    ISR_fn[irq]();

    *(volatile int *)(0xFFFEC110) = irq; // Write to the End of Interrupt Register (ICCEOIR)
}
