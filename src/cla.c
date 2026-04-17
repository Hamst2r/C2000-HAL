#include "cla.h"


#define CLA_MVECT(task)         ( REG16( CLA_BASE + (task) ) )   // Task Interrupt Vector
#define CLA_MCTL                ( REG16( CLA_BASE + 0x10U ) )    // Control Register
#define CLA__MVECTBGRNDACTIVE   ( REG16( CLA_BASE + 0x1BU ) )    // Active register for MVECTBGRND.
#define CLA_SOFTINTEN           ( REG16( CLA_BASE + 0x1CU ) )    // CLA Software Interrupt Enable Register
#define CLA__MSTSBGRND          ( REG16( CLA_BASE + 0x1DU ) )    // Status register for the back ground task.
#define CLA__MCTLBGRND          ( REG16( CLA_BASE + 0x1EU ) )    // Control register for the back ground task.
#define CLA__MVECTBGRND         ( REG16( CLA_BASE + 0x1FU ) )    // Vector for the back ground task.
#define CLA_MIFR                ( REG16( CLA_BASE + 0x20U ) )    // Interrupt Flag Register
#define CLA_MIOVF               ( REG16( CLA_BASE + 0x21U ) )    // Interrupt Overflow Flag Register
#define CLA_MICLR               ( REG16( CLA_BASE + 0x23U ) )    // Interrupt Flag Clear Register
#define CLA_MICLROVF            ( REG16( CLA_BASE + 0x24U ) )    // Interrupt Overflow Flag Clear Register
#define CLA_MIER                ( REG16( CLA_BASE + 0x25U ) )    // Interrupt Enable Register
#define CLA_MIRUN               ( REG16( CLA_BASE + 0x26U ) )    // Interrupt Run Status Register
#define CLA__MPC                ( REG16( CLA_BASE + 0x28U ) )    // CLA Program Counter
#define CLA__MAR0               ( REG16( CLA_BASE + 0x2AU ) )    // CLA Auxiliary Register 0
#define CLA__MAR1               ( REG16( CLA_BASE + 0x2BU ) )    // CLA Auxiliary Register 1
#define CLA__MSTF               ( REG16( CLA_BASE + 0x2EU ) )    // CLA Floating-Point Status Register
#define CLA__MR0                ( REG16( CLA_BASE + 0x30U ) )    // CLA Floating-Point Result Register 0
#define CLA__MR1                ( REG16( CLA_BASE + 0x34U ) )    // CLA Floating-Point Result Register 1
#define CLA__MR2                ( REG16( CLA_BASE + 0x38U ) )    // CLA Floating-Point Result Register 2
#define CLA__MR3                ( REG16( CLA_BASE + 0x3CU ) )    // CLA Floating-Point Result Register 3
#define CLA__MPSACTL            ( REG16( CLA_BASE + 0x42U ) )    // CLA PSA Control Register
#define CLA__MPSA1              ( REG16( CLA_BASE + 0x44U ) )    // CLA PSA1 Register
#define CLA__MPSA2              ( REG16( CLA_BASE + 0x46U ) )    // CLA PSA2 Register

#define CLA_SRCLOCK             ( REG32( CLA_SRC_BASE + 0x00U ) )
#define CLA_SRCSEL(task)        ( REG32( CLA_SRC_BASE + 0x06U + ((task) >> 2) * 2 ) )


#define CLA_M_SRCSEL(task)              ( (0xFFU) << (((task) & 0x03U) * 8) )

#define CLA_S_SRCSEL(task,trig)         ( (trig) << (((task) & 0x03U) * 8) )

extern uint32_t Cla1ProgRunStart, Cla1ProgLoadStart, Cla1ProgLoadSize;
extern uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;


void CLA_Init(void)
{
    // Copy program code from FLASH to RAM
    memcpy((uint32_t *)&Cla1ProgRunStart, (uint32_t *)&Cla1ProgLoadStart, (uint32_t)&Cla1ProgLoadSize);

    // Copy constants from FLASH to RAM
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart, (uint32_t)&Cla1ConstLoadSize);

    // Assign RAM sections for CLA data and program
    CLA_SetupRAM();

    // Clear CLA Task vectors
    for(int task=0;task<8;task++)
    {
        CLA_MVECT(task) = 0;
    }

    // Clear source select configuration
    CLA_SRCSEL(0) = 0;
    CLA_SRCSEL(4) = 0;

    // Clear background Task configurations
    CLA__MCTLBGRND  = 0;
    CLA__MVECTBGRND = 0;
}



//*****************************************************************************
//
//! Setup CLA Task
//!
//! \param task is the CLA task of interest
//! \param trigger is the CLA task trigger source
//! \param taskAddr is the start address of the code for task
//!
//! Each CLA Task (1 to 8) has its own MVECTx register. When a task is
//! triggered, the CLA loads the MVECTx register of the task in question
//! to the MPC (CLA program counter) and begins execution from that point.
//! The CLA has a 16-bit address bus, and can therefore, access the lower
//! 64 KW space. The MVECTx registers take an address anywhere in this space.
//!
//! \return None.
//
//*****************************************************************************
void CLA_SetupTask(enum CLA_Task task, enum CLA_Trig trigger, void* taskAddr)
{
    // Suppressing #770-D: conversion from pointer to smaller integer
    // The CLA address range is 16 bits so the addresses passed to the MVECT
    // registers will be in the lower 64KW address space. Turn the warning
    // back on after the MVECTs are assigned addresses
    if(CLA_MVECT(task) != 0)
    {
        // Resource conflict: CLA Task. Stop execution
        ESTOP0;
        while(1);
    }

    CLA_SRCSEL(task) =  CLA_SRCSEL(task)
                     & ~CLA_M_SRCSEL(task)
                     |  CLA_S_SRCSEL(task, trigger);

    #pragma diag_suppress=770
    CLA_MVECT(task) = (uint16_t)taskAddr;
    #pragma diag_warning=770

    // Enable task
    CLA_MIER |= 1U << task;
}

// //*****************************************************************************
// //
// //! Setup CLA Background Task 
// //!
// //! \param trigger is the CLA task trigger source
// //! \param taskAddr is the start address of the code for task
// //!
// //! Specifies background task address and enables the background task, but would not start it
// //!
// //! \return None.
// //
// //*****************************************************************************
// void CLA_SetupBackgroundTask(enum CLA_Trig trigger, void* taskAddr)
// {
//     // Suppressing #770-D: conversion from pointer to smaller integer
//     // The CLA address range is 16 bits so the addresses passed to the MVECT
//     // registers will be in the lower 64KW address space. Turn the warning
//     // back on after the MVECTs are assigned addresses
//     if(CLA__MVECTBGRND != 0)
//     {
//         // Resource conflict: CLA Background Task. Stop execution
//         ESTOP0;
//         while(1);
//     }

//     CLA_SRCSEL(task) =  CLA_SRCSEL(task)
//                      & ~CLA_M_SRCSEL(task)
//                      |  CLA_S_SRCSEL(task, trigger);

//     #pragma diag_suppress=770
//     CLA__MVECTBGRND = (uint16_t)taskAddr;
//     #pragma diag_warning=770

//     // Enable task
//     CLA_MIER |= 1U << task;
// }
