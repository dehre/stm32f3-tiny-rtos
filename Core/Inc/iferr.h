/**
 * The module iferr provides macros for shortening error-handling.
 * The fn panic is implemented by the caller.
 *
 * Example:
 * ```c
 * #include "iferr.h"
 *
 * HAL_StatusTypeDef fn_a(bool arg);
 * HAL_StatusTypeDef fn_b(void);
 *
 * void panic(void)
 * {
 *     __disable_irq();
 *     __asm("BKPT 1");
 * }
 *
 * HAL_StatusTypeDef init_peripheral(uint32_t arg1, bool arg2)
 * {
 *     assert_or_panic(arg1 < 10);
 *     IFERR_RETE (fn_a(arg2));
 *     IFERR_PANIC(fn_b());
 *     return HAL_OK;
 * }
 * ```
 */

#pragma once

#include "stm32f3xx_hal.h"

/**
 * The panic fn is implemented by the caller.
 */
void panic(void);

/**
 * Assert, or panic.
 */
#define assert_or_panic(expr) ((expr) ? (void)0U : panic())

/**
 * If error, return error.
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

/**
 * If error, panic.
 */
#define IFERR_PANIC(x)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        HAL_StatusTypeDef err = (x);                                                                                   \
        if (err != HAL_OK)                                                                                             \
        {                                                                                                              \
            panic();                                                                                                   \
        }                                                                                                              \
    } while (0)
