/**
 * The module stm32f3xx_it provides the interrupt and exception handlers.
 */

#pragma once

#include "schedl_timer.h"

//==================================================================================================
// CORTEX-M4 PROCESSOR INTERRUPTION AND EXCEPTION HANDLERS
//==================================================================================================

/**
 * The function NMI_Handler handles non maskable interrupts.
 */
void NMI_Handler(void);

/**
 * The function HardFault_Handler handles hard fault interrupts.
 */
void HardFault_Handler(void);

/**
 * The function MemManage_Handler handles memory management faults.
 */
void MemManage_Handler(void);

/**
 * The function BusFault_Handler handles pre-fetch faults and memory access faults.
 */
void BusFault_Handler(void);

/**
 * The function UsageFault_Handler handles undefined instructions or illegal states.
 */
void UsageFault_Handler(void);

/**
 * The function SVC_Handler handles system service calls via SWI instruction.
 */
void SVC_Handler(void);

/**
 * The function DebugMon_Handler handles debug monitors.
 */
void DebugMon_Handler(void);

/**
 * The function PendSV_Handler handles pendable requests for system service.
 */
void PendSV_Handler(void);

/**
 * The function SysTick_Handler handles system tick timer interrupts.
 */
void SysTick_Handler(void);

//==================================================================================================
// STM32F3xx PERIPHERAL INTERRUPT HANDLERS
// For the available peripheral interrupt handler names,
// refer to the startup file (startup_stm32f3xx.s).
//==================================================================================================

/**
 * The function SchedlTimer_IRQHandler handles SchedlTimer interrupts.
 * The implementation is in os_asm.s
 */
void SchedlTimer_IRQHandler(void);
