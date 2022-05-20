/*
 * This module encapsulates the timer functionality used by the OS scheduler.
 */

#pragma once

#include "stm32f3xx_hal.h"

#define OSTimer_Instance TIM2
#define OSTimer_ClkEnable __HAL_RCC_TIM2_CLK_ENABLE
#define OSTimer_IRQn TIM2_IRQn
// TODO LORIS: set preempt priority to low, so that it doesn't interrupt other important ISRs
#define OSTimer_IRQPreemptPriority 4
#define OSTimer_IRQSubPriority 0
#define OSTimer_IRQHandler TIM2_IRQHandler

extern TIM_HandleTypeDef OSTimer_Handle;

HAL_StatusTypeDef OSTimer_Init(void);

HAL_StatusTypeDef OSTimer_Start(void);

void OSTimer_ClearITFlag(void);
