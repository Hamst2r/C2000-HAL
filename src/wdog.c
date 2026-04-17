#include "wdog.h"
#include "wdog_private.h"

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
