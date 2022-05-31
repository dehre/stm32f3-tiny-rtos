/**
 * The module onboard_user_button shows how to use the semaphore provided by the OS
 * to debounce the onboard user button on PA0.
 *
 * In short:
 * - the interrupt handler is registered on rising edges of PA0;
 * - the OnboardUserButton_Task is run by the OS, but blocks immediately waiting for SemaphoreButtonPressed;
 * - when the interrupt is triggered, SemaphoreButtonPressed is signaled;
 * - the task unblocks on SemaphoreButtonPressed, invokes the callback function, and debounces the button;
 * - the callback function toggles the onboard LED on PE8;
 * - the task blocks again on SemaphoreButtonPressed;
 *
 * How this module is used in the project:
 *
 * _stm32f3xx_it.c_
 * ```c
 * #include "onboard_user_button.h"
 *
 * void EXTI0_IRQHandler(void)
 * {
 *     OnboardUserButton_IRQHandler();
 * }
 * ```
 *
 * _main.c_
 * ```c
 * #include "onboard_user_button.h"
 *
 * int main(void)
 * {
 *     IFERR_PANIC(HAL_Init());
 *     SystemClock_Config();
 *
 *     OnboardUserButton_Init();
 *
 *     OS_Init(500);
 *     OS_Thread_CreateFirst(OnboardUserButton_Task);
 *     OS_Launch();
 * }
 * ```
 *
 * IMPORTANT!
 * For this module to behave properly, make sure THREADFREQ is at least 500Hz.
 * Conversely, if THREADFREQ is 1Hz, for example, OnboardUserButton_Task might as well run
 * four seconds after the button has been pressed, making the callback fn very unresponsive.
 */

#pragma once

void OnboardUserButton_Init(void);

void OnboardUserButton_IRQHandler(void);

void OnboardUserButton_Task(void);
