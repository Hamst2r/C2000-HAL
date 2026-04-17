#include "wdog.h"

#define WDOG_SCSR       ( REG16( WDOG_BASE + 0x22UL) )  // System Control & Status Register
#define WDOG_CNTR       ( REG16( WDOG_BASE + 0x23UL) )  // Watchdog Counter Register
#define WDOG_KEY        ( REG16( WDOG_BASE + 0x25UL) )  // Watchdog Reset Key Register
#define WDOG_WCR        ( REG16( WDOG_BASE + 0x2AUL) )  // Watchdog Windowed Control Register


#define WDOG_M_SCSR_WDOVERRIDE  ( 1U )


#define WDOG_V_CR_SYS_RST   (0)  // Value different than WDCR_WDCHK = 5, to cause a system reset.
#define WDOG_V_KEY1         ( 0x55 )
#define WDOG_V_KEY2         ( 0xAA )



void WDOG_SetupWatchdog(void)
{
    // The watchdog's 8bit counter ticks at 10MHz, therefore
    // timeout = SETS_WD_PREDIVIDER * SETS_WD_PRESCALER * 256 / 10e6 = 3.35s
    WDOG_CR = WDOG_V_CR_WDCHK | WDOG_ClkDiv_131072;

    WDOG_SCSR = WDOG_M_SCSR_WDOVERRIDE;
}

void WDOG_WakeupWatchdog(void)
{
    WDOG_KEY = WDOG_V_KEY1;
    WDOG_KEY = WDOG_V_KEY2;
}

void WDOG_SystemReset(void)
{
    DISABLE_INTERRUPTS;
    WDOG_CR = WDOG_V_CR_SYS_RST;
}
