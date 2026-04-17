#include "cla.h"
#include "cla_private.h"

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
