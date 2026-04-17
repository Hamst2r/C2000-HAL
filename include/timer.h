#ifndef TIMER_H
#define TIMER_H

#include "base.h"
#include "clk.h"
#include "support_timer.h"
#include <stdint.h>


#define TIMER_BASE(timer)   (TIMER_START_ADDR + TIMER_INCREMENT_ADDR * (timer))

#define TIMER_PERIOD_MAX    (0)

enum TIMER
{
    TIMER_0     = 0,    // CPU Timer 0
    TIMER_1     = 1,    // CPU Timer 1
    TIMER_2_1us = 2     // CPU Timer 2, permanently assigned to 1us ticking service
};

//
// Register and field definitions for TIM at offset 0x0u
// CPU timer, counter register
//
#define TIMER_TIM(timer)              ( ATOMIC32( TIMER_BASE(timer) + 0x00UL ) ) // Timer Counter
    // this timer counts down, not up

struct TIMER_Status
{
    uint32_t secSinceReset; // Seconds since last reset. Up to 136 years. Updated by app code, please implement the update in a way that it won't cause significant jitter
};

extern struct TIMER_Status TIMER_status;

//
// Global functions
//
extern void TIMER_SetPrescale(enum TIMER timer, uint16_t prescale);
extern void TIMER_SetPeriod(enum TIMER timer, uint32_t period);
extern void TIMER_Setup(enum TIMER timer, uint16_t prescale, uint32_t period);
extern void TIMER_SetupOneUsTick(void);

//
// Global macros to operate the timers
//

//
// Read current time (which counts up in microseconds) the specified CPU Timer
//
#define TIMER_Now    ((int32_t)~TIMER_TIM(TIMER_2_1us))

//
// Get an expiry time, which is delay_in_us in the future
// Note that the time may not be more than 35 minutes
// For anything longer, consider using TIMER_GetExpiryTimeSec()
//
#define TIMER_GetExpiryTime(delayInUs)      (TIMER_Now + (int32_t)(delayInUs))
#define TIMER_GetExpiryTimeSec(delayInSec)  (TIMER_status.secSinceReset + (delayInSec))

//
// Get the time elapsed since the time provided
//
#define TIMER_Since(sinceTime)      (TIMER_Now - (int32_t)(sinceTime))
#define TIMER_SinceSec(sinceTime)   (TIMER_status.secSinceReset - (sinceTime))

//
// Check if the time indicated has expired
//
#define TIMER_HasExpired(expiryTime) (TIMER_Since(expiryTime) >= 0)
#define TIMER_HasExpiredSec(expiryTime) (TIMER_status.secSinceReset >= (expiryTime))

#endif /* TIMER_H */
