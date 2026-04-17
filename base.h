#ifndef BASE_H_
#define BASE_H_

// ########################################################
//
// For all compilers (TI or non-TI)
//
// ########################################################

#include <stdint.h>
#include <stdbool.h>


#ifndef NULL
    #define NULL 0
#endif

#if defined(__TMS320C28XX_CLA__)
    #define REG32(x)    (*((volatile uint32_t *)((uintptr_t)(x))))
    #define REG16(x)    (*((volatile uint16_t *)((uintptr_t)(x))))
#else
    #define REG32(x)    (*((volatile uint32_t *)(x)))
    #define REG16(x)    (*((volatile uint16_t *)(x)))
#endif

// Bitfield Masks
#define MASK(bitWidth)   ((1UL << (bitWidth)) - 1UL)


// // A value of ADJUST_FOR_1US = 4 and microseconds = 10 measured
// // 9.68us with the scope by GPIO toggling.
// #define ADJUST_FOR_1US      4
// #define DELAY_US(usec) do{ \
//     static volatile uint16 ixAdjst = 0;\
//     for(uint32 loopval = (usec); loopval > 0; loopval--){\
//         for(ixAdjst = ADJUST_FOR_1US; ixAdjst > 0; ixAdjst--);\
//         __asm(" NOP");\
//         __asm(" NOP");\
//         __asm(" NOP");}\
//     } while(0)

#ifdef __TI_COMPILER_VERSION__  // TI C28x or CLA
#ifdef __TMS320C28XX_CLA__     // CLA compiler
    // ########################################################
    //
    // TI CLA compiler (not regular C28x)
    //
    // ########################################################


    #ifndef EALLOW
        #define EALLOW __meallow()       // allow access to EALLOW-protected registers
    #endif



#else // end of CLA
    // ########################################################
    //
    // TI C28x Compiler (not CLA)
    //
    // ########################################################


    #ifndef EALLOW
        #define EALLOW __eallow()       // allow access to EALLOW-protected registers
    #endif

    extern __cregister volatile uint16_t IFR;
    extern __cregister volatile uint16_t IER;

    #ifndef EINT
        #define EINT()   __asm(" clrc INTM")    // enable interrupts
    #endif
    #ifndef DINT
        #define DINT()   __asm(" setc INTM")    // disable interrupts
    #endif
    #ifndef ERTM
        #define ERTM()   __asm(" clrc DBGM")    // enable debug events
    #endif
    #ifndef DRTM
        #define DRTM()   __asm(" setc DBGM")    // disable debug events
    #endif
    #ifndef ESTOP0
        #define ESTOP0 __asm(" ESTOP0")     // breakpoint 0
    #endif
    #ifndef ESTOP1
        #define ESTOP1 __asm(" ESTOP1")     // breakpoint 1
    #endif
    #ifndef NOP
        #define NOP    __asm(" NOP")        // no operation
    #endif
    #ifndef IDLE
        #define IDLE   __asm(" IDLE")       // low-power operation
    #endif

    #ifndef INTERRUPT
        #define INTERRUPT __interrupt
    #endif

    #ifndef PRAGMA
        #define PRAGMA(x) _Pragma(#x)
    #endif

    #define VAR_IN_SECTION(variable, section) \
        PRAGMA(DATA_SECTION(variable, #section))    // place variable in section

    // Atomic read to 32-bit peripheral register
    #define ATOMIC32(x)     __byte_peripheral_32((uint32_t *)(x))
    
    #define REG8(x)         __byte((int16_t *)(x),0)

    // these are deprecated because they are not function-like
    // yet they return values
    #ifndef DISABLE_INTERRUPTS
        #define DISABLE_INTERRUPTS __disable_interrupts()   // returns old state
    #endif

    #ifndef ENABLE_INTERRUPTS
        #define ENABLE_INTERRUPTS __enable_interrupts()     // returns old state
    #endif




#endif  // end of not CLA
#else   // end of TI compilers
    // ########################################################
    //
    // Non-TI compiler
    //
    // ########################################################

    #ifndef EALLOW
        #define EALLOW  // allow TI register access
    #endif

    #ifndef EINT
        #define EINT()  // enable interrupts
    #endif
    #ifndef DINT
        #define DINT()  // disable interrupts
    #endif
    #ifndef ERTM
        #define ERTM()  // enable debug events
    #endif
    #ifndef DRTM
        #define DRTM()  // disable debug events
    #endif
    #ifndef ESTOP0
        #define ESTOP0  // breakpoint 0
    #endif
    #ifndef ESTOP1
        #define ESTOP1  // breakpoint 1
    #endif
    #ifndef NOP
        #define NOP     // no operation
    #endif
    #ifndef IDLE
        #define IDLE    // low-power operation
    #endif

    #define VAR_IN_SECTION(variable, section) // place variable in section

    // Atomic read to 32-bit peripheral register
    #define ATOMIC32(x)     (*(uint32_t *)(x))

    #define REG8(x)         (*(int8_t *)(x))

    // these are deprecated because they are not function-like
    // yet they return values
    #ifndef DISABLE_INTERRUPTS
        #define DISABLE_INTERRUPTS (0)   // returns old state
    #endif
    #ifndef ENABLE_INTERRUPTS
        #define ENABLE_INTERRUPTS (0)     // returns old state
    #endif


#endif  // end of non-TI compilers

#endif // end of BASE_H


#endif /* BASE_H_ */
