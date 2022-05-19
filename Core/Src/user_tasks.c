#include "user_tasks.h"

#include "stm32f3xx_hal.h"
#include <stdint.h>

// TODO LORIS: fn or macro to initialize peripherals,
//     maybe embed the initialization in the task itself.

void UserTask_0(void)
{
    static uint32_t n = 0;
    while (1)
    {
        if (n > 0x7FFFFF)
        {
            n = 0;
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
        }
        n++;
    }
}

void UserTask_1(void)
{
    static uint32_t n = 0;
    while (1)
    {
        if (n > 0xFFFFF)
        {
            n = 0;
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
        }
        n++;
    }
}

void UserTask_2(void)
{
    static uint32_t n = 0;
    while (1)
    {
        if (n > 0xFFFF)
        {
            n = 0;
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
        }
        n++;
    }
}
