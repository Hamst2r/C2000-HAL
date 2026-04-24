#include "intr.h"
#include "sys.h"
#include "intr_private.h"

void INTR_DefaultHandler(void);
void INTR_IllegalOperationHandler(void);
void INTR_NmiHandler(void);

void INTR_Init(void)
{
    
    // Disable and clear all interrupts at CPU
    INTR_DisableGlobal();
    IER = 0x0000U;
    IFR = 0x0000U;

    // Clear all PIE-IER and PIE-IFR registers
    for(uint16_t grp=1; grp<=12;grp++)
    {
        INTR_PIE_IER(grp) = 0U;
        INTR_PIE_IFR(grp) = 0U;
    }

    // Enable vector fetching from PIE block
    INTR_PIE_CTRL |= INTR_M_PIE_CTRL_ENPIE;


    // Initialize interrupt vectors
    // We skip the first three locations because they are initialized by Boot
    // ROM with boot variables.
    for(uint16_t id = 3U; id < 224U; id++)
    {
        INTR_VECTOR(id) = (uint32_t)INTR_DefaultHandler;
    }

    // NMI and ITRAP get their own handlers.
    INTR_VECTOR(INTR_Src_00_18_NMI) = (uint32_t)INTR_NmiHandler;
    INTR_VECTOR(INTR_Src_00_19_Illegal) = (uint32_t)INTR_IllegalOperationHandler;
}


static uint32_t INTR_GetVectorID(enum INTR_Src src)
{
    if(INTR_GetGroup(src) == 0)
    {
        // CPU Interrupt Vectors
        return(INTR_GetChannel(src));

    } else {
        // PIE Interrupt Vectors
        // Starts from 32 for Channel <= 8, 128 for Channel > 8
        // INTR_Grp encodes as Group#. eg. Group 5 is encoded as 0x05
        // INTR_Ch encodes as (Channel# - 1). eg. Channel 3 is encoded as 0x02
        return(24U + INTR_GetGroup(src) * 8
                   + ( (INTR_GetChannel(src) >= 8) ? 88 : 0 )
                   + INTR_GetChannel(src));
    }
}

//
//  Register ISR in the interrupt vector table
//
void INTR_RegisterISR(enum INTR_Src src, void* isr)
{
    // Compute Vector ID
    uint16_t vectorID = INTR_GetVectorID(src);

    INTR_VECTOR(vectorID) = (uint32_t)isr;
}

void INTR_Enable(enum INTR_Src src)
{
    uint16_t intStatus;
    uint16_t vectorID;
    enum INTR_Grp group;
    uint16_t groupMask;

    vectorID = INTR_GetVectorID(src);

    // Globally disable interrupts but save status
    intStatus = INTR_DisableGlobal();

    // PIE Interrupts
    if(vectorID >= 0x20U)
    {
        group = INTR_GetGroup(src);
        groupMask = 1U << (group - 1U);

        // Enable channel in PIE group
        INTR_PIE_IER(group) |= (1U << INTR_GetChannel(src));

        // Enable PIE Group Interrupt
        //
        IER |= groupMask;
    }
    // INT13, INT14, DLOGINT, & RTOSINT
    else if((vectorID >= 0x0DU) && (vectorID <= 0x10U))
    {
        IER |= 1U << (vectorID - 1U);
    }
    else
    {
        // Other interrupts
    }

    // Restore gloabl interrupt status
    INTR_RestoreGlobal(intStatus);
}

static void INTR_ClearIFR(uint16_t mask)
{
    switch(mask)
    {
        case 0x0001U:
            IFR &= ~(uint16_t)0x0001U;
            break;
        case 0x0002U:
            IFR &= ~(uint16_t)0x0002U;
            break;
        case 0x0004U:
            IFR &= ~(uint16_t)0x0004U;
            break;
        case 0x0008U:
            IFR &= ~(uint16_t)0x0008U;
            break;
        case 0x0010U:
            IFR &= ~(uint16_t)0x0010U;
            break;
        case 0x0020U:
            IFR &= ~(uint16_t)0x0020U;
            break;
        case 0x0040U:
            IFR &= ~(uint16_t)0x0040U;
            break;
        case 0x0080U:
            IFR &= ~(uint16_t)0x0080U;
            break;
        case 0x0100U:
            IFR &= ~(uint16_t)0x0100U;
            break;
        case 0x0200U:
            IFR &= ~(uint16_t)0x0200U;
            break;
        case 0x0400U:
            IFR &= ~(uint16_t)0x0400U;
            break;
        case 0x0800U:
            IFR &= ~(uint16_t)0x0800U;
            break;
        case 0x1000U:
            IFR &= ~(uint16_t)0x1000U;
            break;
        case 0x2000U:
            IFR &= ~(uint16_t)0x2000U;
            break;
        case 0x4000U:
            IFR &= ~(uint16_t)0x4000U;
            break;
        case 0x8000U:
            IFR &= ~(uint16_t)0x8000U;
            break;
        default:
            //
            // Invalid mask.
            //
            ESTOP0;
            while(1);
    }
}

void INTR_Disable(enum INTR_Src src)
{
    uint16_t intStatus;
    uint16_t vectorID;
    enum INTR_Grp group;
    uint16_t mask;

    vectorID = INTR_GetVectorID(src);

    // Globally disable interrupts but save status
    intStatus = INTR_DisableGlobal();

    // PIE Interrupts
    if(vectorID >= 0x20U)
    {
        group = INTR_GetGroup(src);
        mask  = 1U << (group - 1U);

        // Disable individual PIE interrupt
        INTR_PIE_IER(group) &= ~(1U << INTR_GetChannel(src));

        // Wait for any pending interrupts to get to the CPU
        SYS_NOP(5);

        INTR_ClearIFR(mask);

        // Acknowledge any interrupts
        INTR_PIE_ACK = mask;
    }

    // INT13, INT14, DLOGINT, & RTOSINT
    else if((vectorID >= 0x0DU) && (vectorID <= 0x10U))
    {
        mask = 1U << (vectorID - 1U);

        IER &= ~mask;
        // Wait for any pending interrupts to get to the CPU
        SYS_NOP(5);

        INTR_ClearIFR(mask);
    }
    else
    {
        // Other interrupts
    }

    // Restore gloabl interrupt status
    INTR_RestoreGlobal(intStatus);
}


//*****************************************************************************
//
//! \internal
//! The default interrupt handler.
//!
//! This is the default interrupt handler.  The Interrupt_initVectorTable()
//! function sets all vectors to this function.  Also, when an interrupt is
//! unregistered using the Interrupt_unregister() function, this handler takes
//! its place.  This should never be called during normal operation.
//!
//! The ESTOP0 statement is for debug purposes only. Remove and replace with an
//! appropriate error handling routine for your program.
//!
//! \return None.
//
//*****************************************************************************
void INTR_DefaultHandler(void)
{
    uint16_t pieVect;
    uint16_t vectID;

    //
    // Calculate the vector ID. If the vector is in the lower PIE, it's the
    // offset of the vector that was fetched (bits 7:1 of PIECTRL.PIEVECT)
    // divided by two.
    //
    pieVect = INTR_PIE_CTRL;
    vectID = (pieVect & 0xFEU) >> 1U;

    //
    // If the vector is in the upper PIE, the vector ID is 128 or higher.
    //
    if(pieVect >= 0x0E00U)
    {
        vectID += 128U;
    }

    //
    // Something has gone wrong. An interrupt without a proper registered
    // handler function has occurred. To help you debug the issue, local
    // variable vectID contains the vector ID of the interrupt that occurred.
    //
    ESTOP0;
    for(;;)
    {
        ;
    }
}

//*****************************************************************************
//
//! \internal
//! The default illegal instruction trap interrupt handler.
//!
//! This is the default interrupt handler for an illegal instruction trap
//! (ITRAP).  The Interrupt_initVectorTable() function sets the appropriate
//! vector to this function.  This should never be called during normal
//! operation.
//!
//! The ESTOP0 statement is for debug purposes only.  Remove and replace with
//! an appropriate error handling routine for your program.
//!
//! \return None.
//
//*****************************************************************************
void INTR_IllegalOperationHandler(void)
{
    //
    // Something has gone wrong.  The CPU has tried to execute an illegal
    // instruction, generating an illegal instruction trap (ITRAP).
    //
    ESTOP0;
    for(;;)
    {
        ;
    }
}

//*****************************************************************************
//
//! \internal
//! The default non-maskable interrupt handler.
//!
//! This is the default interrupt handler for a non-maskable interrupt (NMI).
//! The Interrupt_initVectorTable() function sets the appropriate vector to
//! this function.  This should never be called during normal operation.
//!
//! The ESTOP0 statement is for debug purposes only. Remove and replace with an
//! appropriate error handling routine for your program.
//!
//! \return None.
//
//*****************************************************************************
void INTR_NmiHandler(void)
{
    //
    // A non-maskable interrupt has occurred, indicating that a hardware error
    // has occurred in the system.  You can use SysCtl_getNMIFlagStatus() to
    // to read the NMIFLG register and determine what caused the NMI.
    //
    ESTOP0;
    for(;;)
    {
        ;
    }
}
