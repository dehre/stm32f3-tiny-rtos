/**
 * The module stm32f3xx_hal_msp initializes the MSPs (MCU Support Packages).
 */

#pragma once

#include "stm32f3xx_hal.h"

/**
 * The fn HAL_MspInit initializes the global MSP.
 */
void HAL_MspInit(void);

/**
 * The fn HAL_TIM_Base_MspInit initializes the TIMs low level resources.
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);
