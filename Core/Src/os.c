//==================================================================================================
// INCLUDES
//==================================================================================================

#include "os.h"

#include "iferr.h"
#include "schedl_timer.h"

#include "stm32f3xx_hal.h"
#include <stdbool.h>

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

/**
 * TCBState indicates whether the TCB can be used by OS_ThreadCreate
 * to create a new thread.
 */
typedef enum
{
    TCBStateFree,
    TCBStateActive
} TCBState_t;

/**
 * Thread Control Block
 *
 * IMPORTANT!
 * The fn OSAsm_Start and OSAsm_ThreadSwitch, implemented in os_asm.s, expect the stack pointer
 * to be placed first in the struct. Don't shuffle it!
 */
typedef struct TCB
{
    uint32_t *sp;      /* Stack pointer, valid for threads not running */
    struct TCB *next;  /* Pointer to circular-linked-list of TCBs */
    uint32_t sleep;    /* Sleep duration in ms, zero means not sleeping */
    TCBState_t status; /* TCB active or free */
} TCB_t;

//==================================================================================================
// GLOBAL AND STATIC VARIABLES
//==================================================================================================

static TCB_t TCBs[MAXNUMTHREADS];
static uint32_t Stacks[MAXNUMTHREADS][STACKSIZE];

/* Pointer to the currently running thread */
TCB_t *RunPt;

// TODO LORIS: ? remove this variable, and replace it with number of active TCBs?
/* The flag Thread_FirstCreated indicates whether the first thread has been added
 * to the circular linked list of TCBs before the OS is launched */
static bool Thread_FirstCreated = false;

//==================================================================================================
// FUNCTION PROTOTYPES
//==================================================================================================

/**
 * The fn OS_InitTCBsStatus initializes all TCBs' statuses to be free at startup.
 */
static void OS_InitTCBsStatus(void);

/**
 * The fn OS_Init initializes the SchedlTimer and the TCBs.
 */
void OS_Init(uint32_t scheduler_frequency_hz);

/**
 * The fn OS_SetInitialStack and OS_AddThreads link together the threads in a circular list and
 * set up the stack for each of them as if they had already been running and then suspended.
 * Check the "STM32 Cortex-M4 Programming Manual" on page 18 for the list of processor core registers.
 */
static void OS_SetInitialStack(uint32_t tcb_idx);
// TODO LORIS: remove this fn
void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

/**
 * The fn OS_Thread_CreateFirst establishes the circular linked list of TCBs
 * with one node, and points RunPt to that node.
 * The fn must be called before the OS is launched.
 */
void OS_Thread_CreateFirst(void (*task)(void));

/**
 * The fn OS_Thread_Create adds a new thread to the circular linked list of TCBs, then runs it.
 * It fails if all the TCBs are already active.
 *
 * The fn can be called both:
 *   - before the OS is launched (but after the first thread is created);
 *   - after the OS is launched (by a running thread).
 *
 * The thread that calls this function keeps running until the end of its scheduled time-slice.
 * The new thread is run next.
 */
void OS_Thread_Create(void (*task)(void));

/**
 * The fn OS_Launch enables the SchedlTimer, then calls OSAsm_Start, which launches the first thread.
 */
void OS_Launch(void);

/**
 * The fn OSAsm_Start, implemented in os_asm.s, is called by OS_Launch once.
 * It "restores" the first thread's stack on the main stack.
 */
extern void OSAsm_Start(void);

/**
 * The fn OSAsm_ThreadSwitch, implemented in os_asm.s, is periodically called by the SchedlTimer (ISR).
 * It preemptively switches to the next thread, that is, it stores the stack of the running
 * thread and restores the stack of the next thread.
 * It calls OS_Schedule to determine which thread is run next and update RunPt.
 */
extern void OSAsm_ThreadSwitch(void);

/**
 * The fn OS_Scheduler is called by OSAsm_ThreadSwitch and is responsible for determining
 * which thread is run next.
 */
void OS_Scheduler(void);

// TODO LORIS: rename to OS_Thread_Suspend
/**
 * The fn OS_Suspend halts the current thread and switches to the next.
 * It's called by the running thread itself.
 */
void OS_Suspend(void);

// TODO LORIS: rename to OS_Thread_Sleep
/**
 * The fn OS_Sleep makes the current thread dormant for a specified time.
 * It's called by the running thread itself.
 * The fn OS_DecrementTCBsSleepDuration is called by the SysTick ISR every ms and decrements the
 * the value of sleep on the TCBs.
 */
void OS_Sleep(uint32_t ms);
void OS_DecrementTCBsSleepDuration(void);

//==================================================================================================
// IMPLEMENTATION
//==================================================================================================

static void OS_InitTCBsStatus(void)
{
    for (uint32_t idx = 0; idx < MAXNUMTHREADS; idx++)
    {
        TCBs[idx].status = TCBStateFree;
    }
}

void OS_Init(uint32_t scheduler_frequency_hz)
{
    SchedlTimer_Init(scheduler_frequency_hz);
    OS_InitTCBsStatus();
}

static void OS_SetInitialStack(uint32_t tcb_idx)
{
    /* From the "STM32 Cortex-M4 Programming Manual" on page 23:
     * attempting to execute instructions when  the T bit is 0 results in a fault or lockup */
    Stacks[tcb_idx][STACKSIZE - 1] = 0x01000000; /* Thumb Bit (PSR) */
    // Stacks[tcb_idx][STACKSIZE - 2] =           /* R15 (PC) -> set later in fn OS_AddThreads
    Stacks[tcb_idx][STACKSIZE - 3] = 0x14141414;  /* R14 (LR) */
    Stacks[tcb_idx][STACKSIZE - 4] = 0x12121212;  /* R12 */
    Stacks[tcb_idx][STACKSIZE - 5] = 0x03030303;  /* R3 */
    Stacks[tcb_idx][STACKSIZE - 6] = 0x02020202;  /* R2 */
    Stacks[tcb_idx][STACKSIZE - 7] = 0x01010101;  /* R1 */
    Stacks[tcb_idx][STACKSIZE - 8] = 0x00000000;  /* R0 */
    Stacks[tcb_idx][STACKSIZE - 9] = 0x11111111;  /* R11 */
    Stacks[tcb_idx][STACKSIZE - 10] = 0x10101010; /* R10 */
    Stacks[tcb_idx][STACKSIZE - 11] = 0x09090909; /* R9 */
    Stacks[tcb_idx][STACKSIZE - 12] = 0x08080808; /* R8 */
    Stacks[tcb_idx][STACKSIZE - 13] = 0x07070707; /* R7 */
    Stacks[tcb_idx][STACKSIZE - 14] = 0x06060606; /* R6 */
    Stacks[tcb_idx][STACKSIZE - 15] = 0x05050505; /* R5 */
    Stacks[tcb_idx][STACKSIZE - 16] = 0x04040404; /* R4 */

    TCBs[tcb_idx].sp = &Stacks[tcb_idx][STACKSIZE - 16]; /* Thread's stack pointer */
}

void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void))
{
    TCBs[0].next = &(TCBs[1]);
    TCBs[1].next = &(TCBs[2]);
    TCBs[2].next = &(TCBs[0]);

    OS_SetInitialStack(0);
    Stacks[0][STACKSIZE - 2] = (int32_t)task0; /* PC */
    OS_SetInitialStack(1);
    Stacks[1][STACKSIZE - 2] = (int32_t)task1; /* PC */
    OS_SetInitialStack(2);
    Stacks[2][STACKSIZE - 2] = (int32_t)task2; /* PC */

    /* Thread 0 will run first */
    RunPt = &(TCBs[0]);
}

// TODO LORIS: merge this method with OS_Thread_Create
void OS_Thread_CreateFirst(void (*task)(void))
{
    TCBs[0].next = &(TCBs[0]);
    TCBs[0].sleep = 0;
    TCBs[0].status = TCBStateActive;

    OS_SetInitialStack(0);                    /* TCBs[0].sp set here */
    Stacks[0][STACKSIZE - 2] = (int32_t)task; /* PC */

    /* Thread 0 will run first */
    RunPt = &(TCBs[0]);
    Thread_FirstCreated = true;
}

void OS_Thread_Create(void (*task)(void))
{
    __disable_irq();
    uint32_t new_tcb_idx;
    for (new_tcb_idx = 0; new_tcb_idx < MAXNUMTHREADS; new_tcb_idx++)
    {
        if (TCBs[new_tcb_idx].status == TCBStateFree)
            break;
    }
    if (new_tcb_idx == MAXNUMTHREADS)
    {
        // TODO LORIS: return enum OS_Error
        return;
    }

    TCBs[new_tcb_idx].next = RunPt->next;
    RunPt->next = &(TCBs[new_tcb_idx]);

    TCBs[new_tcb_idx].sleep = 0;
    TCBs[new_tcb_idx].status = TCBStateActive;

    OS_SetInitialStack(new_tcb_idx);
    Stacks[new_tcb_idx][STACKSIZE - 2] = (int32_t)task; /* PC */
    __enable_irq();
}

void OS_Launch(void)
{
    /* Prevent the timer's ISR from firing before OSAsm_Start is called */
    __disable_irq();

    SchedlTimer_Start();
    OSAsm_Start();

    /* This statement should not be reached */
    panic();
}

void OS_Scheduler(void)
{
    RunPt = RunPt->next;
    /* Skip sleeping threads; this implementation crashes if all threads are sleeping */
    while ((RunPt->sleep) > 0)
    {
        RunPt = RunPt->next;
    }
}

void OS_Suspend(void)
{
    SchedlTimer_ResetCounter();
}

void OS_Sleep(uint32_t sleep_duration_ms)
{
    RunPt->sleep = sleep_duration_ms;
    OS_Suspend();
}

void OS_DecrementTCBsSleepDuration(void)
{
    for (size_t tcb_idx = 0; tcb_idx < NUMTHREADS; tcb_idx++)
    {
        if (TCBs[tcb_idx].sleep > 0)
        {
            TCBs[tcb_idx].sleep -= 1;
        }
    }
}
