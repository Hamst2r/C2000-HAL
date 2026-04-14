#ifndef EPWM_H_
#define EPWM_H_

// #include <stdbool.h>
#include "base.h"
#include "adc.h"
#include "support_epwm.h"


#define EPWM_BASE(epwm)                  (EPWM_START_ADDR + EPWM_INCREMENT_ADDR * (epwm))

#define EPWM_TBCTR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x04 ) )    // Time Base Counter
#define EPWM_TBPRD(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x63 ) )    // Time Base Period
#define EPWM_TBPRDHR(epwm)               ( REG16( EPWM_BASE(epwm) + 0x62 ) )    // Time Base Period (Hi-Res)
#define EPWM_HR_TBPRD(epwm)              ( ATOMIC32( EPWM_BASE(epwm) + 0x62 ) ) // Time Base Period, all 32 bits (Normal + Hi-Res bits)
#define EPWM_TBPHS(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x61 ) )    // Time Base Phase
#define EPWM_TBPHSHR(epwm)               ( REG16( EPWM_BASE(epwm) + 0x60 ) )    // Time Base Phase
#define EPWM_HR_TBPHS(epwm)              ( ATOMIC32( EPWM_BASE(epwm) + 0x60 ) ) // Time Base Phase, all 32 bits (Normal + Hi-Res bits)

#define EPWM_CMPx(epwm,cmp)              ( REG16( EPWM_BASE(epwm) + 0x6B + (cmp) * 2 ) )        // Compare registers, Upper 16 bits (Normal bits), multi-map for CMPA/CMPB/CMPC/CMPD
#define EPWM_CMPxHR(epwm,cmp)            ( REG16( EPWM_BASE(epwm) + 0x6A + (cmp) * 2 ) )        // Compare registers, Lower 16 bits (Hi-Res bits), multi-map for CMPAHR/CMPBHR
#define EPWM_HR_CMPx(epwm,cmp)           ( ATOMIC32( EPWM_BASE(epwm) + 0x6A + (cmp) * 2 ) )     // Compare registers, All 32 bits (Normal + Hi-Res bits), multi-map for [CMPA:CMPAHR]/[CMPB:CMPBHR]

#define EPWM_DBDELAY(epwm,path)          ( REG16( EPWM_BASE(epwm) + 0x51 + (path) * 2 ) )       // Dead-Band Delay, multi-map for DBRED/DBFED
#define EPWM_DBDELAYHR(epwm,path)        ( REG16( EPWM_BASE(epwm) + 0x50 + (path) * 2 ) )       // Dead-Band Delay, multi-map for DBRED/DBFED
#define EPWM_HR_DBDELAY(epwm,path)       ( ATOMIC32( EPWM_BASE(epwm) + 0x50 + (path) * 2 ) )    // Dead-Band Delay, multi-map for DBRED/DBFED

#define EPWM_TZFLG(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x93 ) )   // Trip Zone Flag
#define EPWM_TZCBCFLG(epwm)              ( REG16( EPWM_BASE(epwm) + 0x94 ) )   // Trip Zone Cycle-by-Cycle (CBC) Flag
#define EPWM_TZOSTFLG(epwm)              ( REG16( EPWM_BASE(epwm) + 0x95 ) )   // Trip Zone One-Shot (OST) Flag
#define EPWM_TZCLR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x97 ) )   // Trip Zone Clear
#define EPWM_TZCBCCLR(epwm)              ( REG16( EPWM_BASE(epwm) + 0x98 ) )   // Trip Zone CBC Clear
#define EPWM_TZOSTCLR(epwm)              ( REG16( EPWM_BASE(epwm) + 0x99 ) )   // Trip Zone OST Clear
#define EPWM_TZFRC(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x9B ) )   // Trip Zone Force Trigger

#define EPWM_M_EVT_CMPCD                ( 1U << 3 )


enum EPWM_ClkDiv
{
    EPWM_ClkDiv_1_HS1_DIV1          = ( 0U | ( 0 << 3)),
    EPWM_ClkDiv_2_HS2_DIV1          = ( 1U | ( 0 << 3)),
    EPWM_ClkDiv_4_HS4_DIV1          = ( 2U | ( 0 << 3)),
    EPWM_ClkDiv_6_HS6_DIV1          = ( 3U | ( 0 << 3)),
    EPWM_ClkDiv_8_HS8_DIV1          = ( 4U | ( 0 << 3)),
    EPWM_ClkDiv_10_HS10_DIV1        = ( 5U | ( 0 << 3)),
    EPWM_ClkDiv_12_HS12_DIV1        = ( 6U | ( 0 << 3)),
    EPWM_ClkDiv_14_HS14_DIV1        = ( 7U | ( 0 << 3)),
    EPWM_ClkDiv_16_HS8_DIV2         = ( 4U | ( 1 << 3)),
    EPWM_ClkDiv_20_HS10_DIV2        = ( 5U | ( 1 << 3)),
    EPWM_ClkDiv_24_HS12_DIV2        = ( 6U | ( 1 << 3)),
    EPWM_ClkDiv_28_HS14_DIV2        = ( 7U | ( 1 << 3)),
    EPWM_ClkDiv_32_HS8_DIV4         = ( 4U | ( 2 << 3)),
    EPWM_ClkDiv_40_HS10_DIV4        = ( 5U | ( 2 << 3)),
    EPWM_ClkDiv_48_HS12_DIV4        = ( 6U | ( 2 << 3)),
    EPWM_ClkDiv_56_HS14_DIV4        = ( 7U | ( 2 << 3)),
    EPWM_ClkDiv_64_HS8_DIV8         = ( 4U | ( 3 << 3)),
    EPWM_ClkDiv_80_HS10_DIV8        = ( 5U | ( 3 << 3)),
    EPWM_ClkDiv_96_HS12_DIV8        = ( 6U | ( 3 << 3)),
    EPWM_ClkDiv_112_HS14_DIV8       = ( 7U | ( 3 << 3)),
    EPWM_ClkDiv_128_HS8_DIV16       = ( 4U | ( 4 << 3)),
    EPWM_ClkDiv_160_HS10_DIV16      = ( 5U | ( 4 << 3)),
    EPWM_ClkDiv_192_HS12_DIV16      = ( 6U | ( 4 << 3)),
    EPWM_ClkDiv_224_HS14_DIV16      = ( 7U | ( 4 << 3)),
    EPWM_ClkDiv_256_HS8_DIV32       = ( 4U | ( 5 << 3)),
    EPWM_ClkDiv_320_HS10_DIV32      = ( 5U | ( 5 << 3)), 
    EPWM_ClkDiv_384_HS12_DIV32      = ( 6U | ( 5 << 3)),
    EPWM_ClkDiv_448_HS14_DIV32      = ( 7U | ( 5 << 3)),
    EPWM_ClkDiv_512_HS8_DIV64       = ( 4U | ( 6 << 3)),
    EPWM_ClkDiv_640_HS10_DIV64      = ( 5U | ( 6 << 3)),
    EPWM_ClkDiv_768_HS12_DIV64      = ( 6U | ( 6 << 3)),
    EPWM_ClkDiv_896_HS14_DIV64      = ( 7U | ( 6 << 3)),
    EPWM_ClkDiv_1024_HS8_DIV128     = ( 4U | ( 7 << 3)),
    EPWM_ClkDiv_1280_HS10_DIV128    = ( 5U | ( 7 << 3)),
    EPWM_ClkDiv_1536_HS12_DIV128    = ( 6U | ( 7 << 3)),
    EPWM_ClkDiv_1792_HS14_DIV128    = ( 7U | ( 7 << 3)),
};

enum EPWM_CMP
{
    EPWM_CMP_A = 0,
    EPWM_CMP_B = 1,
    EPWM_CMP_C = 2,
    EPWM_CMP_D = 3
};

enum EPWM_AQ
{
    EPWM_AQ_A = 0,
    EPWM_AQ_B = 1
};

enum EPWM_CounterMode
{
    EPWM_CounterMode_Up     = 0,
    EPWM_CounterMode_Down   = 1,
    EPWM_CounterMode_UpDown = 2,
    EPWM_CounterMode_Freeze = 3
};

enum EPWM_SyncOutSrc
{
    EPWM_SyncOutSrc_SW      = 1U << 0,  // Generate Sync-Out Pulse when SW sync bit in set
    EPWM_SyncOutSrc_Zero    = 1U << 1,  // Generate Sync-Out Pulse when TBCTR = 0
    EPWM_SyncOutSrc_CMPB    = 1U << 2,  // Generate Sync-Out Pulse when TBCTR = CMPB
    EPWM_SyncOutSrc_CMPC    = 1U << 3,  // Generate Sync-Out Pulse when TBCTR = CMPC
    EPWM_SyncOutSrc_CMPD    = 1U << 4,  // Generate Sync-Out Pulse when TBCTR = CMPD
    EPWM_SyncOutSrc_DCAEVT1 = 1U << 5,  // Generate Sync-Out Pulse upon DCAEVT1.sync event
    EPWM_SyncOutSrc_DCBEVT1 = 1U << 6   // Generate Sync-Out Pulse upon DCBEVT1.sync event
};


enum EPWM_PeriodLoad
{
    EPWM_PeriodLoad_Immediate           = (1 << 2) | 0,
    EPWM_PeriodLoad_Shadow_Zero         = (0 << 2) | 0,
    EPWM_PeriodLoad_Shadow_Zero_Sync    = (0 << 2) | 1,
    EPWM_PeriodLoad_Shadow_Sync         = (0 << 2) | 2
};

enum EPWM_Load
{
    EPWM_Load_Disable                   = 0x0013U,
    EPWM_Load_Immediate                 = 0x0000U,

    EPWM_Load_Shadow_Zero               = 0x0010U,
    EPWM_Load_Shadow_Period             = 0x0011U,
    EPWM_Load_Shadow_Zero_Period        = 0x0012U,

    EPWM_Load_Shadow_Zero_Sync          = 0x0110U,
    EPWM_Load_Shadow_Period_Sync        = 0x0111U,
    EPWM_Load_Shadow_Zero_Period_Sync   = 0x0112U,

    EPWM_Load_Shadow_Sync               = 0x0210U
};

enum EPWM_AqEvent
{
    EPWM_AqEvent_Zero       = 0x00,
    EPWM_AqEvent_Period     = 0x02,
    EPWM_AqEvent_Up_CMPA    = 0x04,
    EPWM_AqEvent_Down_CMPA  = 0x06,
    EPWM_AqEvent_Up_CMPB    = 0x08,
    EPWM_AqEvent_Down_CMPB  = 0x0A,
    EPWM_AqEvent_Up_T1      = 0x10,
    EPWM_AqEvent_Down_T1    = 0x12,
    EPWM_AqEvent_Up_T2      = 0x14,
    EPWM_AqEvent_Down_T2    = 0x16
};

enum EPWM_AqAction
{
    EPWM_AqAction_Disable   = 0,
    EPWM_AqAction_Low       = 1,
    EPWM_AqAction_High      = 2,
    EPWM_AqAction_Toggle    = 3
};

enum EPWM_DB
{
    EPWM_DB_RED_Apath,
    EPWM_DB_FED_Bpath
};

enum EPWM_DbLoad
{
    EPWM_DbLoad_Disable             = 0b111,
    EPWM_DbLoad_Immediate           = 0b000,

    EPWM_DbLoad_Shadow_Zero         = 0b100,
    EPWM_DbLoad_Shadow_Period       = 0b101,
    EPWM_DbLoad_Shadow_Zero_Period  = 0b110
};

enum EPWM_DbClk
{
    EPWM_DbClk_FullCycle = 0,
    EPWM_DbClk_HalfCycle = 1
};

enum EPWM_DbMode
{
    EPWM_DbMode_Bypass,
    EPWM_DbMode_ActiveHighCompl,
    EPWM_DbMode_ActiveLowCompl,
    EPWM_DbMode_ActiveHigh,
    EPWM_DbMode_ActiveLow
};

enum EPWM_DbInput
{
    EPWM_DbInput_AQ_A     = 0,  // Both A/B paths use EPWMxA (from AQ) as input
    EPWM_DbInput_Swap     = 1,  // A path uses EPWMxB (from AQ), B path uses EPWMxA (from AQ)
    EPWM_DbInput_A2A_B2B  = 2,  // A path uses EPWMxA (from AQ), B path uses EPWMxB (from AQ)
    EPWM_DbInput_AQ_B     = 3   // Both A/B paths use EPWMxB (from AQ) as input
};

enum EPWM_SwapMode
{
    EPWM_SwapMode_NoSwap    = 0,
    EPWM_SwapMode_BothA     = 1,
    EPWM_SwapMode_BothB     = 2,
    EPWM_SwapMode_FullSwap  = 3
};

// Digital Compare Event Condition
enum EPWM_DcCondition
{
    EPWM_DcCondition_Disabled = 0,  // Disable the DC Event
    EPWM_DcCondition_H0     = 1,    // DCxH = low
    EPWM_DcCondition_H1     = 2,    // DCxH = high
    EPWM_DcCondition_L0     = 3,    // DCxL = low
    EPWM_DcCondition_L1     = 4,    // DCxL = high
    EPWM_DcCondition_L1_H0  = 5     // DCxL = high, DCxH = low
};

enum EPWM_DcCmp
{
    EPWM_DcCmp_AH = 0,
    EPWM_DcCmp_AL = 1,
    EPWM_DcCmp_BH = 2,
    EPWM_DcCmp_BL = 3
};

enum EPWM_DcEvent
{
    EPWM_DcEvent_A1 = 0,
    EPWM_DcEvent_A2 = 1,
    EPWM_DcEvent_B1 = 2,
    EPWM_DcEvent_B2 = 3
};

enum EPWM_DcEvtSync
{
    EPWM_DcEvtSync_Sync  = 0,   // DC Event Trip is synchronized with EPWMCLK
    EPWM_DcEvtSync_Async = 0    // DC Event Trip is asynchronous
};

enum EPWM_DcEvtLatch
{
    EPWM_DcEvtLatch_Disabled     = 0b000, // DC Event Trip is not latched
    EPWM_DcEvtLatch_Zero         = 0b001, // DC Event Trip is latched, cleared when counter equals zero
    EPWM_DcEvtLatch_Period       = 0b011, // DC Event Trip is latched, cleared when counter equals period
    EPWM_DcEvtLatch_ZeroOrPeriod = 0b101, // DC Event Trip is latched, cleared when counter equals zero or period
};

enum EPWM_TzSig
{
    EPWM_TzSig_CBC_TZ1_Input1   = (1L <<  0),   // Cycle By Cycle from TZ1 (Input XBAR1)
    EPWM_TzSig_CBC_TZ2_Input2   = (1L <<  1),   // Cycle By Cycle from TZ2 (Input XBAR2)
    EPWM_TzSig_CBC_TZ3_Input3   = (1L <<  2),   // Cycle By Cycle from TZ3 (Input XBAR3)
    EPWM_TzSig_CBC_TZ4_eQEPErr  = (1L <<  3),   // Cycle By Cycle from TZ4 (eQEP Error)
    EPWM_TzSig_CBC_TZ5_ClkFail  = (1L <<  4),   // Cycle By Cycle from TZ5 (Clock Fail)
    EPWM_TzSig_CBC_TZ6_EmuStop  = (1L <<  5),   // Cycle By Cycle from TZ6 (Emulation Stop)
    EPWM_TzSig_CBC_DCAEVT2      = (1L <<  6),   // Cycle By Cycle from DCAEVT2
    EPWM_TzSig_CBC_DCBEVT2      = (1L <<  7),   // Cycle By Cycle from DCBEVT2
    EPWM_TzSig_OST_TZ1_Input1   = (1L <<  8),   // One-shot from TZ1 (Input XBAR1)        
    EPWM_TzSig_OST_TZ2_Input2   = (1L <<  9),   // One-shot from TZ2 (Input XBAR2)        
    EPWM_TzSig_OST_TZ3_Input3   = (1L << 10),   // One-shot from TZ3 (Input XBAR3)        
    EPWM_TzSig_OST_TZ4_eQEPErr  = (1L << 11),   // One-shot from TZ4 (eQEP Error)        
    EPWM_TzSig_OST_TZ5_ClkFail  = (1L << 12),   // One-shot from TZ5 (Clock Fail)        
    EPWM_TzSig_OST_TZ6_EmuStop  = (1L << 13),   // One-shot from TZ6 (Emulation Stop)        
    EPWM_TzSig_OST_DCAEVT1      = (1L << 14),   // One-shot from DCAEVT1       
    EPWM_TzSig_OST_DCBEVT1      = (1L << 15),   // One-shot from DCBEVT1       
};

enum EPWM_TzFlag
{
    EPWM_TzFlag_INT     = (1L << 0),   // Trip-Zone Interrupt Flag
    EPWM_TzFlag_CBC     = (1L << 1),   // Cycle By Cycle Flag
    EPWM_TzFlag_OST     = (1L << 2),   // One-Shot Flag
    EPWM_TzFlag_DCAEVT1 = (1L << 3),   // DCAEVT1 Flag
    EPWM_TzFlag_DCAEVT2 = (1L << 4),   // DCAEVT2 Flag
    EPWM_TzFlag_DCBEVT1 = (1L << 5),   // DCBEVT1 Flag
    EPWM_TzFlag_DCBEVT2 = (1L << 6)    // DCBEVT2 Flag
};

enum EPWM_TzOSTFlag
{
    EPWM_TzOSTFlag_TZ1_Input1   = (1L << 0),   // One-shot from TZ1 (Input XBAR1)        
    EPWM_TzOSTFlag_TZ2_Input2   = (1L << 1),   // One-shot from TZ2 (Input XBAR2)        
    EPWM_TzOSTFlag_TZ3_Input3   = (1L << 2),   // One-shot from TZ3 (Input XBAR3)        
    EPWM_TzOSTFlag_TZ4_eQEPErr  = (1L << 3),   // One-shot from TZ4 (eQEP Error)        
    EPWM_TzOSTFlag_TZ5_ClkFail  = (1L << 4),   // One-shot from TZ5 (Clock Fail)        
    EPWM_TzOSTFlag_TZ6_EmuStop  = (1L << 5),   // One-shot from TZ6 (Emulation Stop)        
    EPWM_TzOSTFlag_DCAEVT1      = (1L << 6),   // One-shot from DCAEVT1       
    EPWM_TzOSTFlag_DCBEVT1      = (1L << 7),   // One-shot from DCBEVT1       
};

enum EPWM_TzCBCFlag
{
    EPWM_TzCBCFlag_TZ1_Input1   = (1L << 0),   // Cycle By Cycle from TZ1 (Input XBAR1)
    EPWM_TzCBCFlag_TZ2_Input2   = (1L << 1),   // Cycle By Cycle from TZ2 (Input XBAR2)
    EPWM_TzCBCFlag_TZ3_Input3   = (1L << 2),   // Cycle By Cycle from TZ3 (Input XBAR3)
    EPWM_TzCBCFlag_TZ4_eQEPErr  = (1L << 3),   // Cycle By Cycle from TZ4 (eQEP Error)
    EPWM_TzCBCFlag_TZ5_ClkFail  = (1L << 4),   // Cycle By Cycle from TZ5 (Clock Fail)
    EPWM_TzCBCFlag_TZ6_EmuStop  = (1L << 5),   // Cycle By Cycle from TZ6 (Emulation Stop)
    EPWM_TzCBCFlag_DCAEVT2      = (1L << 6),   // Cycle By Cycle from DCAEVT2
    EPWM_TzCBCFlag_DCBEVT2      = (1L << 7),   // Cycle By Cycle from DCBEVT2
};

enum EPWM_TzEvent
{
    EPWM_TzEvent_TZA     =  0,  // TZ Event (OR of CBC and OST) affect on EPWMxA
    EPWM_TzEvent_TZB     =  2,  // TZ Event (OR of CBC and OST) affect on EPWMxB
    EPWM_TzEvent_DCAEVT1 =  4,  // DCAEVT1.force affect on EPWMxA
    EPWM_TzEvent_DCAEVT2 =  6,  // DCAEVT2.force affect on EPWMxA
    EPWM_TzEvent_DCBEVT1 =  8,  // DCBEVT1.force affect on EPWMxB
    EPWM_TzEvent_DCBEVT2 = 10   // DCBEVT2.force affect on EPWMxB
};

enum EPWM_TzAction
{
    EPWM_TzAction_HiZ   = 0,
    EPWM_TzAction_High  = 1,
    EPWM_TzAction_Low   = 2,
    EPWM_TzAction_None  = 3
};

enum EPWM_TzCbcClear
{
    EPWM_TzCbcClear_Zero        = 0,
    EPWM_TzCbcClear_Period      = 1,
    EPWM_TzCbcClear_ZeroPeriod  = 2
};

enum EPWM_EVT
{
    EPWM_EVT_DCxEVT1    = 0U,
    EPWM_EVT_Zero       = 1U,
    EPWM_EVT_Period     = 2U,
    EPWM_EVT_ZeroPeriod = 3U,
    EPWM_EVT_CMPA_Up    = 4U,
    EPWM_EVT_CMPA_Down  = 5U,
    EPWM_EVT_CMPB_Up    = 6U,
    EPWM_EVT_CMPB_Down  = 7U,
    EPWM_EVT_CMPC_Up    = 4U | EPWM_M_EVT_CMPCD,
    EPWM_EVT_CMPC_Down  = 5U | EPWM_M_EVT_CMPCD,
    EPWM_EVT_CMPD_Up    = 6U | EPWM_M_EVT_CMPCD,
    EPWM_EVT_CMPD_Down  = 7U | EPWM_M_EVT_CMPCD
};

enum EPWM_SOC
{
    EPWM_SOC_A = 0,
    EPWM_SOC_B = 1
};

enum EPWM_HR_Mode
{
    EPWM_HR_Mode_Disabled,
    EPWM_HR_Mode_Duty,
    EPWM_HR_Mode_Phase,
    EPWM_HR_Mode_Freq,
    EPWM_HR_Mode_Duty_Phase,
    EPWM_HR_Mode_Duty_Phase_Freq,
};

void EPWM_StopGlobalClk(void);
void EPWM_StartGlobalClk(void);
void EPWM_SetupTripOST(enum EPWM module);

void EPWM_SetupDcCombinationInput(enum EPWM module, enum EPWM_DcCmp cmp, enum EPWM_DcSrc src, bool enable);
void EPWM_SetupDcEventSource(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcSrc src);
void EPWM_Advanced_SetupDcInputSource(enum EPWM module, enum EPWM_DcCmp cmp, enum EPWM_DcSrc src);
void EPWM_Advanced_SetupDcEventCondition(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcCondition condition);
void EPWM_SetupDcEventFilterEnable(enum EPWM module, enum EPWM_DcEvent event, bool enable);
void EPWM_SetupDcEventTripMode(enum EPWM module, enum EPWM_DcEvent event, enum EPWM_DcEvtSync sync, enum EPWM_DcEvtLatch latch);
void EPWM_SetTzCbcClearing(enum EPWM module, enum EPWM_TzCbcClear clear);


void EPWM_EnableTripZoneSignal(enum EPWM module, enum EPWM_TzSig tzSignal);
void EPWM_DisableTripZoneSignal(enum EPWM module, enum EPWM_TzSig tzSignal);
void EPWM_SetupTripZoneAction(enum EPWM module, enum EPWM_TzEvent event, enum EPWM_TzAction action);

void EPWM_SetupClkDiv(enum EPWM module, enum EPWM_ClkDiv clkdiv);

enum ADC_Trig EPWM_SetupBasicAdcTimer(enum EPWM module, float freq);
float EPWM_SetupUp_ZeroAB(enum EPWM module, float freq, float dutyA, float dutyB, float phase, float dbNs, bool syncToPWM1);
float EPWM_SetupUpDown_Complementary(enum EPWM module, float freq, float duty, float phase, float dbNs, bool syncToPWM1);
//float EPWM_HR_SetupUpDownComplementary(enum EPWM module, float freq, float duty, float phase, float dbNs, enum EPWM_HR_Mode hiResMode, bool syncToPWM1);

enum ADC_Trig EPWM_SetupAdcTrigger(enum EPWM module, enum EPWM_SOC soc, enum EPWM_EVT event, uint16_t prescaler);
// void EPWM_SetupInterrupt(enum EPWM module, enum EPWM_EVT event, uint16_t prescaler);

void EPWM_SetFreq(enum EPWM module, float freq);
void EPWM_SetDuty(enum EPWM module, float duty);
void EPWM_SetPhase(enum EPWM module, float phase);
void EPWM_SetDeadband(enum EPWM module, float dbNs_a2b, float dbNs_b2a);

static inline void EPWM_SetTimeBaseCounter(enum EPWM module, uint16_t value)
{
    EPWM_TBCTR(module) = value;
}
static inline void EPWM_SetTimeBasePeriod(enum EPWM module, uint16_t value)
{
    EPWM_TBPRD(module) = value;
}
static inline void EPWM_SetTimeBasePhase(enum EPWM module, uint16_t value)
{
    EPWM_TBPHS(module) = value;
}
static inline void EPWM_SetCompare(enum EPWM module, enum EPWM_CMP cmp, uint16_t value)
{
    EPWM_CMPx(module, cmp) = value;
}

// static inline void EPWM_HR_SetTimeBaseCounter(enum EPWM module, uint32_t value)
// {
//     EPWM_HR_TBCTR(module) = value;
// }
// static inline void EPWM_HR_SetTimeBasePeriod(enum EPWM module, uint32_t value)
// {
//     EPWM_HR_TBPRD(module) = value;
// }
// static inline void EPWM_HR_SetTimeBasePhase(enum EPWM module, uint32_t value)
// {
//     EPWM_HR_TBPHS(module) = value;
// }
// static inline void EPWM_HR_SetCompare(enum EPWM module, enum EPWM_CMP cmp, uint32_t value)
// {
//     EPWM_HR_CMPx(module, cmp) = value;
// }

// static inline void EPWM_SetTimeBaseCounterHR(enum EPWM module, uint16_t value)
// {
//     EPWM_TBCTRHR(module) = value;
// }
// static inline void EPWM_SetTimeBasePeriodHR(enum EPWM module, uint16_t value)
// {
//     EPWM_TBPRDHR(module) = value;
// }
// static inline void EPWM_SetTimeBasePhaseHR(enum EPWM module, uint16_t value)
// {
//     EPWM_TBPHSHR(module) = value;
// }
// static inline void EPWM_SetCompareHR(enum EPWM module, enum EPWM_CMP cmp, uint16_t value)
// {
//     EPWM_CMPxHR(module, cmp) = value;
// }


static inline bool EPWM_GetTripZoneFlag(enum EPWM module, enum EPWM_TzFlag flag)
{
    return ( (EPWM_TZFLG(module) & flag) != 0 );
}

static inline void EPWM_ClearTripZoneFlag(enum EPWM module, enum EPWM_TzFlag flag)
{
    EPWM_TZCLR(module) = flag;
}

static inline void EPWM_ForceTripZoneFlag(enum EPWM module, enum EPWM_TzFlag flag)
{
    EPWM_TZFRC(module) = flag;
}

static inline bool EPWM_GetTripZoneOSTFlag(enum EPWM module, enum EPWM_TzOSTFlag flag)
{
    return ( (EPWM_TZOSTFLG(module) & flag) != 0 );
}

static inline void EPWM_ClearTripZoneOSTFlag(enum EPWM module, enum EPWM_TzOSTFlag flag)
{
    EPWM_TZOSTCLR(module) = flag;
}

static inline bool EPWM_GetTripZoneCBCFlag(enum EPWM module, enum EPWM_TzCBCFlag flag)
{
    return ( (EPWM_TZCBCFLG(module) & flag) != 0 );
}

static inline void EPWM_ClearTripZoneCBCFlag(enum EPWM module, enum EPWM_TzCBCFlag flag)
{
    EPWM_TZCBCCLR(module) = flag;
}

#endif /* EPWM_H_ */
