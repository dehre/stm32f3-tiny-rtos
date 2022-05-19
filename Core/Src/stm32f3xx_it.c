//==================================================================================================
// INCLUDES
//==================================================================================================

#include "stm32f3xx_it.h"

#include "stm32f3xx_hal.h"

//==================================================================================================
// CORTEX-M4 PROCESSOR INTERRUPTION AND EXCEPTION HANDLERS
//==================================================================================================

void NMI_Handler(void)
{
    while (1)
    {
    }
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

//==================================================================================================
// STM32F3xx PERIPHERAL INTERRUPT HANDLERS
// For the available peripheral interrupt handler names,
// please refer to the startup file (startup_stm32f3xx.s).
//==================================================================================================
