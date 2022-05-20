.syntax unified @ See https://sourceware.org/binutils/docs/as/ARM_002dInstruction_002dSet.html
.cpu cortex-m4
.fpu softvfp
.thumb

@ The .global directive gives the symbols external linkage
.global OSAsm_Start

.section    .text.OSAsm_Start
.type	OSAsm_Start, %function
OSAsm_Start:
                               @ push regs R0-R3,R12,LR,PC,PSR to the current stack
    CPSID   I                  @ disable interrupts
    LDR     R0, =RunPt         @ R0 = &RunPt; // TCB_t**  R0 = &RunPt
    LDR     R1, [R0]           @ R1 = *R0;    // TCB_t*   R1 = RunPt
    LDR     SP, [R1]           @ SP = *R1;    // uint32_t SP = *(RunPt.sp)
                               @ now we switched to the thread's stack, which we populated before
    POP     {R4-R11}           @ pop regs R4-R11
    POP     {R0-R3}            @ pop regs R0-R3
    POP     {R12}              @ pop reg  R12
    POP     {LR}               @ discard LR
    POP     {LR}               @ pop PC to the link register (start location)
    POP     {R1}               @ discard PSR
    CPSIE   I                  @ enable interrupts
    BX      LR                 @ start first thread
