/*
 * This module initializes the global MSP.
 */

#pragma once

#include "stm32f3xx_hal.h"

/**
 * @brief This function initializes the global MSP (MCU Support Package).
 */
void HAL_MspInit(void);

/**
 * @brief This function enables the timer's clock, sets its interrupt priority
 * on the NVIC, and enables the interrupts.
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
