/*
 * This header file provides macros for shortening error-handling.
 *
 * Example:
 * ```c
 * #include "iferr.h"
 *
 * HAL_StatusTypeDef init_peripheral(void)
 * {
 *     IFERR_RETE(some_fn());
 *     IFERR_RETE(another_fn(arg));
 *     return HAL_OK;
 * }
 * ```
 */

// TODO LORIS: use a global variable to store file, fn, and line of error
//   so crashes will be less anonymous

#pragma once

#include "stm32f3xx_hal.h"

/*
 * If error, return error
 */
#define IFERR_RETE(x)                                                                                                  \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            return err;                                                                                                \
        }                                                                                                              \
    }
