//==================================================================================================
// INCLUDES
//==================================================================================================

#include "os.h"

#include "iferr.h"
#include "os_timer.h"

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//
// Thread Control Block
//
typedef struct TCB
{
    uint32_t *sp;     // stack pointer, valid for threads not running
    struct TCB *next; // pointer to circular-linked-list of TCBs
    uint32_t sleep;   // 0 means not sleeping
} TCB_t;

//==================================================================================================
// GLOBAL AND STATIC VARIABLES
//==================================================================================================

static TCB_t TCBs[NUMTHREADS];
static uint32_t Stacks[NUMTHREADS][STACKSIZE];

TCB_t *RunPt; // pointer to the currently running thread

//==================================================================================================
// FUNCTION PROTOTYPES
//==================================================================================================

//
// The fn OS_SetInitialStack and OS_AddThreads link together the threads in
//   a circular list and set up the stack for each of them as if they had
//   already been running and then suspended.
// Check "STM32 Cortex-M4 Programming Manual" page 18 for the list of processor core registers.
//
static void OS_SetInitialStack(uint32_t i);
void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

//
// The fn OS_Init sets the clock?, then initializes OS_Timer and Sleep_Timer.
// TODO LORIS: rephrase
//
HAL_StatusTypeDef OS_Init(uint32_t schedulerFrequencyHz);

//
// The fn OS_Launch enables OS_Timer and Sleep_Timer, then calls OSAsm_Start,
//   which starts the first thread.
//
HAL_StatusTypeDef OS_Launch(void);

//
// The fn OSAsm_Start, defined in os_asm.s, is called by OS_Launch once.
// It "restores" the first thread's stack on the main stack.
//
extern void OSAsm_Start(void);

//
// The fn OSAsm_ThreadSwitch, defined in os_asm.s, is periodically called by OS_Timer (ISR).
// It preemptively switches to the next thread, that is, it stores the stack of the running
//   thread and restores the stack of the next thread.
// It calls OS_Schedule to decide which thread is run next and update RunPt.
//
extern void OSAsm_ThreadSwitch(void);

//
// The fn OS_Scheduler is called by OSAsm_ThreadSwitch and is responsible for deciding
//   the thread that is run next.
//
void OS_Scheduler(void);

//
// The fn OS_Suspend halts the current thread and switches to the next.
// It's called by the running thread itself.
//
void OS_Suspend(void);

//
// The fn OS_Sleep makes the current thread dormant for a specified time.
// It's called by the running thread itself.
// The fn OS_DecrementTCBsSleepValue is called by Sleep_Timer every ms and decrements the
//   the value of sleep on the thread-control-blocks.
//
void OS_Sleep(uint32_t ms);
static void OS_DecrementTCBsSleepValue(void);

//==================================================================================================
// IMPLEMENTATION
//==================================================================================================

static void OS_SetInitialStack(uint32_t i)
{
    // From "STM32 Cortex-M4 Programming Manual" page 23: attempting to execute instructions when
    //   the T bit is 0 results in a fault or lockup.
    Stacks[i][STACKSIZE - 1] = 0x01000000; // Thumb Bit (PSR)
    // Stacks[i][STACKSIZE - 2] =           // R15 (PC) -> set later in fn OS_AddThreads
    Stacks[i][STACKSIZE - 3] = 0x14141414;  // R14 (LR)
    Stacks[i][STACKSIZE - 4] = 0x12121212;  // R12
    Stacks[i][STACKSIZE - 5] = 0x03030303;  // R3
    Stacks[i][STACKSIZE - 6] = 0x02020202;  // R2
    Stacks[i][STACKSIZE - 7] = 0x01010101;  // R1
    Stacks[i][STACKSIZE - 8] = 0x00000000;  // R0
    Stacks[i][STACKSIZE - 9] = 0x11111111;  // R11
    Stacks[i][STACKSIZE - 10] = 0x10101010; // R10
    Stacks[i][STACKSIZE - 11] = 0x09090909; // R9
    Stacks[i][STACKSIZE - 12] = 0x08080808; // R8
    Stacks[i][STACKSIZE - 13] = 0x07070707; // R7
    Stacks[i][STACKSIZE - 14] = 0x06060606; // R6
    Stacks[i][STACKSIZE - 15] = 0x05050505; // R5
    Stacks[i][STACKSIZE - 16] = 0x04040404; // R4

    TCBs[i].sp = &Stacks[i][STACKSIZE - 16]; // thread's stack pointer
}

void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void))
{
    TCBs[0].next = &(TCBs[1]);
    TCBs[1].next = &(TCBs[2]);
    TCBs[2].next = &(TCBs[0]);

    OS_SetInitialStack(0);
    Stacks[0][STACKSIZE - 2] = (int32_t)task0; // PC
    OS_SetInitialStack(1);
    Stacks[1][STACKSIZE - 2] = (int32_t)task1; // PC
    OS_SetInitialStack(2);
    Stacks[2][STACKSIZE - 2] = (int32_t)task2; // PC

    RunPt = &(TCBs[0]); // thread 0 will run first
}

HAL_StatusTypeDef OS_Init(uint32_t schedulerFrequencyHz)
{
    // TODO LORIS: setup clock as part of OS_Init ?

    // TODO LORIS: param to adjust frequency
    // TODO LORIS: should the interrupt handler be passed as arg to Timer_Init? as for blinky
    return OSTimer_Init();
}

HAL_StatusTypeDef OS_Launch(void)
{
    // TODO LORIS: do I really need to disable irq here?
    __disable_irq(); // prevent the timer's ISR from firing before OSAsm_Start is called
    // TODO LORIS: this fn won't return, so if an error happens, just crash
    IFERR_RETE(OSTimer_Start());
    OSAsm_Start();
    return HAL_OK;
}

void OS_Scheduler(void)
{
    RunPt = RunPt->next;
}
