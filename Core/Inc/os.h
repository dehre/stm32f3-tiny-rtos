#pragma once

#include <stdint.h>

#define NUMTHREADS 3    // maximum number of threads
#define STACKSIZE 100   // number of 32-bit words in stack
#define THREADFREQ 1000 // maximum time-slice before the scheduler is run, in Hz

void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

void OS_Init(uint32_t scheduler_frequency_hz);

void OS_Launch(void);

void OS_Scheduler(void);
