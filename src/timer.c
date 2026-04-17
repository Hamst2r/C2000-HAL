#include "timer.h"
#include "timer_private.h"

// 0x00 means prescale by 1, 0x01 means prescale by 2, etc
#define TICK_AT_1US    (uint16_t)(CLK_status.sysClkLogicalMHz - 1)
#define TICK_AT_10US   (uint16_t)(CLK_status.sysClkLogicalMHz*10 - 1)
#define TICK_AT_100US  (uint16_t)(CLK_status.sysClkLogicalMHz*100 - 1)
//
// Configure CPU Timer prescaling factor
//
void TIMER_SetPrescale(enum TIMER timer, uint16_t prescale)
{
    // In the register, 0x00 means prescale by 1, 0x01 means prescale by 2, etc
    uint16_t valueInRegister = prescale - 1;

    TIMER_TPR(timer) = TIMER_S_TPR_TDDR(valueInRegister);
    TIMER_TPRH(timer) = TIMER_S_TPRH_TDDRH(valueInRegister >> 8);
}

//
// Configure CPU Timer period
//
void TIMER_SetPeriod(enum TIMER timer, uint32_t period)
{
    TIMER_PRD(timer) = period-1;
}

/*******************************************************************
 * @brief Start CPU Timer counting
*******************************************************************/
void TIMER_Start(enum TIMER timer)
{
    TIMER_TCR(timer) &= ~TIMER_M_TCR_TSS;
}


//
// Setup a CPU Timer
//
void TIMER_Setup(enum TIMER timer, uint16_t prescale, uint32_t period)
{
    TIMER_SetPrescale(timer, prescale);
    TIMER_SetPeriod(timer, TIMER_PERIOD_MAX);
    TIMER_Start(timer);
}

void TIMER_SetupOneUsTick(void)
{
    uint16_t prescale = CLK_status.sysClkLogicalMHz;   // SYSCLK cycles in 1 us (logical)

    TIMER_SetPrescale(TIMER_2_1us, prescale);
    TIMER_SetPeriod(TIMER_2_1us, TIMER_PERIOD_MAX);
    TIMER_Start(TIMER_2_1us);
}
