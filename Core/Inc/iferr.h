/*
 * This header file provides macros for shortening error-handling.
 *
 * Example:
 * ```c
 * #include "iferr.h"
 *
 * void PanicHandler(void)
 * {
 *     __disable_irq();
 *     __asm("BKPT 1");
 * }
 *
 * HAL_StatusTypeDef init_peripheral(void)
 * {
 *     IFERR_RETE(some_fn());
 *     IFERR_PANIC(another_fn(arg));
 *     return HAL_OK;
 * }
 * ```
 */

#pragma once

#include "stm32f3xx_hal.h"

/*
 * The PanicHandler needs to be implemented, otherwise the linker will fail.
 */
void IfErr_PanicHandler(void);

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
 * If error, panic
 */
#define IFERR_PANIC(x)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            IfErr_PanicHandler();                                                                                      \
        }                                                                                                              \
    } while (0)
