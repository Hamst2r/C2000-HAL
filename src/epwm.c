
#include "epwm.h"
#include "xbar.h"
#include "clk.h"
#include "sys.h"
#include <math.h>

#define EPWM_HRPHASE_COMPENSATION        ( (float) 1.6 )    //  Hi-Res Phase value for compensating delay during phase sync

#define EPWM_TBCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x00 ) )   // Time Base Control 1
#define EPWM_TBCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x01 ) )   // Time Base Control 2
#define EPWM_TBCTL3(epwm)                ( REG16( EPWM_BASE(epwm) + 0x07 ) )   // Time Base Control 3
#define EPWM_TBSTS(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x05 ) )   // Time Base Status
#define EPWM_SYNCINSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x03 ) )   // SYNC-IN Source Select
#define EPWM_SYNCOUTEN(epwm)             ( REG16( EPWM_BASE(epwm) + 0x06 ) )   // SYNC-OUT Source Enable
#define EPWM_CMPCTL(epwm,cmp)            ( REG16( EPWM_BASE(epwm) + 0x08 + (cmp >> 1)) )   // Counter Compare Control, multi-map for CMPCTL/CMPCTL2


#define EPWM_DBCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x0C ) )   // Dead-Band Control 1
#define EPWM_DBCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x0D ) )   // Dead-Band Control 2

#define EPWM_HRCNFG(epwm)                ( REG16( EPWM_BASE(epwm) + 0x20 ) )   // HRPWM Config 1
#define EPWM_HRCNFG2(epwm)               ( REG16( EPWM_BASE(epwm) + 0x27 ) )   // HRPWM Config 2
#define EPWM_HRPWR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x21 ) )   // HRPWM Power
#define EPWM_HRMSTEP(epwm)               ( REG16( EPWM_BASE(epwm) + 0x26 ) )   // HRPWM MEP Step
#define EPWM_HRPCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0x2D ) )   // Hi-Res Period Control
#define EPWM_TRREM(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x2E ) )   // HRPWM Hi-Res Remainder

#define EPWM_AQCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x10 ) )   // Action Qualifier Control
#define EPWM_AQTSRCSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x11 ) )   // Action Qualifier Trigger Source Select
#define EPWM_AQCTLx(epwm,aq,event)       ( REG16( EPWM_BASE(epwm) + 0x40 + (aq) * 2 + (event) >> 4) )   // Action Qualifier x Control, multi-map for AQCTLA/AQCTLA2/AQCTLB/AQCTLB2

#define EPWM_AQSFRC(epwm)                ( REG16( EPWM_BASE(epwm) + 0x47 ) )   // Action Qualifier S/W Force
#define EPWM_AQCSFRC(epwm)               ( REG16( EPWM_BASE(epwm) + 0x49 ) )   // Action Qualifier S/W Force (continuous)

#define EPWM_PCCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x14 ) )   // PWM Chopper Control

#define EPWM_GLDCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0x34 ) )   // Global Load Control 1
#define EPWM_GLDCTL2(epwm)               ( REG16( EPWM_BASE(epwm) + 0x74 ) )   // Global Load Control 2
#define EPWM_GLDCFG(epwm)                ( REG16( EPWM_BASE(epwm) + 0x35 ) )   // Global Load Config
#define EPWM_EPWMXLINK(epwm)             ( REG32( EPWM_BASE(epwm) + 0x38 ) )   // ePWM Register Link


#define EPWM_TZSEL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x80 ) )   // Trip Zone Select (connect input event to CBC/OST)
#define EPWM_TZCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x84 ) )   // Trip Zone Control 1 (output action w/o considering direction)
#define EPWM_TZCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x85 ) )   // Trip-Zone Control 2 (output action w/ direction)
#define EPWM_TZCTLDCA(epwm)              ( REG16( EPWM_BASE(epwm) + 0x86 ) )   // Trip Zone Control - Digital Compare A (output action w/ direction)
#define EPWM_TZCTLDCB(epwm)              ( REG16( EPWM_BASE(epwm) + 0x87 ) )   // Trip Zone Control - Digital Compare B (output action w/ direction)
#define EPWM_TZEINT(epwm)                ( REG16( EPWM_BASE(epwm) + 0x8D ) )   // Trip Zone Enable Interrupt

#define EPWM_ETSEL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xA4 ) )   // Event Trigger Select
#define EPWM_ETPS(epwm)                  ( REG16( EPWM_BASE(epwm) + 0xA6 ) )   // Event Trigger Pre-Scale
#define EPWM_ETFLG(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xA8 ) )   // Event Trigger Flag
#define EPWM_ETCLR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xAA ) )   // Event Trigger Clear
#define EPWM_ETFRC(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xAC ) )   // Event Trigger Force
#define EPWM_ETINTPS(epwm)               ( REG16( EPWM_BASE(epwm) + 0xAE ) )   // Event Trigger Interrupt Pre-Scale
#define EPWM_ETSOCPS(epwm)               ( REG16( EPWM_BASE(epwm) + 0xB0 ) )   // Event Trigger SOC Pre-Scale
#define EPWM_ETCNTINITCTL(epwm)          ( REG16( EPWM_BASE(epwm) + 0xB2 ) )   // Event Trigger Counter Initialization Control
#define EPWM_ETCNTINIT(epwm)             ( REG16( EPWM_BASE(epwm) + 0xB4 ) )   // Event Trigger Counter Initialization

#define EPWM_DcOrSel(epwm,cmp)           ( REG16( EPWM_BASE(epwm) + 0xD2 + cmp) )   // Digital Compare Trip Select (controls ORed/Combination input), multi-map for DCxxTRIPSEL
#define EPWM_DCTRIPSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0xC0 ) )   // Digital Compare Trip Select (TRIP to DCxH/L)
#define EPWM_TZDCSEL(epwm)               ( REG16( EPWM_BASE(epwm) + 0x82 ) )   // Trip Zone Digital Comparator Select (DCxH/L to DCxEVT1/2)
#define EPWM_DCxCTL(epwm,dcEvt)          ( REG16( EPWM_BASE(epwm) + 0xC3 + EPWM_GetDcModule(dcEvt)) )   // Digital Compare Control, multi-map for DCACTL/DCBCTL
#define EPWM_DCFCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0xC7 ) )   // Digital Compare Filter Control
#define EPWM_DCFOFFSET(epwm)             ( REG16( EPWM_BASE(epwm) + 0xC9 ) )   // Digital Compare Filter Offset
#define EPWM_DCFOFFSETCNT(epwm)          ( REG16( EPWM_BASE(epwm) + 0xCA ) )   // Digital Compare Filter Offset Counter
#define EPWM_DCFWINDOW(epwm)             ( REG16( EPWM_BASE(epwm) + 0xCB ) )   // Digital Compare Filter Window
#define EPWM_DCFWINDOWCNT(epwm)          ( REG16( EPWM_BASE(epwm) + 0xCC ) )   // Digital Compare Filter Window Counter
#define EPWM_BLANKPULSEMIXSEL(epwm)      ( REG16( EPWM_BASE(epwm) + 0xCD ) )   // Blanking Window Trigger Pulse Select
#define EPWM_DCCAPCTL(epwm)              ( REG16( EPWM_BASE(epwm) + 0xC8 ) )   // Digital Compare TB Counter Capture Control
#define EPWM_DCCAP(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xCF ) )   // Digital Compare TB Counter Capture

#define EPWM_EPWMLOCK(epwm)              ( ATOMIC32( EPWM_BASE(epwm) + 0xFA ) )   // EPWM Lock
#define EPWM_VCAPCTL(epwm)               ( REG16( EPWM_BASE(epwm) + 0x18 ) )   // Valley Capture Control
#define EPWM_VCNTCFG(epwm)               ( REG16( EPWM_BASE(epwm) + 0x19 ) )   // Valley Counter Config
#define EPWM_SWVDELVAL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x77 ) )   // Software Valley Mode Delay
#define EPWM_HWVDELVAL(epwm)             ( REG16( EPWM_BASE(epwm) + 0xFD ) )   // Hardware Valley Mode Delay
#define EPWM_VCNTVAL(epwm)               ( REG16( EPWM_BASE(epwm) + 0xFE ) )   // Hardware Valley Counter


#define EPWM_S_TBCTL_FREESOFT(mode)         ( (mode) << 14 )
#define EPWM_S_TBCTL_PHSDIR(dir)            ( (dir)  << 13 )
#define EPWM_S_TBCTL_CLKDIV(div)            ( (div)  <<  7 )
#define EPWM_S_TBCTL_PRDLD(load)            ( (load) <<  3 )
#define EPWM_S_TBCTL_PHSEN(en)              ( (en)   <<  2 )
#define EPWM_S_TBCTL_CTRMODE(mode)          ( (mode) <<  0 )
#define EPWM_S_TBCTL2_PRDLDSYNC(load)       ( (load) << 14 )
#define EPWM_S_CMPCTL(cmp,load)             ( (load) << ((cmp&1U) * 2))
#define EPWM_S_AQCTL(aq,load)               ( (load) << ((aq) * 2))
#define EPWM_S_DBCTL_OUTSWAP(swap)          ( (swap) << 12 )
#define EPWM_S_DBCTL_INMODE(mode)           ( (mode) <<  4 )
#define EPWM_S_DCxCTL_SYNCSEL(dcEvt,sync)   ( sync  << (EPWM_GetDcEvtNum(dcEvt)*8 + 1) )
#define EPWM_S_DCxCTL_LATCH(dcEvt,latch)    ( latch << (EPWM_GetDcEvtNum(dcEvt)*8 + 4) )
#define EPWM_S_TZCLR_CBC_PULSE(clear)       ( (clear) << 14 )
#define EPWM_S_ETSOCPS_PRD2(soc, prd)       ( (prd) <<  ((soc)*8) )
#define EPWM_S_ETSEL_SOCSEL(soc,sel)        ( (sel) <<  ((soc)*4 + 8U)  )

#define EPWM_M_TBCTL_FREESOFT            ( 0x03U << 14 )
#define EPWM_M_TBCTL_PHSDIR              ( 0x01U << 13 )
#define EPWM_M_TBCTL_CLKDIV              ( 0x3FU <<  7 )
#define EPWM_M_TBCTL_PRDLD               ( 0x01U <<  3 )
#define EPWM_M_TBCTL_PHSEN               ( 0x01U <<  2 )
#define EPWM_M_TBCTL_CTRMODE             ( 0x03U <<  0 )
#define EPWM_M_TBCTL2_PRDLDSYNC          ( 0x03U << 14 )
#define EPWM_M_AQCTLx(event)             ( 0x03U << (event) )
#define EPWM_M_CMPCTL(cmp)               ( 0x0C13U << ((cmp&1U) * 2))
#define EPWM_M_AQCTL(aq)                 ( 0x0313U << ((aq) * 2))
#define EPWM_M_DBCTL_HALFCYCLE           ( 0x01U << 15 )
#define EPWM_M_DBCTL_OUTSWAP             ( 0x03U << 12 )
#define EPWM_M_DBCTL_LOADMODE(path)      ( (path) ? 0x0B00 : 0x04C0 )
#define EPWM_M_DBCTL_INMODE              ( 0x03U <<  4 )
#define EPWM_M_DBCTL_OPMODE              ( 0x0FU <<  0 )    // Combination of POLSEL and OUTMODE
#define EPWM_M_TZDCSEL(evt)              ( 0x07U << ((evt)*3) )
#define EPWM_M_DCTRIPSEL(evt)            ( 0x0FU << ((evt)*4) )
#define EPWM_M_DCxCTL_SRCSEL(dcEvt)      ( 0x01U << (EPWM_GetDcEvtNum(dcEvt)*8) )
#define EPWM_M_DCxCTL_LATSYNC(dcEvt)     ( 0x72U << (EPWM_GetDcEvtNum(dcEvt)*8) )
#define EPWM_M_TZCTL(tzEvt)              ( 0x03U <<  (tzEvt)  )
#define EPWM_M_ETPS_SOCPSSEL             ( 0x01U <<  5  )
#define EPWM_M_ETSOCPS_PRD2(soc)         ( 0x0FU <<  ((soc)*8) )
#define EPWM_M_ETSEL_SOCEN(soc)          ( 0x01U <<  ((soc)*4 + 11U) )
#define EPWM_M_ETSEL_SOCSEL(soc)         ( 0x07U <<  ((soc)*4 + 8U)  )
#define EPWM_M_ETSEL_SOCSELCMP(soc)      ( 0x01U <<  ((soc)*1 + 4U)  )

#define EPWM_IsHiRes(epwm)               ( ( ((epwm) >> 6) & 1U ) == 1 )
#define EPWM_GetDcModule(dcEvt)          ( (dcEvt) >> 1 )
#define EPWM_GetDcEvtNum(dcEvt)          ( (dcEvt) & 1U )

enum EPWM_EmuMode
{
   //! Stop after next Time Base counter increment or decrement
   EPWM_EmuMode_StopAfterNextTb = 0,
   //! Stop when counter completes whole cycle
   EPWM_EmuMode_StopAfterCycle = 1,
   //! Free run
   EPWM_EmuMode_FreeRun = 2
};

struct EPWM_Status
{
    enum EPWM_HR_Mode hiResMode;
    bool isUpDown;
    bool isClkScaled;
    float tbFreqLogicalMHz;
    // float tbFreqPhysical;
    // float pwmFreqLogical;
    // float pwmFreqPhysical;
};

struct EPWM_Status EPWM_status[EPWM_COUNT];

void EPWM_SetEmulationMode(enum EPWM module, enum EPWM_EmuMode mode)
{
    // Write to FREE_SOFT bits
    EPWM_TBCTL(module) =  EPWM_TBCTL(module)
                       & ~EPWM_M_TBCTL_FREESOFT
                       |  EPWM_S_TBCTL_FREESOFT(mode);
}

void EPWM_StopGlobalClk(void)
{
    SYS_DisablePeripheral(SYS_ClkGate_TBCLKSYNC);
}

void EPWM_StartGlobalClk(void)
{
    SYS_EnablePeripheral(SYS_ClkGate_TBCLKSYNC);
}


void EPWM_SetupDcCombinationInput(enum EPWM module, enum EPWM_DcCmp cmp, enum EPWM_DcSrc src, bool enable)
{
    if(enable)
    {
        EPWM_DcOrSel(module, cmp) |= 1U << src;
    } else {
        EPWM_DcOrSel(module, cmp) &= ~(1U << src);
    }
}

void EPWM_Advanced_SetupDcInputSource(enum EPWM module, enum EPWM_DcCmp cmp, enum EPWM_DcSrc src)
{
    EPWM_DCTRIPSEL(module) = EPWM_DCTRIPSEL(module)
                           & ~EPWM_M_DCTRIPSEL(cmp)
                           | src;
}

void EPWM_Advanced_SetupDcEventCondition(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcCondition condition)
{
    EPWM_TZDCSEL(module) = EPWM_TZDCSEL(module)
                         & ~EPWM_M_TZDCSEL(event)
                         | condition;
}

//
//  Configures TRIP signals that can trigger a DC Event
//  To simplify design, Active High DCxH/L will always connect to DCxEVT1/2 respectively
//
void EPWM_SetupDcEventSource(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcSrc src)
{
    // DCxH -> DCxEVT1, DCxL -> DCxEVT2, active high
    enum EPWM_DcCondition condition = (event & 0x01U) ? EPWM_DcCondition_L1 : EPWM_DcCondition_H1;
    // enum EPWM_DcCondition condition = (enum EPWM_DcCondition) (((event & 0x01U) << 1) + 2);
    
    EPWM_Advanced_SetupDcInputSource(module, (enum EPWM_DcCmp) event, src);
    EPWM_Advanced_SetupDcEventCondition(module, event, condition);
}

void EPWM_SetupDcEventFilterEnable(enum EPWM module, enum EPWM_DcEvent event, bool enable)
{
    if(enable)
    {
        EPWM_DCxCTL(module, EPWM_GetDcModule(event)) |=  EPWM_M_DCxCTL_SRCSEL(EPWM_GetDcEvtNum(event));
    } else {
        EPWM_DCxCTL(module, EPWM_GetDcModule(event)) &= ~EPWM_M_DCxCTL_SRCSEL(EPWM_GetDcEvtNum(event));
    }
}

void EPWM_SetupDcEventTripMode(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcEvtSync sync, enum EPWM_DcEvtLatch latch)
{
    EPWM_DCxCTL(module,event) =  EPWM_DCxCTL(module, event)
                              & ~EPWM_M_DCxCTL_LATSYNC(event)
                              | (EPWM_S_DCxCTL_LATCH(event, latch) | EPWM_S_DCxCTL_SYNCSEL(event, sync));
}


void EPWM_EnableTripZoneSignal(enum EPWM module, enum EPWM_TzSig tzSignal)
{
    EPWM_TZSEL(module) |= tzSignal;
}

void EPWM_DisableTripZoneSignal(enum EPWM module, enum EPWM_TzSig tzSignal)
{
    EPWM_TZSEL(module) &= ~tzSignal;
}

void EPWM_SetTzCbcClearing(enum EPWM module, enum EPWM_TzCbcClear clear)
{
    EPWM_TZCLR(module) = EPWM_S_TZCLR_CBC_PULSE(clear);
}

void EPWM_SetupTripZoneAction(enum EPWM module, enum EPWM_TzEvent event, enum EPWM_TzAction action)
{
    EPWM_TZCTL(module) = EPWM_TZCTL(module)
                       & ~(EPWM_M_TZCTL(event))
                       | (action << event);
}

void EPWM_SetupTripOST(enum EPWM module)
{
    // Input XBAR 1 (HW-SD), Input XBAR 6 (FW OST), and ePWM XBAR4 (CMPSS-OST) are routed to DCAEVT1 (via DCAH)
    EPWM_SetupDcCombinationInput(module, EPWM_DcCmp_AH, EPWM_DcSrc_01_Input1, true);
    EPWM_SetupDcCombinationInput(module, EPWM_DcCmp_AH, EPWM_DcSrc_06_Input6, true);
    EPWM_SetupDcCombinationInput(module, EPWM_DcCmp_AH, EPWM_DcSrc_04_XBAR4_OST, true);
    EPWM_SetupDcEventSource(module, EPWM_DcEvent_A1, EPWM_DcSrc_Combination);

    // Configure DCAEVT1 signal path: no filter, async, no latch
    EPWM_SetupDcEventFilterEnable(module, EPWM_DcEvent_A1, false);
    EPWM_SetupDcEventTripMode(module, EPWM_DcEvent_A1, EPWM_DcEvtSync_Async, EPWM_DcEvtLatch_Disabled);

    // DCAEVT1 is routed to Trip-Zone One-Shot
    EPWM_EnableTripZoneSignal(module, EPWM_TzSig_OST_DCAEVT1);

    // Clear flags for DCAEVT1
    EPWM_ClearTripZoneFlag(module, EPWM_TzFlag_DCAEVT1);
    EPWM_ClearTripZoneOSTFlag(module, EPWM_TzOSTFlag_DCAEVT1);

    // Trip-Zone pulls both outputs low
    EPWM_SetupTripZoneAction(module, EPWM_TzEvent_TZA, EPWM_TzAction_Low);
    EPWM_SetupTripZoneAction(module, EPWM_TzEvent_TZB, EPWM_TzAction_Low);
}



void EPWM_SetupClkDiv(enum EPWM module, enum EPWM_ClkDiv clkdiv)
{
    // Calculate total divider of selected config
    uint16_t hspClkDiv, clkDivider, totalDivider;
    hspClkDiv = (clkdiv & 0x07) * 2;
    hspClkDiv += (hspClkDiv == 0);
    clkDivider = 1;
    for(uint16_t i=0;i<(clkdiv >> 3);i++)
    {
        clkDivider *= 2;
    }
    totalDivider = hspClkDiv * clkDivider;
    // Update time base frequency info
    EPWM_status[module].tbFreqLogicalMHz = CLK_status.sysClkLogicalMHz / totalDivider;
    EPWM_status[module].isClkScaled = (totalDivider != 1);

    EPWM_TBCTL(module) =  EPWM_TBCTL(module)
                       & ~EPWM_M_TBCTL_CLKDIV
                       |  EPWM_S_TBCTL_CLKDIV(clkdiv);
}

void EPWM_SetupPeriodLoadMode(enum EPWM module, enum EPWM_PeriodLoad loadMode)
{
    if(loadMode == EPWM_PeriodLoad_Immediate)
    {
        EPWM_TBCTL(module) |= EPWM_M_TBCTL_PRDLD;
    } else {
        EPWM_TBCTL(module) &= ~EPWM_M_TBCTL_PRDLD;
        EPWM_TBCTL2(module) = EPWM_TBCTL2(module) & ~EPWM_M_TBCTL2_PRDLDSYNC
                            | (loadMode & 0x03U);
    }
}

void EPWM_SetupCompareLoadMode(enum EPWM module, enum EPWM_CMP cmp, enum EPWM_Load loadMode)
{
    // CMPCTL's bit fields are left-shifted by 2 bits for upper byte
    uint16_t value = (loadMode & 0x00FF) | ((loadMode & 0xFF00) << 2);
    EPWM_CMPCTL(module, cmp) = EPWM_CMPCTL(module, cmp) & ~EPWM_M_CMPCTL(cmp)
                             | EPWM_S_CMPCTL(cmp, value);
}

void EPWM_SetupActionQualifierLoadMode(enum EPWM module, enum EPWM_AQ aq, enum EPWM_Load loadMode)
{
    EPWM_AQCTL(module) = EPWM_AQCTL(module) & ~EPWM_M_AQCTL(aq)
                       | EPWM_S_AQCTL(aq, loadMode);
}

void EPWM_ClearActionQualifier(enum EPWM module, enum EPWM_AQ aq)
{
    // EPWM_AQCTLx1 = 0
    EPWM_AQCTLx(module, aq, EPWM_AqEvent_Zero) = 0;
    // EPWM_AQCTLx2 = 0
    EPWM_AQCTLx(module, aq, EPWM_AqEvent_Up_T1) = 0;
}
void EPWM_SetupActionQualifier(enum EPWM module, enum EPWM_AQ aq, enum EPWM_AqEvent event, enum EPWM_AqAction action)
{
    EPWM_AQCTLx(module, aq, event) = EPWM_AQCTLx(module, aq, event) & ~EPWM_M_AQCTLx(event)
                                   | (action << event);
}

void EPWM_SetupCounterMode(enum EPWM module, enum EPWM_CounterMode mode)
{
    EPWM_TBCTL(module) = EPWM_TBCTL(module) & ~EPWM_M_TBCTL_CTRMODE
                       | EPWM_S_TBCTL_CTRMODE(mode);
}

void EPWM_SetupPhaseShiftEnable(enum EPWM module, bool enable)
{
    EPWM_TBCTL(module) = EPWM_TBCTL(module) & ~EPWM_M_TBCTL_PHSEN
                       | EPWM_S_TBCTL_PHSEN(enable);
}

void EPWM_SetupSyncOut(enum EPWM module, enum EPWM_SyncOutSrc source)
{
    EPWM_SYNCOUTEN(module) = source;
}

void EPWM_SetupSyncIn(enum EPWM module, enum EPWM_SyncInSrc source)
{
    EPWM_SYNCINSEL(module) = source;
}

void EPWM_SetupDeadBandClock(enum EPWM module, enum EPWM_DbClk clkMode)
{
    if(clkMode == EPWM_DbClk_FullCycle)
    {
        EPWM_DBCTL(module) &= ~EPWM_M_DBCTL_HALFCYCLE;
    } else {
        EPWM_DBCTL(module) |=  EPWM_M_DBCTL_HALFCYCLE;
    }
}

void EPWM_SetupDeadBandInput(enum EPWM module, enum EPWM_DbInput input)
{
    EPWM_DBCTL(module) = EPWM_DBCTL(module) & ~EPWM_M_DBCTL_INMODE
                       | EPWM_S_DBCTL_INMODE(input);
}

void EPWM_SetupDeadBandLoadMode(enum EPWM module, enum EPWM_DB path, enum EPWM_DbLoad loadMode)
{
    uint16_t value;
    if(path == EPWM_DB_FED_Bpath)
    {
        value = (((loadMode & 0b100) << 1) | (loadMode & 0b011)) << 8;
    } else {
        value = (((loadMode & 0b100) << 2) | (loadMode & 0b011)) << 6;
    }
    EPWM_DBCTL(module) = EPWM_DBCTL(module) & ~EPWM_M_DBCTL_LOADMODE(path)
                       | value;
}

void EPWM_SetupDeadBandControlLoadMode(enum EPWM module, enum EPWM_DbLoad loadMode)
{
    EPWM_DBCTL2(module) = loadMode;
}

void EPWM_SetupDeadBandMode(enum EPWM module, enum EPWM_DbMode mode)
{
    EPWM_DBCTL(module) = EPWM_DBCTL(module) & ~EPWM_M_DBCTL_OPMODE
                       | mode;
}

void EPWM_SetupOutputSwap(enum EPWM module, enum EPWM_SwapMode mode)
{
    EPWM_DBCTL(module) = EPWM_DBCTL(module) & ~EPWM_M_DBCTL_OUTSWAP
                       | EPWM_S_DBCTL_OUTSWAP(mode);
}

enum ADC_Trig EPWM_SetupBasicAdcTimer(enum EPWM module, float freqHz)
{
    uint32_t period32;
    uint16_t period;

    if (EPWM_status[module].tbFreqLogicalMHz == 0.0)
    {
        EPWM_SetupClkDiv(module, EPWM_ClkDiv_2_HS2_DIV1);
    }

    period32 = roundf(EPWM_status[module].tbFreqLogicalMHz * 1E6 / freqHz) - 1;

    if( (period32 & 0xFFFF0000) != 0 )
    {
        // Frequency is too low. Please configure clock divider using EPWM_SetupClkDiv()
        ESTOP0;
        while(1);
    }

    period = (uint16_t)(period32 & 0x0000FFFF);


    EPWM_status[module].hiResMode = EPWM_HR_Mode_Disabled;

    EPWM_status[module].isUpDown = false;
    EPWM_SetEmulationMode(module, EPWM_EmuMode_FreeRun);

    // Configure period register and reset counter
    EPWM_SetupPeriodLoadMode(module, EPWM_PeriodLoad_Shadow_Zero_Sync);
    EPWM_SetupCounterMode(module, EPWM_CounterMode_Up);
    EPWM_SetTimeBaseCounter(module, 0);
    EPWM_SetTimeBasePeriod(module, period);

    return EPWM_SetupAdcTrigger(module, EPWM_SOC_A, EPWM_EVT_Period, 1);
}

// void EPWM_SetupUp_ZeroAB(enum EPWM module, float freq, float dutyA, float dutyB, float phase, float dbNs, bool syncToPWM1)
// {
//     EPWM_SetupTripOST(module);

//     float tbFreq;
// }

float EPWM_SetupUpDown_Complementary(enum EPWM module, float freq, float duty, float phase, float dbNs, bool syncToPWM1)
{
    uint32_t period32;
    uint16_t period;

    EPWM_SetupTripOST(module);

    if (EPWM_status[module].tbFreqLogicalMHz == 0.0)
    {
        EPWM_SetupClkDiv(module, EPWM_ClkDiv_1_HS1_DIV1);
    }

    period32 = roundf(EPWM_status[module].tbFreqLogicalMHz * 1E6 / freq / 2.0);

    if( (period32 & 0xFFFF0000) != 0 )
    {
        // Frequency is too low. Please configure clock divider using EPWM_SetupClkDiv()
        ESTOP0;
        while(1);
    }

    period = (uint16_t)(period32 & 0x0000FFFF);

    EPWM_status[module].isUpDown = true;
    EPWM_status[module].hiResMode = EPWM_HR_Mode_Disabled;
    EPWM_SetEmulationMode(module, EPWM_EmuMode_FreeRun);

    // Configure register load mode    
    EPWM_SetupPeriodLoadMode(module, EPWM_PeriodLoad_Shadow_Zero_Sync);
    EPWM_SetupCompareLoadMode(module, EPWM_CMP_A, EPWM_Load_Shadow_Zero_Period);
    EPWM_SetupActionQualifierLoadMode(module, EPWM_AQ_A, EPWM_Load_Shadow_Zero_Period_Sync);
    EPWM_SetupActionQualifierLoadMode(module, EPWM_AQ_B, EPWM_Load_Shadow_Zero_Period_Sync);
    EPWM_SetupDeadBandControlLoadMode(module, EPWM_DbLoad_Immediate);
    EPWM_SetupDeadBandLoadMode(module, EPWM_DB_RED_Apath, EPWM_DbLoad_Shadow_Zero_Period);
    EPWM_SetupDeadBandLoadMode(module, EPWM_DB_FED_Bpath, EPWM_DbLoad_Shadow_Zero_Period);

    // Configure frequency and duty cycle
    EPWM_SetupCounterMode(module, EPWM_CounterMode_UpDown);
    EPWM_SetTimeBaseCounter(module, 0);
    EPWM_SetTimeBasePeriod(module, period);
    EPWM_SetCompare(module, EPWM_CMP_A, period * duty + 0.5);

    // Configure Action Qualifier
    EPWM_ClearActionQualifier(module, EPWM_AQ_A);
    EPWM_SetupActionQualifier(module, EPWM_AQ_A, EPWM_AqEvent_Down_CMPA, EPWM_AqAction_High);
    EPWM_SetupActionQualifier(module, EPWM_AQ_A, EPWM_AqEvent_Up_CMPA  , EPWM_AqAction_Low);

    // Configure dead-band
    EPWM_SetupDeadBandClock(module, EPWM_DbClk_HalfCycle);
    EPWM_SetupDeadBandMode(module, EPWM_DbMode_ActiveHighCompl);
    EPWM_SetupDeadBandInput(module, EPWM_DbInput_AQ_A);
    EPWM_SetDeadband(module, dbNs, dbNs);

    // Configure sync and phase
    // If sync source is ePWM 1, we don't need to set the phase shift in ePWM 1 (since it does not have sync-in)
    if(syncToPWM1)
    {
        if(module == EPWM_1)
        {
            EPWM_SetupPhaseShiftEnable(EPWM_1, false);
            EPWM_SetupSyncIn(module, EPWM_SyncInSrc_Disable);
            EPWM_SetupSyncOut(module, (enum EPWM_SyncOutSrc)(EPWM_SyncOutSrc_Zero|EPWM_SyncOutSrc_SW));
        } else {
            EPWM_SetPhase(module, phase);
            EPWM_SetupPhaseShiftEnable(module, true);
            EPWM_SetupSyncIn(module, EPWM_SyncInSrc_ePWM1);
        }
    } else {
        EPWM_SetPhase(module, phase);
        EPWM_SetupPhaseShiftEnable(module, true);
    }

    return (EPWM_status[module].tbFreqLogicalMHz * 1E6 / period / 2.0);
}

// void EPWM_HR_SetupUpDownComplementary(enum EPWM module, float freq, float duty, float phase, float dbNs, enum EPWM_HR_Mode hiResMode, bool syncToPWM1)
// {
//     EPWM_SetupTripOST(module);
// }

enum ADC_Trig EPWM_SetupAdcTrigger(enum EPWM module, enum EPWM_SOC soc, enum EPWM_EVT event, uint16_t prescaler)
{
    if(prescaler > 15)
    {
        // Prescaler should be no greater than 15. Stopping execution
        ESTOP0;
        while(1);
    }
    else if(prescaler == 0)
    {
        EPWM_ETSEL(module) &= ~EPWM_M_ETSEL_SOCEN(soc);
    }
    else
    {
        EPWM_ETSEL(module) = EPWM_ETSEL(module)
                           & ~EPWM_M_ETSEL_SOCSEL(soc)
                           & ~EPWM_M_ETSEL_SOCSELCMP(soc)
                           | EPWM_M_ETSEL_SOCEN(soc)
                           | EPWM_S_ETSEL_SOCSEL(soc,event & 0x07U)
                           | (EPWM_M_ETSEL_SOCSELCMP(soc) * (event >> 3U));


        // Enabled the improved 15-step prescaler
        EPWM_ETPS(module) |= EPWM_M_ETPS_SOCPSSEL;
        EPWM_ETSOCPS(module) =  EPWM_ETSOCPS(module)
                             & ~EPWM_M_ETSOCPS_PRD2(soc)
                             |  EPWM_S_ETSOCPS_PRD2(soc,prescaler);
    }

    return ADC_Trig_EPWM(module & (0x1F), soc);
}

// void EPWM_SetupInterrupt(enum EPWM module, enum EPWM_SOC soc, enum EPWM_EVT event, uint16_t prescaler);



// void EPWM_SetFreq(enum EPWM module, float freq);
// void EPWM_SetDuty(enum EPWM module, float duty);
void EPWM_SetPhase(enum EPWM module, float phase)
{   
    int32_t phaseValue;
    uint16_t period = EPWM_TBPRD(module);

    // For Up-Down, shift by 100% TBPRD is only 50% shift. 
    // There is delay in ePWM sync. Compensation values are: (TBCLK = EPWMCLK): 2 x EPWMCLK; (TBCLK < EPWMCLK): 1 x TBCLK
    if(EPWM_status[module].isUpDown)
    {
        phaseValue = roundf(phase * period * 2 + (EPWM_status[module].isClkScaled ? 1.0 : 1.6));
        
        while(phaseValue >= period - 1)
        {
            phaseValue -= period * 2;
        }
        while(phaseValue <= -period)
        {
            phaseValue += period * 2;
        }

        if(phaseValue >= 0)
        {
            EPWM_TBCTL(module) |= EPWM_M_TBCTL_PHSDIR;
            EPWM_SetTimeBasePhase(module, phaseValue);
        } else {
            EPWM_TBCTL(module) &= ~EPWM_M_TBCTL_PHSDIR;
            EPWM_SetTimeBasePhase(module, -phaseValue);
        }
    }

}


void EPWM_SetDeadband(enum EPWM module, float dbNs_a2b, float dbNs_b2a)
{
    EPWM_DBDELAY(module, EPWM_DB_RED_Apath) = dbNs_b2a * EPWM_status[module].tbFreqLogicalMHz * ( 2.0f / 1000.0 );
    EPWM_DBDELAY(module, EPWM_DB_FED_Bpath) = dbNs_a2b * EPWM_status[module].tbFreqLogicalMHz * ( 2.0f / 1000.0 );
}
