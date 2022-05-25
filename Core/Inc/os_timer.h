/*
 * This module encapsulates the timer functionality used by the OS scheduler.
 */

#pragma once

#include "stm32f3xx_hal.h"

#define OSTimer_Instance TIM2
#define OSTimer_ClkEnable __HAL_RCC_TIM2_CLK_ENABLE
#define OSTimer_IRQn TIM2_IRQn
#define OSTimer_IRQPreemptPriority (TICK_INT_PRIORITY + 1)
#define OSTimer_IRQSubPriority 0
#define OSTimer_IRQHandler TIM2_IRQHandler

void OSTimer_Init(uint32_t reload_frequency_hz);

void OSTimer_Start(void);

void OSTimer_ClearITFlag(void);

/* implemented in os_asm.s */
void OSTimer_IRQHandler(void);
