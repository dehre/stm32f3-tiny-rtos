//==================================================================================================
// INCLUDES
//==================================================================================================

#include "iferr.h"
#include "onboard_user_button.h"
#include "os.h"
#include "user_tasks.h"

#include "stm32f3xx_hal.h"

//==================================================================================================
// DEFINES - MACROS
//==================================================================================================

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

static void SystemClock_Config(void);

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

//==================================================================================================
// GLOBAL FUNCTIONS
//==================================================================================================

int main(void)
{
    /* Reset all peripherals, initialize the Systick, configure the system clock */
    IFERR_PANIC(HAL_Init());
    SystemClock_Config();
    OnboardUserButton_Init();

    /* Set up and start the OS */
    OS_Init(THREADFREQ);
    OS_Thread_CreateFirst(UserTask_0);
    OS_Thread_Create(UserTask_1);
    OS_Thread_Create(UserTask_2);
    OS_Thread_Create(OnboardUserButton_Task);
    OS_Launch();

    /* This statement should not be reached */
    panic();
}

void panic(void)
{
    __disable_irq();
    __asm("BKPT 1");
}

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    IFERR_PANIC(HAL_RCC_OscConfig(&RCC_OscInitStruct));

    /* As the system clock frequency is just 8MHz (no PLL), it's safe not to reduce the bus clocks */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_CFGR_SW_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; /* Fmax = 36 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; /* Fmax = 72 MHz */
    IFERR_PANIC(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0));

    /* Each time HAL_RCC_ClockConfig() is called to configure the system clock frequency,
     * the variable SystemCoreClock is updated */
    assert_or_panic(SystemCoreClock == 8000000);
}
