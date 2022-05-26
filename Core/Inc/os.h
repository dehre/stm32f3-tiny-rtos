/**
 * The module os encapsulates the core functionality of the operating system and
 * exposes the functions for interacting with it.
 */

#pragma once

#include <stdint.h>

#define NUMTHREADS 3  /* Maximum number of threads */
#define STACKSIZE 100 /* Number of 32-bit words in each TCB's stack */
#define THREADFREQ 1  /* Maximum time-slice, in Hz, before the scheduler is run */

/**
 * Function descriptions are provided in os.c
 */

void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

void OS_Init(uint32_t scheduler_frequency_hz);

void OS_Launch(void);

void OS_Scheduler(void);

void OS_Suspend(void);
