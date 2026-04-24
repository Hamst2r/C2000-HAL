#ifndef XBAR_H_
#define XBAR_H_

#include "base.h"
#include "support_xbar.h"
#include "cmpss.h"
#include "epwm.h"

//
// XBAR module is consist of these submodules:
// - Input XBAR / CLB Input XBAR
// - ePWM XBAR (Trip)
// - CLB XBAR (if CLB exists on device)
// - Output XBAR / CLB Ouptut XBAR
//

#define XBAR_FLAG(flag)             ( REG32( XBAR_FLAG_BASE + (flag) >> 5))
#define XBAR_CLEAR(flag)            ( REG32( XBAR_FLAG_BASE + (flag) >> 5 + 0x08UL))

#define XBAR_INPUT_SELECT(xbar)     ( REG16( XBAR_INPUT_BASE + XBAR_GetOffset(xbar) + XBAR_GetChannel(xbar) ) )

#define XBAR_TRIP_ENABLE(trip)      ( REG32( XBAR_TRIP_BASE + 0x20 + (trip) * 2) )

#define XBAR_OUTPUT_LATCHFRC(xbar)  ( REG16( XBAR_OUTPUT_BASE + XBAR_GetOffset(xbar) + 0x34UL) )

#define XBAR_Flag_Mask(flag)        (1UL << (flag & 0x1F))

#define XBAR_GetOffset(xbar)        ((xbar) & 0xFFF0)
#define XBAR_GetChannel(xbar)       ((xbar) & 0x000F)

//
// Input XBAR submodule
//

void XBAR_Input_SetSource(enum XBAR_Input inputXbar, uint16_t gpio);

static inline void XBAR_Input_SetLow(enum XBAR_Input inputXbar)
{
    XBAR_INPUT_SELECT(inputXbar) = 0xFFFF;
}

static inline void XBAR_Input_SetHigh(enum XBAR_Input inputXbar)
{
    XBAR_INPUT_SELECT(inputXbar) = 0xFFFE;
}

static inline void XBAR_TriggerFWSD(void)
{
    XBAR_Input_SetHigh(XBAR_Input_6_FWSD);
}

static inline void XBAR_ClearFWSD(void)
{
    XBAR_Input_SetLow(XBAR_Input_6_FWSD);
}



// 
// ePWM XBAR (Trip) submodule
//

enum EPWM_DcSrc XBAR_Trip_SetupCMPSS(enum XBAR_Trip trip, enum CMPSS_CMP cmp);

static inline uint32_t XBAR_Trip_GetEnable(enum XBAR_Trip trip)
{
    return XBAR_TRIP_ENABLE(trip);
}
static inline uint32_t XBAR_Trip_Disable(enum XBAR_Trip trip)
{
    uint32_t mask = XBAR_TRIP_ENABLE(trip);
    XBAR_TRIP_ENABLE(trip) = 0;
    return mask;
}
static inline void XBAR_Trip_Enable(enum XBAR_Trip trip, uint32_t mask)
{
    XBAR_TRIP_ENABLE(trip) = mask;
}



//
// CLB XBAR (AuxSig) submodule, only compiled if device supports CLB
//
#if __has_include("support_clb.h")


void XBAR_CLB_SetupSource(enum XBAR_CLB xbar, enum XBAR_CLBSrc source);
void XBAR_CLB_SetupInvert(enum XBAR_CLB xbar, bool invert);

#endif

//
// Output XBAR submodule
//

static inline void XBAR_Output_TriggerLatch(enum XBAR_Output output)
{

}


//
// XBAR Flags
//
static inline bool XBAR_CheckFlag(enum XBAR_Flag flag)
{
    return (XBAR_FLAG(flag) & XBAR_Flag_Mask(flag)) != 0;
}

static inline void XBAR_ClearFlag(enum XBAR_Flag flag)
{
    XBAR_FLAG(flag) = XBAR_Flag_Mask(flag);
}

static inline bool XBAR_CheckCmpssFlag(enum CMPSS_CMP cmp)
{
    return (( (XBAR_FLAG(XBAR_Flag_CMP1H_Trip) >> (CMPSS_CMP_GetCMPSS(cmp) * 2)) & CMPSS_CMP_GetHL(cmp) ) != 0);
}


#endif /* XBAR_H_ */
