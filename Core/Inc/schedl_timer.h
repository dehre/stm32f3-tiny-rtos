/**
 * The module schedl_timer abstract the timer functionality used by the OS scheduler.
 */

#pragma once

#include "stm32f3xx_hal.h"

#define SchedlTimer_Instance TIM2
#define SchedlTimer_ClkEnable __HAL_RCC_TIM2_CLK_ENABLE
#define SchedlTimer_IRQn TIM2_IRQn
#define SchedlTimer_IRQPreemptPriority (TICK_INT_PRIORITY + 1)
#define SchedlTimer_IRQSubPriority 0
#define SchedlTimer_IRQHandler TIM2_IRQHandler

void SchedlTimer_Init(uint32_t reload_frequency_hz);

void SchedlTimer_Start(void);

void SchedlTimer_ClearITFlag(void);

/* Implemented in os_asm.s */
void SchedlTimer_IRQHandler(void);
