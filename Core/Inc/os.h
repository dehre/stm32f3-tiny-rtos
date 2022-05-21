#pragma once

#include "stm32f3xx_hal.h"
#include <stdint.h>

#define NUMTHREADS 3    // maximum number of threads
#define STACKSIZE 100   // number of 32-bit words in stack
#define THREADFREQ 1000 // maximum time-slice before the scheduler is run, in Hz

void OS_AddThreads(void (*task0)(void), void (*task1)(void), void (*task2)(void));

HAL_StatusTypeDef OS_Init(uint32_t schedulerFrequencyHz);

HAL_StatusTypeDef OS_Launch(void);

void OS_Scheduler(void);
