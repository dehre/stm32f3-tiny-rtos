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

#pragma once

#include "stm32f3xx_hal.h"

// TODO LORIS: IFERR_BKPT turns on onboard red led in both debug and release;
//    it needs iferr_init() to be called, which initializes gpio pin.

/*
 * If error, return error
 */
#define IFERR_RETE(x)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            return err;                                                                                                \
        }                                                                                                              \
    } while (0)

/*
 * If error, hang there
 */
#ifdef DEBUG
#define IFERR_BKPT(x)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            __asm("BKPT 1");                                                                                           \
        }                                                                                                              \
    } while (0)
#else
#define IFERR_BKPT(x)                                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            while (1)                                                                                                  \
            {                                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
#endif
