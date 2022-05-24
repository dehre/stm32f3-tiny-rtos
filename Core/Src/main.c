//==================================================================================================
// INCLUDES
//==================================================================================================

#include "iferr.h"
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
    /* Reset of all peripherals, initialize the Flash interface and the Systick. */
    IFERR_PANIC(HAL_Init());

    /* Configure the system clock */
    SystemClock_Config();

    OS_AddThreads(UserTask_0, UserTask_1, UserTask_2);
    OS_Init(10);
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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    IFERR_PANIC(HAL_RCC_OscConfig(&RCC_OscInitStruct));

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    IFERR_PANIC(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1));
}
