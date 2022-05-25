.syntax unified @ See https://sourceware.org/binutils/docs/as/ARM_002dInstruction_002dSet.html
.cpu cortex-m4
.fpu softvfp
.thumb

@ The .global directive gives the symbols external linkage.
@ For clarity, the fn OSAsm_ThreadSwitch is exported as TIM2_IRQHandler, so that the vector table
@   in startup.s doesn't need to be modified.
.global OSAsm_Start
.set TIM2_IRQHandler, OSAsm_ThreadSwitch
.global TIM2_IRQHandler

.extern RunPt
.extern OSTimer_ClearITFlag
.extern OS_Scheduler

.section    .text.OSAsm_Start
.type	OSAsm_Start, %function
OSAsm_Start:
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

@ TODO LORIS: aligned comments, one space further

.section    .text.OSAsm_ThreadSwitch
.type	OSAsm_ThreadSwitch, %function
OSAsm_ThreadSwitch:
                               @ save R0-R3,R12,LR,PC,PSR
    CPSID   I                  @ prevent interrupt during context-switch
    PUSH    {R4-R11}           @ save remaining regs R4-R11
    LDR     R0, =RunPt         @ R0 = &RunPt; // TCB_t** R0  = &RunPt
    LDR     R1, [R0]           @ R1 = *R0;    // TCB_t*  R1  = RunPt
    STR     SP, [R1]           @ *R1 = SP;    // *(RunPt.sp) = SP

    PUSH    {R0, LR}           @ push R0 and LR, so that fn calls don't loose them
    BL      OSTimer_ClearITFlag   @ clear OSTimer interrupt flag
    BL      OS_Scheduler       @ call OS_Scheduler, RunPt is updated
    POP     {R0, LR}           @ restore R0 and LR

    LDR     R1, [R0]           @ R1 = *R0;    // TCB_t*   R1 = RunPt
    LDR     SP, [R1]           @ SP = *R1;    // uint32_t SP = *(RunPt.sp)
                               @ now we switched to the new thread's stack
    POP     {R4-R11}           @ restore regs R4-R11
    CPSIE   I                  @ tasks run with interrupts enabled
    BX      LR                 @ restore R0-R3,R12,LR,PC,PSR
