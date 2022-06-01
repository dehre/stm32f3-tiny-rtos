/**
 * The module os encapsulates the core functionality of the operating system and
 * exposes the functions for interacting with it.
 */

#pragma once

#include <stdint.h>

#define MAXNUMTHREADS 10 /* Maximum number of threads, allocated at compile time */
#define STACKSIZE 100    /* Number of 32-bit words in each TCB's stack */
#define THREADFREQ 1     /* Maximum time-slice, in Hz, before the scheduler is run */

#define OS_SCHEDL_PRIO_MIN UINT8_MAX    /* Lowest priority that can be assigned to a thread */
#define OS_SCHEDL_PRIO_MAX 0            /* Highest priority that can be assigned to a thread */
#define OS_SCHEDL_PRIO_MAIN_THREAD 200  /* Baseline priority to be assigned to main threads */
#define OS_SCHEDL_PRIO_EVENT_THREAD 100 /* Baseline priority to be assigned to event threads */

/**
 * The type Semaphore_t abstracts the semaphore's counter.
 * A value of type *Semaphore_t should only be updated through the fn OS_Semaphore_Wait
 * and OS_Semaphore_Signal.
 */
typedef int32_t Semaphore_t;

/**
 * Function descriptions are provided in os.c
 */

void OS_Init(uint32_t scheduler_frequency_hz);

void OS_Thread_CreateFirst(void (*task)(void), uint8_t priority);

void OS_Thread_Create(void (*task)(void), uint8_t priority);

void OS_Launch(void);

void OS_Scheduler(void);

void OS_Thread_Suspend(void);

void OS_Thread_Sleep(uint32_t sleep_duration_ms);

void OS_DecrementTCBsSleepDuration(void);

void OS_Thread_Kill(void);

void OS_Semaphore_Wait(Semaphore_t *sem);

void OS_Semaphore_Signal(Semaphore_t *sem);
