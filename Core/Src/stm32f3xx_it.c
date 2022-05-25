//==================================================================================================
// INCLUDES
//==================================================================================================

#include "stm32f3xx_it.h"

#include "os_timer.h"

#include "core_cm4.h"
#include "stm32f3xx_hal.h"
#include <stdbool.h>

//==================================================================================================
// ENUMS - STRUCTS - TYPEDEFS
//==================================================================================================

typedef struct
{
    uint32_t CFSReg; /* Configurable Fault Status Register */
    uint16_t UFSReg; /* Usage Fault Status Registers */
    uint8_t BFSReg;  /* Bus Fault Status Registers */
    uint8_t MMFSReg; /* Memory Manage Fault Status Registers */
    uint32_t HFSReg; /* HardFault Status Register */

    /* Usage Faults */
    bool DIVBYZERO;
    bool UNALIGNED;
    bool NOCP;
    bool INVPC;
    bool INVSTATE;
    bool UNDEFINSTR;

    /* Bus Faults */
    bool BFARVALID;
    bool LSPERR;
    bool STKERR;
    bool UNSTKERR;
    bool IMPRECISERR;
    bool PRECISERR;
    bool IBUSERR;

    /* MemManage Faults */
    bool MMARVALID;
    bool MLSPERR;
    bool MSTKERR;
    bool MUNSTKERR;
    bool DACCVIOL;
    bool IACCVIOL;

    /* Hard Faults */
    bool DEBUGEVT;
    bool FORCED;
    bool VECTTBL;
} HardFaultStatusRegisters_t;

//==================================================================================================
// STATIC PROTOTYPES
//==================================================================================================

static void InspectHardFault(void);

//==================================================================================================
// STATIC VARIABLES
//==================================================================================================

static __USED HardFaultStatusRegisters_t HardFaultStatusRegs;

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
    /* To determine what caused the fault, inspect the global variable HardFaultStatusRegs.
     * Useful article: https://interrupt.memfault.com/blog/cortex-m-fault-debug */
    InspectHardFault();
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

//==================================================================================================
// STATIC FUNCTIONS
//==================================================================================================

static void InspectHardFault(void)
{
    uint32_t *cfsr_addr = (uint32_t *)0xE000ED28;
    uint32_t cfsr = *cfsr_addr;

    uint32_t *hfsr_addr = (uint32_t *)0xE000ED2C;
    uint32_t hfsr = *hfsr_addr;

    HardFaultStatusRegs = (HardFaultStatusRegisters_t){
        .CFSReg = cfsr,
        .UFSReg = cfsr >> SCB_CFSR_USGFAULTSR_Pos,
        .BFSReg = cfsr >> SCB_CFSR_BUSFAULTSR_Pos,
        .MMFSReg = cfsr >> SCB_CFSR_MEMFAULTSR_Pos,
        .HFSReg = hfsr,

        /* Usage Faults */
        .DIVBYZERO = cfsr & SCB_CFSR_DIVBYZERO_Msk,
        .UNALIGNED = cfsr & SCB_CFSR_UNALIGNED_Msk,
        .NOCP = cfsr & SCB_CFSR_NOCP_Msk,
        .INVPC = cfsr & SCB_CFSR_INVPC_Msk,
        .INVSTATE = cfsr & SCB_CFSR_INVSTATE_Msk,
        .UNDEFINSTR = cfsr & SCB_CFSR_UNDEFINSTR_Msk,

        /* Bus Faults */
        .BFARVALID = cfsr & SCB_CFSR_BFARVALID_Msk,
        .LSPERR = cfsr & SCB_CFSR_LSPERR_Msk,
        .STKERR = cfsr & SCB_CFSR_STKERR_Msk,
        .UNSTKERR = cfsr & SCB_CFSR_UNSTKERR_Msk,
        .IMPRECISERR = cfsr & SCB_CFSR_IMPRECISERR_Msk,
        .PRECISERR = cfsr & SCB_CFSR_PRECISERR_Msk,
        .IBUSERR = cfsr & SCB_CFSR_IBUSERR_Msk,

        /* MemManage Faults */
        .MMARVALID = cfsr & SCB_CFSR_MMARVALID_Msk,
        .MLSPERR = cfsr & SCB_CFSR_MLSPERR_Msk,
        .MSTKERR = cfsr & SCB_CFSR_MSTKERR_Msk,
        .MUNSTKERR = cfsr & SCB_CFSR_MUNSTKERR_Msk,
        .DACCVIOL = cfsr & SCB_CFSR_DACCVIOL_Msk,
        .IACCVIOL = cfsr & SCB_CFSR_IACCVIOL_Msk,

        /* Hard Faults */
        .DEBUGEVT = hfsr & SCB_HFSR_DEBUGEVT_Msk,
        .FORCED = hfsr & SCB_HFSR_FORCED_Msk,
        .VECTTBL = hfsr & SCB_HFSR_VECTTBL_Msk,
    };
}
