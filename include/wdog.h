#ifndef WDOG_H_
#define WDOG_H_

#include "base.h"
#include "support_wdog.h"


#define WDOG_CR         ( REG16( WDOG_BASE + 0x29UL) )  // Watchdog Control Register

#define WDOG_M_CR_WDDIS         ( 1U << 6 )


#define WDOG_V_CR_WDCHK     ( 5U << 3 )

enum WDOG_ClkDiv
{
    WDOG_ClkDiv_2       = 0x0800,   // 5MHz,     Trigger in 51.2us
    WDOG_ClkDiv_4       = 0x0900,   // 2.5MHz,   Trigger in 102.4us
    WDOG_ClkDiv_8       = 0x0A00,   // 1.25MHz,  Trigger in 204.8us
    WDOG_ClkDiv_16      = 0x0B00,   // 625kHz,   Trigger in 409.6us
    WDOG_ClkDiv_32      = 0x0C00,   // 312.5kHz, Trigger in 819.2us
    WDOG_ClkDiv_64      = 0x0D00,   // 156kHz,   Trigger in 1.64ms
    WDOG_ClkDiv_128     = 0x0E00,   // 78kHz,    Trigger in 3.28ms
    WDOG_ClkDiv_256     = 0x0F00,   // 39kHz,    Trigger in 6.55ms
    WDOG_ClkDiv_512     = 0x0000,   // 19.5kHz,  Trigger in 13.1ms
    WDOG_ClkDiv_1024    = 0x0100,   // 9.8kHz,   Trigger in 26.2ms
    WDOG_ClkDiv_2048    = 0x0200,   // 4.9kHz,   Trigger in 52.4ms
    WDOG_ClkDiv_4096    = 0x0300,   // 2.4kHz,   Trigger in 104.9ms
    WDOG_ClkDiv_8192    = 0x0302,   // 1.2kHz,   Trigger in 209.7ms
    WDOG_ClkDiv_16384   = 0x0303,   // 610Hz,    Trigger in 419.4ms
    WDOG_ClkDiv_32768   = 0x0304,   // 305Hz,    Trigger in 838.9ms
    WDOG_ClkDiv_65536   = 0x0305,   // 152.6Hz,  Trigger in 1.677s
    WDOG_ClkDiv_131072  = 0x0306,   // 76.3Hz,   Trigger in 3.355s
    WDOG_ClkDiv_262144  = 0x0307,   // 38Hz,     Trigger in 6.711s
};

//
// Disable watchdog for situation like setting up system
//
static inline void WDOG_Disable(void)
{
    WDOG_CR |= WDOG_V_CR_WDCHK | WDOG_M_CR_WDDIS;
}

//
// Configure watchdog to restart device if the program halts
//
void WDOG_Setup(enum WDOG_ClkDiv clkDiv);

//
// Reset watchdog timer to avoid system restart
//
void WDOG_Feed(void);

//
// Reboot the system by using the watchdog
//
void WDOG_SystemReset(void);

#endif /* WDOG_H_ */
