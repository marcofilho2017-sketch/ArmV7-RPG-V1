.section .vectors, "ax"
.global _start
.global irq_entry

_start:
    b reset_handler
    b dump
    b dump
    b dump
    b dump
    b dump
    b irq_entry
    b dump


reset_handler:
    //ldr r1, =was_active
    //ldr r0, [r1]
    //cmp r0, #1
    //bleq dump
    //mov r0, #1
    //str r0, [r1]
    ldr r0, =0x8000000
    mov sp, r0
    bl gic_init
    bl uart_begin
    bl main

halt:
    ldr r0, =halt_text
    bl printf
    b .


irq_entry:
    sub     lr, lr, #4        // ARM ajusta LR_irq para instrução seguinte, corrigir
    stmfd   sp!, {r0-r12, lr} // salva registradores
    bl      irq_handler       // chama sua função em C
    ldmfd   sp!, {r0-r12, lr} // restaura registradores
    subs    pc, lr, #0        // volta da interrupção restaurando CPSR


dump:
    push {r0-r12, lr}
    mrs r0, cpsr
    push {r0}
    mov r0, sp
    bl dump_core
    pop {r0}
    pop {r0-r12, lr}
    b .                       // Hold code forever

halt_text:
.ascii "ALERT: Program Halted.\n"
.byte 0

//was_active:
//.word 0           // Variable holding wheter the computer crashed sometime or not
