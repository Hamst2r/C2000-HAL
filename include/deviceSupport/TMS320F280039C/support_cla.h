#ifndef SUPPORT_CLA_H_
#define SUPPORT_CLA_H_

#include "mem.h"

#define CLA_BASE        (0x00001400UL)
#define CLA_SRC_BASE    (0x00007980UL)  // In SysCtrl Chapter

enum CLA_Trig
{
    CLA_Trig_SW             =   0,
    CLA_Trig_ADCAINT1       =   1,
    CLA_Trig_ADCAINT2       =   2,
    CLA_Trig_ADCAINT3       =   3,
    CLA_Trig_ADCAINT4       =   4,
    CLA_Trig_ADCA_EVT_INT   =   5,
    CLA_Trig_ADCBINT1       =   6,
    CLA_Trig_ADCBINT2       =   7,
    CLA_Trig_ADCBINT3       =   8,
    CLA_Trig_ADCBINT4       =   9,
    CLA_Trig_ADCB_EVT_INT   =  10,
    CLA_Trig_ADCCINT1       =  11,
    CLA_Trig_ADCCINT2       =  12,
    CLA_Trig_ADCCINT3       =  13,
    CLA_Trig_ADCCINT4       =  14,
    CLA_Trig_ADCC_EVT_INT   =  15,
    CLA_Trig_XINT1          =  29,
    CLA_Trig_XINT2          =  30,
    CLA_Trig_XINT3          =  31,
    CLA_Trig_XINT4          =  32,
    CLA_Trig_XINT5          =  33,
    CLA_Trig_EPWM1_INT      =  36,
    CLA_Trig_EPWM2_INT      =  37,
    CLA_Trig_EPWM3_INT      =  38,
    CLA_Trig_EPWM4_INT      =  39,
    CLA_Trig_EPWM5_INT      =  40,
    CLA_Trig_EPWM6_INT      =  41,
    CLA_Trig_EPWM7_INT      =  42,
    CLA_Trig_EPWM8_INT      =  43,
    CLA_Trig_MCANA_FEVT0    =  52,
    CLA_Trig_MCANA_FEVT1    =  53,
    CLA_Trig_MCANA_FEVT2    =  54,
    CLA_Trig_CPU_TINT0      =  68,
    CLA_Trig_CPU_TINT1      =  69,
    CLA_Trig_CPU_TINT2      =  70,
    CLA_Trig_ECAP1_INT      =  75,
    CLA_Trig_ECAP2_INT      =  76,
    CLA_Trig_ECAP3_INT      =  77,
    CLA_Trig_EQEP1_INT      =  83,
    CLA_Trig_EQEP2_INT      =  84,
    CLA_Trig_ECAP3_INT2     =  89,
    CLA_Trig_SD1_ERRINT     =  95,
    CLA_Trig_SD1FLT1_DRINT  =  96,
    CLA_Trig_SD1FLT2_DRINT  =  97,
    CLA_Trig_SD1FLT3_DRINT  =  98,
    CLA_Trig_SD1FLT4_DRINT  =  99,
    CLA_Trig_SD2_ERRINT     = 100,
    CLA_Trig_SD2FLT1_DRINT  = 101,
    CLA_Trig_SD2FLT2_DRINT  = 102,
    CLA_Trig_SD2FLT3_DRINT  = 103,
    CLA_Trig_SD2FLT4_DRINT  = 104,
    CLA_Trig_PMBUSA_INT     = 105,
    CLA_Trig_SPIA_TXINT     = 109,
    CLA_Trig_SPIA_RXINT     = 110,
    CLA_Trig_SPIB_TXINT     = 111,
    CLA_Trig_SPIB_RXINT     = 112,
    CLA_Trig_LINA_INT1      = 117,
    CLA_Trig_LINA_INT0      = 118,
    CLA_Trig_LINB_INT1      = 119,
    CLA_Trig_LINB_INT0      = 120,
    CLA_Trig_BGCRC_INT      = 121,
    CLA_Trig_Reserved       = 122,
    CLA_Trig_FSITXA_INT1    = 123,
    CLA_Trig_FSITXA_INT2    = 124,
    CLA_Trig_FSIRXA_INT1    = 125,
    CLA_Trig_FSIRXA_INT2    = 126,
    CLA_Trig_CLB1_INT       = 127,
    CLA_Trig_CLB2_INT       = 128,
    CLA_Trig_CLB3_INT       = 129,
    CLA_Trig_CLB4_INT       = 130,
    CLA_Trig_HICA_INT       = 179,
    CLA_Trig_DMA_CH1INT     = 184,
    CLA_Trig_DMA_CH2INT     = 185,
    CLA_Trig_DMA_CH3INT     = 186,
    CLA_Trig_DMA_CH4INT     = 187,
    CLA_Trig_DMA_CH5INT     = 188,
    CLA_Trig_DMA_CH6INT     = 189
};

static inline void CLA_SetupRAM(void)
{
    // Set RAM Section LS2 and LS3 as shared data, LS4-7 as CLA program code
    MEM_SetLsRamMode(MEM_Sect_LS2, MEM_LsRamMode_CPU_CLA_Data);
    MEM_SetLsRamMode(MEM_Sect_LS3, MEM_LsRamMode_CPU_CLA_Data);
    MEM_SetLsRamMode(MEM_Sect_LS4, MEM_LsRamMode_CLA_Program);
    MEM_SetLsRamMode(MEM_Sect_LS5, MEM_LsRamMode_CLA_Program);
    MEM_SetLsRamMode(MEM_Sect_LS6, MEM_LsRamMode_CLA_Program);
    MEM_SetLsRamMode(MEM_Sect_LS7, MEM_LsRamMode_CLA_Program);
}

#endif /* SUPPORT_CLA_H_ */
