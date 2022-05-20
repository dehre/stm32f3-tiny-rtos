//==================================================================================================
// INCLUDES
//==================================================================================================

#include "os.h"

#include <stdint.h>

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
    int32_t *sp;      // stack pointer, valid for threads not running
    struct TCB *next; // pointer to circular-linked-list of TCBs
    uint32_t sleep;   // 0 means not sleeping
} TCB_t;

//==================================================================================================
// GLOBAL AND STATIC VARIABLES
//==================================================================================================

static TCB_t TCBs[NUMTHREADS];
static int32_t Stacks[NUMTHREADS][STACKSIZE];

TCB_t *RunPt; // pointer to the currently running thread

//==================================================================================================
// FUNCTION PROTOTYPES
//==================================================================================================

//
// The fn OS_SetInitialStack and OS_AddThreads link together the threads in
//   a circular list and set up the stack for each of them as if they had
//   already been running and then suspended.
//
static void OS_SetInitialStack(int32_t i);
int32_t OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

//
// The fn OS_Init sets the clock, then initializes SysTick and Timer0.
// TODO LORIS: rephrase
//
void OS_Init(uint32_t schedulerFrequencyHz);

//
// The fn OS_Launch enables SysTick and Timer0, then calls OSAsm_Start,
//   which starts the first thread.
// TODO LORIS: rephrase
//
void OS_Launch(void);

//
// The fn OSAsm_Start, defined in os-asm.s, is called by OS_Launch once.
// It "restores" the first thread's stack on the main stack.
//
extern void OSAsm_Start(void);

//
// The fn OSAsm_ThreadSwitch, defined in os-asm.s, is periodically called by SysTick (ISR).
// It preemptively switches to the next thread, that is, it stores the stack of the running
//   thread and restores the stack of the next thread.
// It calls OS_Schedule to decide which thread is run next and update `runPt`.
// TODO LORIS: rephrase
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
// The fn OS_DecrementTCBsSleepValue is called by Timer0 every ms and decrements the
//   the value of `sleep` on the thread-control-blocks.
// TODO LORIS: rephrase
//
void OS_Sleep(uint32_t ms);
static void OS_DecrementTCBsSleepValue(void);

//==================================================================================================
// IMPLEMENTATION
//==================================================================================================
